#include "comm.h"

//����ȫ�ֶ���
static Queue_T S_sendQue;
//�¼������ַ
static EventValve_T S_event[QUEUELENGTH];

//
static uint16_t S_valveCalcCount = 0;

static ValveStatus_T S_valveTab[VALVE_TYPE_MAX]={
	{DONE, 500, 0, VALVE_USED, VALVE_STEPS_ONECE, DirectHold, 0},
	{DONE, 500, 0, VALVE_USED, VALVE_STEPS_ONECE>>1, DirectHold, 0},
};

ValveStatus_T* prtvalveStatus= S_valveTab;

typedef enum {
	IndexInitStep,
	IndexCloseStep,
	IndexMinStep,
	IndexMaxStep,
	ValveEnumMax
}STEPS_Enum;

#define  VALVE_INIT_STEP   -560
#define VALVE_CLOSE_STEP  520
#define VALVE_MIN_STEP 30
#define VALVE_MAX_STEP 470

static int16_t S_stepsTab[VALVE_TYPE_MAX][ValveEnumMax]={
	{VALVE_INIT_STEP,VALVE_CLOSE_STEP,VALVE_MIN_STEP,VALVE_MAX_STEP},
	{VALVE_INIT_STEP,VALVE_CLOSE_STEP,VALVE_MIN_STEP,VALVE_MAX_STEP-200}
};

static uint8_t S_calcFlag = 0;

static bool isValveUsed(VALVEKINDLE_ENUM valveKind)
{
	return S_valveTab[valveKind].isUsed == VALVE_USED;
}

static void initValveTab(VALVEKINDLE_ENUM valveKind)
{
	prtvalveStatus[valveKind].isDone = DONE;
	prtvalveStatus[valveKind].totalSteps = 500;
	prtvalveStatus[valveKind].runSteps = 0;
	prtvalveStatus[valveKind].isUsed = VALVE_USED;
	prtvalveStatus[valveKind].calcLastStep = VALVE_STEPS_ONECE;
	prtvalveStatus[valveKind].calcDirection = DirectHold;
	prtvalveStatus[valveKind].calcCounts = 0;
}


void Valve_createEvent(void)
{
	//���泤��Ҫת��Ϊ�ֽڳ���
	Queue_create(&S_sendQue, S_event, sizeof(S_event)/QUEUELENGTH);
}
bool Valve_popEvent(P_EventValve event)
{
	return Queue_pop(&S_sendQue, event);
}
bool Valve_getEvent(P_EventValve event)
{
	return Queue_get(&S_sendQue, event);
}
bool Valve_pushEvent(P_EventValve event)
{
	if (!isValveUsed((VALVEKINDLE_ENUM)event->event.eventId))
	{
		return false;
	}
	return Queue_push(&S_sendQue, event, sizeof(EventValve_T));
}
uint16_t Valve_lenEvent(void)
{
	return Queue_length(&S_sendQue);
}
void Valve_destoryEvent(void)
{
	Queue_destory(&S_sendQue);
	initValveTab(VALVE_TYPE_MAINA);
	initValveTab(VALVE_TYPE_SUBB);
}

static bool isValveDone(VALVEKINDLE_ENUM valveKind)
{
	return S_valveTab[valveKind].isDone == DONE;
}


static void checkTotalSteps(P_EventValve event)
{
	uint16_t tstate = event->state;
	int16_t tcode = event->code;
	uint16_t valveKind = event->event.eventId;
	P_ValveStatus valve = &S_valveTab[valveKind];

	if ( VALVE_INIT == tstate || VALVE_CLOSE == tstate)
	{
		//init
		if (tstate == VALVE_INIT)
		{
			valve->runSteps = S_stepsTab[valveKind][IndexInitStep];
			valve->totalSteps = 0;
		}else
		{
			//close
			valve->runSteps = S_stepsTab[valveKind][IndexCloseStep] - valve->totalSteps;
			valve->totalSteps = 500;
		}
	}
	//���������� ȷ��totalstep��runstep�ڷ�Χ��
	else{
		if ((valve->totalSteps + tcode) > S_stepsTab[valveKind][IndexMaxStep] )
		{
			valve->runSteps = S_stepsTab[valveKind][IndexMaxStep] - valve->totalSteps;
			valve->totalSteps = S_stepsTab[valveKind][IndexMaxStep];
		}
		else if ((valve->totalSteps + tcode) < S_stepsTab[valveKind][IndexMinStep])
		{
			valve->runSteps = S_stepsTab[valveKind][IndexMinStep] - valve->totalSteps;
			valve->totalSteps = S_stepsTab[valveKind][IndexMinStep];
		}else{
			valve->runSteps = tcode;
			valve->totalSteps += tcode;
		}
	}

	//update totalSteps;
}

