#include "comm.h"

static RunFun_T _funTable[SIG_FUN_MAX];

typedef struct _curFlag{
	uint16_t timeCount;		//��ʱ
	uint8_t recRunCount;	//���յ�������������Ϣ����
	uint8_t compStartCheck;	//��Ƶ�������̼��
	uint8_t compStartSuc;	//���������ɹ�����
}CurFlag_T;

static CurFlag_T S_curFlag={
	0, 0, 0, 0	
};

#define TimeBase 2	//2��/s
#define Time2S (2 * TimeBase)
#define Time5S (5 * TimeBase)
#define Time60S (60 * TimeBase)
#define Time90S (90 * TimeBase)
#define Time150S (150 * TimeBase)

void clearTimeCount(void) 
{
	S_curFlag.timeCount = 0;
}

uint16_t getTimeCount(void)
{
	return S_curFlag.timeCount++;
}

void addRecRunCount(void)
{
	S_curFlag.recRunCount++;
}

void clearRecRunCount(void)
{
	S_curFlag.recRunCount = 0;
}

uint8_t getRecRunCount(void)
{
	return S_curFlag.recRunCount;
}

void setCompStartCkeck(uint8_t isStart)
{
	S_curFlag.compStartCheck = isStart & 0x01;
}

uint8_t getCompStartCheck(void)
{
	return S_curFlag.compStartCheck;
}

void setCompStartSuc(uint8_t isStart)
{
	S_curFlag.compStartSuc = isStart & 0x01;
}

uint8_t getCompStartSuc(void)
{
	return S_curFlag.compStartSuc;
}

void clearCurFlag(void)
{
	clearTimeCount();
	clearRecRunCount();
	setCompStartCkeck(DONE);
}


STATEFUN_ENUM StateFun_funoff_exit(void)
{
	//printf("off run\r\n");
	return FUN_STATE_INIT;
}

STATEFUN_ENUM StateFun_funoff_init(void)
{
	//printf("off init\r\n");
	return FUN_STATE_RUN;
}

STATEFUN_ENUM StateFun_funoff_run(void)
{
	//printf("off run\r\n");
	return FUN_STATE_RUN;
}

STATEFUN_ENUM StateFun_funon_exit(void)
{
	//printf("on exit\r\n");
	return FUN_STATE_INIT;
}

STATEFUN_ENUM StateFun_funon_init(void)
{
	//printf("on init\r\n");
	return FUN_STATE_RUN;
}

STATEFUN_ENUM StateFun_funon_run(void)
{
	//printf("on run\r\n");
	return FUN_STATE_RUN;
}
static void driverCloseCheck(void)
{
	if (getCompStartCheck() == UNDONE)
	{
		if (MainData_rxDrGetStatus() == 1)
		{
			addRecRunCount();
		}
		else{
			clearRecRunCount();
		}

		if (getRecRunCount() >= 10)
		{
			setCompStartSuc(DONE);
			setCompStartCkeck(DONE);
		}
	}
}
//*************
STATEFUN_ENUM StateFun_funHeat_exit(void)
{
	uint16_t time;
	time = getTimeCount();
	switch(time)
	{
	case Time2S:
		MainData_txSetHz(30);	
		break;
	case Time60S:
		MainData_txSetHz(0);
		MainData_txSetCode(UNDONE); //send 0
		setCompStartCkeck(UNDONE); //
		break;
	case Time90S:
		if (!getCompStartSuc())
		{
			//��⵽һֱ������error
			//send  err sig
		}
		//set start check driver err
		RV_setRelay(RELAY1_PUMB, UNDONE);
		RV_setRelay(RELAY7_FUNH, UNDONE);
		RV_setRelay(RELAY8_FUNL, UNDONE);
		Valve_setToStep(VALVE_TYPE_MAINA, 30, VALVE_CLOSE);
		clearCurFlag();
		return FUN_STATE_INIT;
	default:
		break;
	}
	//������⣬��ⷵ�ص�P2λΪ6
	driverCloseCheck();
	//printf("heat init\r\n");
	return FUN_STATE_NULL;
}

static void driverStartCheck(void)
{
	if (getCompStartCheck() == UNDONE)
	{
		if (MainData_rxDrGetStatus() == 6)
		{
			addRecRunCount();
		}
		else{
			clearRecRunCount();
		}

		if (getRecRunCount() >= 10)
		{
			setCompStartSuc(DONE);
			setCompStartCkeck(DONE);
		}
	}
}

static void setValveToinitSteps(VALVEKINDLE_ENUM valveKindle)
{
	int16_t steps = 350 - Valve_getTotalSteps(valveKindle);
	Valve_setToStep(valveKindle, steps, VALVE_RUN);
}

STATEFUN_ENUM StateFun_funHeat_init(void)
{
	uint16_t time;
	time = getTimeCount();
	switch(time)
	{
	case Time2S:
		RV_setRelay(RELAY1_PUMB, DONE);
		break;
	case Time5S:
		//check water In
		//send  err sig

		RV_setRelay(RELAY7_FUNH, DONE);
		RV_setRelay(RELAY8_FUNL, DONE);
		setValveToinitSteps(VALVE_TYPE_MAINA);
		break;
	case Time60S:
		MainData_txSetHz(30);
		MainData_txSetCode(DONE);
		setCompStartCkeck(UNDONE);
		break;
	case Time90S:
		if (!getCompStartSuc())
		{
			//δ��⵽������Ϣ��error
			//send  err sig
		}
		//set start check driver err
		break;
	case Time150S:

		//final to run state
		clearCurFlag();
//		printf("heat init\r\n");
		return FUN_STATE_RUN;
	default:
		break;
	}
	//������⣬��ⷵ�ص�P2λΪ6
	driverStartCheck();
	//printf("heat init\r\n");
	return FUN_STATE_NULL;
}

STATEFUN_ENUM StateFun_funHeat_run(void)
{
	//printf("heat run\r\n");
	return FUN_STATE_RUN;
}

P_RunFun StateFun_create(void)
{
	_funTable[SIG_FUN_OFF].exit = StateFun_funoff_exit;
	_funTable[SIG_FUN_OFF].init = StateFun_funoff_init;
	_funTable[SIG_FUN_OFF].run = StateFun_funoff_run;
	_funTable[SIG_FUN_ON].exit = StateFun_funon_exit;
	_funTable[SIG_FUN_ON].init = StateFun_funon_init;
	_funTable[SIG_FUN_ON].run  = StateFun_funon_run;

	_funTable[SIG_FUN_HEAT].exit = StateFun_funHeat_exit;
	_funTable[SIG_FUN_HEAT].init = StateFun_funHeat_init;
	_funTable[SIG_FUN_HEAT].run  = StateFun_funHeat_run;

	return _funTable;
}

