#include "comm.h"

RunFun_T _funTable[SIG_FUN_MAX];

STATEFUN_ENUM StateFun_funoff_exit(void)
{
	printf("off run\r\n");
	return FUN_STATE_INIT;
}

STATEFUN_ENUM StateFun_funoff_init(void)
{
	printf("off init\r\n");
	return FUN_STATE_RUN;
}

STATEFUN_ENUM StateFun_funoff_run(void)
{
	printf("off run\r\n");
	return FUN_STATE_RUN;
}

STATEFUN_ENUM StateFun_funon_exit(void)
{
	printf("on exit\r\n");
	return FUN_STATE_INIT;
}

STATEFUN_ENUM StateFun_funon_init(void)
{
	printf("on init\r\n");
	return FUN_STATE_RUN;
}

STATEFUN_ENUM StateFun_funon_run(void)
{
	printf("on run\r\n");
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

	return _funTable;
}