static void setValveState(DONE_ENUM isDone, VALVEKINDLE_ENUM valveKind)
{
	S_valveTab[valveKind].isDone = isDone;
}

static int16_t getValveRunStep(VALVEKINDLE_ENUM valveKind)
{
	return S_valveTab[valveKind].runSteps;
}

static void valveRunDone(VALVEKINDLE_ENUM valveKind)
{
	static uint8_t i = 0;
	//undone
	if (!isValveDone(valveKind))
	{
		if (i >= 10)
		{
			i = 0;
			setValveState(DONE, valveKind);
			//2. ��������ʱ���Ƚ��������ͷ���ֵ��0������ͨ�緢��
			//3.�տ���ʱ����Ӳ���1��ʼ
			RV_clearValveValue(valveKind);
		}
		i++;
	}
}

static void stepChange(VALVESTATE_ENUM addOrSub, VALVEKINDLE_ENUM valveKind)
{
	assert(addOrSub == VALVE_ADD || addOrSub == VALVE_BACK);
	if (addOrSub == VALVE_ADD)
	{
		S_valveTab[valveKind].runSteps++;
	}else
		S_valveTab[valveKind].runSteps--;
}
static void valveRun(VALVEKINDLE_ENUM valveKind)
{
	int16_t runstep = getValveRunStep(valveKind);
	if (runstep == 0)
	{
		valveRunDone(valveKind);
		return;
	}else if (runstep < 0)
	{
		//add count, but IO out back
		stepChange(VALVE_ADD, valveKind);
		RV_valveRunDirect(VALVE_BACK, valveKind);
	}else{
		//sub count, but IO out add
		stepChange(VALVE_BACK, valveKind);
		RV_valveRunDirect(VALVE_ADD, valveKind);
	}
}

void Valve_ProcessEvent(VALVEKINDLE_ENUM valveKind)
{
	EventValve_T event;
	//@@@@@���ͺ�����Ϊδʹ�ã��¼����޷�����

	if (Valve_getEvent(&event))
	{
		//done 
		if (isValveDone(valveKind) && event.event.eventId == valveKind)
		{
			//set start 
			//1.check total step
			checkTotalSteps(&event);
			//2.set run
			setValveState(UNDONE, valveKind);
			Valve_popEvent(&event);
			return;
		}
	}
	valveRun(valveKind);
}

uint16_t Valve_getState(VALVEKINDLE_ENUM valveKind)
{
	return S_valveTab[valveKind].isDone;
}

void RVOUT_taskProcess(void)
{
	Valve_ProcessEvent(VALVE_TYPE_MAINA);
	Valve_ProcessEvent(VALVE_TYPE_SUBB);

	//update HWdata
	RV_Task4OutProcess();
}

void Valve_hwInit(void)
{
	//EventValve_T initEvent;
	Valve_createEvent();
	//send init event

	Valve_setToStep(VALVE_TYPE_MAINA, 30, VALVE_INIT);
	Valve_setToStep(VALVE_TYPE_SUBB, 30, VALVE_INIT);
}

void Valve_setToStep(VALVEKINDLE_ENUM valveKindle, int16_t steps, VALVESTATE_ENUM state)
{
	EventValve_T initEvent;

	if (steps == 0)
		return;
	initEvent.event.eventType = VALVE_TYPE;
	initEvent.event.eventId = valveKindle;
	initEvent.code = steps;
	initEvent.state = state;
	Valve_pushEvent(&initEvent);
}

int16_t Valve_getTotalSteps(VALVEKINDLE_ENUM valvekindle)
{
	if (valvekindle < VALVE_TYPE_MAX)
	{
		return S_valveTab[valvekindle].totalSteps;
	}
	return 0;
}

static void valveDirectHold(int16_t subT, int16_t superHeat,VALVEKINDLE_ENUM valveKind )
{
	//Ŀ����ȶȷ�ΧT+0.5 
	if (subT > superHeat+5)
	{
		//1.���������ϴΣ����ȼӴ󣬴���+1 TabValveDirect[IndexDirectHold],IndexDirectBack,IndexDirectHold,IndexDirectForward,
		//DirectBack,DirectHold,DirectForward
		prtvalveStatus[valveKind].calcDirection = DirectForward;
		prtvalveStatus[valveKind].calcCounts +=1;
	}
	else if (subT < superHeat - 5)
	{
		//2.���������ϴΣ����ȼ��٣�����+1
		prtvalveStatus[valveKind].calcDirection = DirectBack;
		prtvalveStatus[valveKind].calcCounts +=1;
	}
	else{
		//���������ȱ��֣�������0
		prtvalveStatus[valveKind].calcDirection = DirectHold;
		prtvalveStatus[valveKind].calcCounts = 0;
	}
}

