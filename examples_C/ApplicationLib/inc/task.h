#ifndef __TASK_H
#define __TASK_H


//1.枚举定义任务列表;
//2.任务初始化;
//3.TaskComps增加任务初始化的数据;

#include "commdata.h"

//增加程序挂起标记
typedef struct _TPC_TASK 
{
	u8 Run;   //运行标记，0-不运行，1-运行；
	u8 Suspend; //挂起标记，0-不挂起，1-挂起；
	u16 Timer;  //计时器；
	u16 ItvTime;  // 任务运行间隔时间；
	void ( *TaskHook )(void);  //运行的任务函数；
} TPC_TASK;



typedef enum
{
	
	TPC_RUN_CLM = 0x00,  //任务停止;
	TPC_RUN_STM = 0x01  //任务运行;
	
} TPC_RUN_MASK;


//定义任务列表；
typedef enum 
{
	TASK_LED1ON,
	TASK_UART2,
	TASK_ADC,
	TASK_RV,
	TASK_UART3,
	TASK_FUNSTATE,
	TASK_VALVECALC,
	TASKS_MAX
} TASK_LIST;

extern void TaskInit(void);
extern void TPCRemarks( TPC_TASK *pTask);
extern void TPCProcess(TPC_TASK *pTask);
extern TPC_TASK TaskComps[];

void Task_changeLedTime(uint8_t normals);


#endif

