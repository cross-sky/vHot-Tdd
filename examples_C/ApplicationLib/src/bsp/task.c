#include "comm.h"

//add task
//1.add init 2.add task.h->task 3.add taskcomps

TPC_TASK TaskComps[]=
{
	{0, 0, 301,  301, LED1on},			// 5*200=1s   ok	RTCom2_task
	{0, 0, 302,  302, RTCom2_task},		//  rtcom2 test
	{0, 0, 20, 20, ComInputADC_process},//ADC
	{0, 0, 21,  21, RV_Task4OutProcess},	//rvout
//	{0, 0, 4,  4, UartDmaSendV2},
	//{0, 0, 1,  1, UartDmaSend},
};

void TaskInit(void)
{
	//��2�飺���2λ����ָ����ռʽ���ȼ�-PreemptionPriority�����2λ����ָ����Ӧ���ȼ�-SubPriority
	//���顱���ȼ���>������ռ���ȼ���>���������ȼ���
	//��ֵԽС����������ȼ���Խ��,�б�Ҫ������0
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	UB_LED_Init();		//PC1,PC3,PC13  ok	
	SystickInit();
	UART2_Init();
	ComInputADC_Init();
	RV_hwInit();
	//@@@@@@@@@��ʾ��Ҫ��ʱһ�������������Ҳû��ϵ���������ط�
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