static void valveDirectForward(int16_t subT, int16_t superHeat,VALVEKINDLE_ENUM valveKind)
{
	//Ŀ����ȶȷ�ΧT+0.5 
	if (subT > superHeat + 5)
	{
		//1.���������ϴΣ����ȼӴ󣬴���+1
		prtvalveStatus[valveKind].calcDirection = DirectForward;
		prtvalveStatus[valveKind].calcCounts +=1;
	}
	else if (subT < superHeat - 5)
	{
		//2.�������룬���ȼ��٣���������
		prtvalveStatus[valveKind].calcLastStep >>=1;
		prtvalveStatus[valveKind].calcDirection = DirectBack;
	}
	else{
		//���������ȱ��֣�������0
		prtvalveStatus[valveKind].calcDirection = DirectHold;
		prtvalveStatus[valveKind].calcCounts = 0;
	}
}

static void valveDirectBack(int16_t subT, int16_t superHeat,VALVEKINDLE_ENUM valveKind)
{
	//Ŀ����ȶȷ�ΧT+0.5 
	if (subT > superHeat + 5)
	{
		//1.�������룬���ȼӴ󣬴�������
		prtvalveStatus[valveKind].calcLastStep >>=1;
		prtvalveStatus[valveKind].calcDirection = DirectForward;
	}
	else if (subT < superHeat - 5)
	{
		//2.���������ϴΣ����ȼ��٣�����+1
		prtvalveStatus[valveKind].calcDirection = DirectBack;
		prtvalveStatus[valveKind].calcCounts +=1;
	}
	else{
		//���������ȱ����ϴΣ�������0
		prtvalveStatus[valveKind].calcDirection = DirectHold;
		prtvalveStatus[valveKind].calcCounts = 0;
	}
}

static bool getLastValveClacDirect(VALVEKINDLE_ENUM valveKind, VALVESTATE_ENUM valveState)
{
	//DirectHold
	return prtvalveStatus[valveKind].calcDirection == valveState;
}

static void checkValveCalcCounts(VALVEKINDLE_ENUM valveKind)
{
	if (prtvalveStatus[valveKind].calcCounts >=4)
	{
		if (prtvalveStatus[valveKind].calcDirection == DirectForward || prtvalveStatus[valveKind].calcDirection == DirectBack)
		{
			prtvalveStatus[valveKind].calcLastStep = VALVE_STEPS_ONECE;
			prtvalveStatus[valveKind].calcCounts = 0;
		}
	}
}

static int16_t calcValveStepsMainA(VALVEKINDLE_ENUM valveKind, int16_t subT, int16_t superHeat)
{
	//3.1�ϴε������ͷ�ά�ֿ���
	if (getLastValveClacDirect(valveKind, DirectHold))
	{
		valveDirectHold(subT,superHeat,valveKind);
	}
	//3.2�ϴε������ͷ���������
	else if (getLastValveClacDirect(valveKind, DirectForward))
	{
		valveDirectForward(subT,superHeat,valveKind);
	}
	//3.3�ϴε������ͷ����ȼ���
	else
	{
		valveDirectBack(subT,superHeat,valveKind);
	}
	//4.�������ӻ���ٴ���>=4�����ò����ʹ���
	checkValveCalcCounts(valveKind);

	//����<1,������1
	if (prtvalveStatus[valveKind].calcLastStep <= 1 )
	{
		prtvalveStatus[valveKind].calcLastStep = 1;
	}

	//5.���Ͳ���LastStep*ValveDirection, valveRun,
	//����*���� ��������ת
	return prtvalveStatus[valveKind].calcLastStep*(int16_t)(prtvalveStatus[valveKind].calcDirection - DirectHold);	
	//return code;
}

