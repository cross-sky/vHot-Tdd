#ifndef __QUEUE_H
#define __QUEUE_H

#include "commdata.h"

#define  QUEUELENGTH 8

typedef struct T* P_Queue;
typedef struct T Queue_T;

typedef struct DataStruct{
	int16_t len[QUEUELENGTH];
	void *buff[QUEUELENGTH];
}DataStruct;

struct T{
	int8_t in;
	int8_t out;
	int8_t maxSize;
	int8_t qLen;
	DataStruct data;
};

void Queue_create(P_Queue que, void* dataAddr, uint16_t dataSize);
bool Queue_pop(P_Queue que, void* dstAddr);
bool Queue_push(P_Queue que, void* dataAddr, uint16_t dataLen);
uint16_t Queue_length(P_Queue que);
void Queue_destory(P_Queue que);

#endif
