#include "comm.h"

//创建全局队列
static Queue_T S_sendQue;
//事件保存地址
static Event_T S_event[QUEUELENGTH];
//状态函数地址
static P_RunFun S_funTable;
//转换状态函数地址
static P_Fundata S_pfun;
static Fundata_T S_fundata;
//转换时要执行的函数
static pRunFunctions S_changeFunList[FUN_STATE_MAX];

void Funstate_createEvent(void)
{
	//储存长度要转换为字节长度
	Queue_create(&S_sendQue, S_event, sizeof(S_event)/QUEUELENGTH);
}
bool Funstate_popEvent(P_Event funEvent)
{
	return Queue_pop(&S_sendQue, funEvent);
}
bool Funstate_pushEvent(P_Event funEvent)
{
	return Queue_push(&S_sendQue, funEvent, sizeof(Event_T));
}
uint16_t Funstate_lengthEvent(void)
{
	return Queue_length(&S_sendQue);
}
void Funstate_destoryEvent(void)
{
	Queue_destory(&S_sendQue);
}

static void setChangeFunList(void)
{
	S_changeFunList[FUN_STATE_EXIT] = S_funTable[S_pfun->preFun].exit;
	S_changeFunList[FUN_STATE_INIT] = S_funTable[S_pfun->curFun].init;
	S_changeFunList[FUN_STATE_RUN] = S_funTable[S_pfun->curFun].run;
}

void Fundata_create(P_Fundata fundata)
{
	fundata->isChange = DONE;
	fundata->curFunState = FUN_STATE_RUN;
	fundata->curFun = SIG_FUN_OFF;
	fundata->preFun = SIG_FUN_OFF;
	S_pfun = fundata;

	S_funTable = StateFun_create();
	setChangeFunList();
}
void Fundata_destory(P_Fundata fundata)
{
	Fundata_create(fundata);
}

static DONE_ENUM checkIsFunChangeDone(void)
{
	return S_pfun->isChange;
}

static void setChangeFlag(DONE_ENUM isDone)
{
	S_pfun->isChange = isDone;
}

static void setFunChangeState(STATEFUN_ENUM newFunState)
{
	S_pfun->curFunState = newFunState;
	//when fun state change to run, then set change done
	if (newFunState == FUN_STATE_RUN && checkIsFunChangeDone() == UNDONE)
	{
		setChangeFlag(DONE);
	}
}



static pRunFunctions getRunfun(void)
{
	STATEFUN_ENUM funState = Fundata_getFunChangeState();
	return S_changeFunList[funState];
}

static void savePreRunFun(pRunFunctions preFun)
{
	S_changeFunList[FUN_STATE_NULL] = preFun;
}

static void setPrepareValueToChange(Event_T *_event)
{
	//3.check event is not cur
	if (_event->eventId != Fundata_getCurFunSig())
	{
		//set necessary value
		S_pfun->preFun = S_pfun->curFun;
		S_pfun->curFun = (FUN_SIG_ENUM)_event->eventId;
		//4.1 set change flag
		setChangeFlag(UNDONE);
		//4.2 set change fun state
		setFunChangeState(FUN_STATE_EXIT);
		//4.3 set change fun
		setChangeFunList();
	}
}

static void checkNewEvent(void)
{
	Event_T _event;
	//1.1 check curchange is done
	if (checkIsFunChangeDone())
	{
		//1.check if any state need to change
		if (Funstate_lengthEvent() > 0)
		{
			//2.get event
			Funstate_popEvent(&_event);
			//3.set necessary value
			setPrepareValueToChange(&_event);
		}
	}
}

void Funstate_processEvent(void)
{
	pRunFunctions runFun;
	STATEFUN_ENUM runResult;

	checkNewEvent();
	//get pfun 
	runFun = getRunfun();
	runResult = runFun();
	//4.save runResult
	setFunChangeState(runResult);
	//4.1 save preRunfun	
	savePreRunFun(runFun);
	//return true;
}

FUN_SIG_ENUM Fundata_getCurFunSig(void)
{
	return S_pfun->curFun;
}

STATEFUN_ENUM Fundata_getFunChangeState(void)
{
	return S_pfun->curFunState;
}

void Fundata_init(void)
{
	Funstate_createEvent();
	Fundata_create(&S_fundata);
}

void Funstate_sendSig(FUN_SIG_ENUM sig)
{
	Event_T fevent;
	if (sig > SIG_FUN_MAX)
	{
		return;
	}
	fevent.eventType = FUN_TYPE;
	fevent.eventId = sig;
	Funstate_pushEvent(&fevent);
}
