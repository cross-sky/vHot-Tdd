#ifndef __COMMDATA_H
#define __COMMDATA_H

#include "commdata.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "assert.h"

typedef enum{
	UNDONE, DONE
}DONE_ENUM;

#define bool char
#define true 1
#define false 0

typedef void (*pvNormalFun)(void);
typedef uint8_t (*pu8NormalFun)(void);
#endif
