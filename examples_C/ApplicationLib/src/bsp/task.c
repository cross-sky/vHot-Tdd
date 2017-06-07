#include "comm.h"

//add task
//1.add init 2.add task.h->task 3.add taskcomps

TPC_TASK TaskComps[]=
{
	{0, 0, 301,  301, LED1on},			// 5*200=1s   ok	RTCom2_task
	{0, 0, 302,  302, RTCom2_task},		//  rtcom2 test
	{0, 0, 20, 20, ComInputADC_DIProcess},//ADC - di
	{0, 0, 6,  6, RVOUT_taskProcess},	//rvout 
	{0, 0, 299,  299, UART3_RTProcess}, //rs485 
	{0, 0, 100,  100, Funstate_processEvent},
	{0, 0, 200,  200, ValveCalc_task}, //valve calc task
//	{0, 0, 4,  4, UartDmaSendV2},
	//{0, 0, 1,  1, UartDmaSend},
};

void TaskInit(void)
{
	//第2组：最高2位用于指定抢占式优先级-PreemptionPriority，最低2位用于指定响应优先级-SubPriority
	//“组”优先级别>“抢”占优先级别>“副”优先级别
	//数值越小所代表的优先级就越高,有必要设置组0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	MainData_initPara();

	UB_LED_Init();		//PC1,PC3,PC13  ok	
	SystickInit();
	UART2_Init();
	ComInputADC_Init();
	ComInputDI_Init();
	RV_hwInit();
	Valve_hwInit();
	RTCom1_init();
	UART3_Init();

	MainData_initPara();
	Fundata_init();
	//@@@@@@@@@显示板要延时一点才启动，不过也没关系，后面有重发
}


void TPCRemarks( TPC_TASK *pTask)
{
	u8 i;
	
	for ( i=0; i<TASKS_MAX; i++)
	{
		if( pTask[i].Timer > 0 && pTask[i].Suspend == 0)
		{
			pTask[i].Timer--;
			if (pTask[i].Timer == 0)
			{
				pTask[i].Timer = pTask[i].ItvTime;
				pTask[i].Run = TPC_RUN_STM; 
			}
		}
	}
}

void TPCProcess(TPC_TASK *pTask)
{
	u8 i;

	for (i=0; i<TASKS_MAX; i++)
	{
		if (pTask[i].Run == TPC_RUN_STM && pTask[i].Suspend == 0)
		{
			pTask[i].TaskHook();
			pTask[i].Run = TPC_RUN_CLM;
		}

	}
}

void Task_changeLedTime(uint8_t normals)
{
	if (normals & 0x01)
	{
		TaskComps[TASK_LED1ON].ItvTime = 101;
	}
	else
	{
		TaskComps[TASK_LED1ON].ItvTime = 301;
	}
}



