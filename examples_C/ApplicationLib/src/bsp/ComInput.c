#include "ComInput.h"

void ComInput_Create()
{

}
// void ComInput_Create(P_ComInput src, P_ComInput dst)
// {
// 	assert(src);
// 	assert(dst);
// 	memcpy(dst, src, sizeof(ComInput_T));
// }
void ComInput_Destory(P_ComInput comInput)
{
	
}

static uint8_t getConvertFlag(P_ComInput comInput)
{
	return comInput->convertFlag;
}

static void setConvertFlag(P_ComInput comInput, uint8_t newFlag)
{
	comInput->convertFlag = newFlag;
}

static bool checkCountConvertComplete(P_ComInput comInput)
{
	return comInput->count >= comInput->maxCount;
}

static bool checkRunStageComplete(P_ComInput comInput)
{
	return comInput->runTime >= comInput->maxRunTime;
}

static bool checkHardConvertComplete(P_ComInput comInput)
{
	//return comInput->hardFlag == DONE;
	uint8_t result = comInput->hardFlagFun();
	return result == DONE;
}

static bool checkRunTimeComplete(P_ComInput comInput)
{
	return comInput->runTime >= comInput->maxRunTime;
}

static void addRunCount(P_ComInput comInput)
{
	comInput->count++;
}

static void addRunTime(P_ComInput comInput)
{
	comInput->runTime++;
}

static void addWaitTime(P_ComInput comInput)
{
	comInput->waitTime++;
}

static void clearWaitTime(P_ComInput comInput)
{
	comInput->waitTime = 0;
}
static void clearRunTime(P_ComInput comInput)
{
	comInput->runTime = 0;
}
static void clearRunCount(P_ComInput comInput)
{
	comInput->count = 0;
}

void ComInput_Process(P_ComInput comInput)
{
	if (getConvertFlag(comInput))
	{
		if (checkHardConvertComplete)
		{
			//check countTimes
			if (checkCountConvertComplete(comInput))
			{
				//comvert complete
				comInput->aveFun();
				setConvertFlag(comInput, DONE);
			}
			else{
				//convert count < 5
				//add run count
				addRunCount(comInput);
				clearWaitTime(comInput);
				//clear hardFlag
				comInput->clearHardFlagFun();
				comInput->hardFun();
			}
		}
		else
		{
			//wait+1, check wait
			addWaitTime(comInput);
		}
	}
	//add runTime;
	addRunTime(comInput);
	//check convert stage complete
	if (checkRunTimeComplete(comInput))
	{
		//new convet start
		setConvertFlag(comInput, UNDONE);
		clearRunCount(comInput);
		clearRunTime(comInput);
	}
}