//�������������ͷ�
//void ValveCalc_calcValveMain(VALVEKINDLE_ENUM valveKind)
//{
//	int16_t superHeat,subT;
//	int16_t code;
//
//	//0.1 �����¶�>100,
//	if (ADC_getAOut() > AirOutTemperMax100)
//	{
//		code = (ADC_getAOut()- AirOutTemperMax100+10)/10;
//		//
//		Valve_setToStep(VALVE_TYPE_MAINA, code, VALVE_RUN);
//		return;
//	}
//
//	//1. ��������-����
//	subT = ADC_getAIN() - ADC_getMEva();
//	//2.��ȡĿ����ȶ�
//	superHeat = ADC_getSuperHeat();
//
//	//3.calc valve steps
//	code = calcValveStepsMainA(valveKind, subT, superHeat);
//	//@@@@@@@@@@@@@@@@
//	//min step to 150
//	if (code + Valve_getTotalSteps(VALVE_TYPE_MAINA) < 150)
//	{
//		return;
//	}
//
//	//push to queue
//	Valve_setToStep(VALVE_TYPE_MAINA, code, VALVE_RUN);
//}

void ValveCalc_calcValveMain(VALVEKINDLE_ENUM valveKind)
{
	int16_t superHeat,subT;
	int16_t code;

	//0.1 �����¶�>100,
	if (ADC_getAOut() > AirOutTemperMax100)
	{
		//k = 10,
		code = (ADC_getAOut()- AirOutTemperMax100);
		//
		Valve_setToStep(VALVE_TYPE_MAINA, code, VALVE_RUN);
		return;
	}

	//1. ��������-����
	subT = ADC_getAIN() - ADC_getAINSaturation();
	//2.��ȡĿ����ȶ�
	superHeat = ADC_getSuperHeat();

	//3.calc valve steps
	code = calcValveStepsMainA(valveKind, subT, superHeat);
	//@@@@@@@@@@@@@@@@
	//min step to 150
	if (code + Valve_getTotalSteps(VALVE_TYPE_MAINA) < 120)
	{
		return;
	}

	//push to queue
	Valve_setToStep(VALVE_TYPE_MAINA, code, VALVE_RUN);
}

static bool checkEnvAboveT(VALVEKINDLE_ENUM valveKind)
{
	if (ADC_getEnv() >= 50)
	{
		//close sub valve
		if (isValveUsed(valveKind))
		{
			Valve_setToStep(valveKind, 30, VALVE_INIT);
			prtvalveStatus[valveKind].isUsed = VALVE_UNUSED;
		}
		return true;
	}
	return false;
}

void ValveCalc_calcValveSub(VALVEKINDLE_ENUM valveKind)
{
	int16_t superHeat,subT;
	int16_t code;

	////0.1 �����¶�>100,
	//if (ADC_getAOut() > AirOutTemperMax100)
	//{
	//	//k = 10,
	//	code = (ADC_getAOut()- AirOutTemperMax100+10)/10;
	//	//
	//	Valve_setToStep(valveKind, code, VALVE_RUN);
	//	return;
	//}

	//env > 5, exit
	if (checkEnvAboveT(valveKind))
		return;

	//env < 5, then start sub valve
	if (!isValveUsed(valveKind))
	{
		prtvalveStatus[valveKind].isUsed = VALVE_USED;
		//Valve_setToStep(valveKind, 30, VALVE_RUN);
		//return;
	}

	//1. ��������-����
	subT = ADC_getEconOut() - ADC_getEconIn();
	//2.��ȡĿ����ȶ�
	superHeat = ADC_getEconomizerHeat();

	//3.calc valve steps
	code = calcValveStepsMainA(valveKind, subT, superHeat);
	//@@@@@@@@@@@@@@@@
	//min step to 30
	if (code + Valve_getTotalSteps(valveKind) < 40)
		code = 40 - Valve_getTotalSteps(valveKind);

	//push to queue
	Valve_setToStep(valveKind, code, VALVE_RUN);
}

static bool checkStartValveCalc(void)
{
	return S_calcFlag == DONE;
}

void ValveCalc_task(void)
{
	if (S_valveCalcCount >= 60)
	{
		if (checkStartValveCalc())
		{
			ValveCalc_calcValveMain(VALVE_TYPE_MAINA);
			//ValveCalc_calcValveSub(VALVE_TYPE_SUBB);
		}

		S_valveCalcCount = 0;
	}
	S_valveCalcCount++;
}

void ValveClac_closeClac(VALVEKINDLE_ENUM valveKind)
{
	//prtvalveStatus[valveKind].isUsed = VALVE_UNUSED;
	S_calcFlag = UNDONE;
}

void ValveClac_startClac(VALVEKINDLE_ENUM valveKind)
{
	S_valveCalcCount = 0;
	S_calcFlag = DONE;
	//prtvalveStatus[valveKind].isUsed = VALVE_USED;
}
