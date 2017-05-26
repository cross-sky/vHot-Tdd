#include "comm.h"

//创建全局队列
static Queue_T S_sendQue;
//事件保存地址
static EventValve_T S_event[QUEUELENGTH];

static ValveStatus_T S_valveTab[VALVE_TYPE_MAX]={
	{DONE, 500, 0,  VALVE_HOLD, VALVE_USED},
	{DONE, 500, 0,  VALVE_HOLD, VALVE_USED},
};

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

static bool isValveUsed(VALVEKINDLE_ENUM valveKind)
{
	return S_valveTab[valveKind].isUsed == VALVE_USED;
}


void Valve_createEvent(void)
{
	//储存长度要转换为字节长度
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
	//开度有限制 确保totalstep和runstep在范围内
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

static int16_t getValveStep(VALVEKINDLE_ENUM valveKind)
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
			//2. 步数结束时，先将电子膨胀阀的值清0，避免通电发热
			//3.刚开机时，会从步数1开始
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
	int16_t runstep = getValveStep(valveKind);
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
	//@@@@@发送后，设置为未使用，事件就无法处理

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

void Valve_taskProcess(void)
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
