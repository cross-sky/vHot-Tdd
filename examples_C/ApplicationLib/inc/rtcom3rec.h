#ifndef __RTCOM3REC_H
#define __RTCOM3REC_H

#include "rtcom3.h"

bool RTCom3Rec_checkHeader(P_RTCom3DMA dmaAddr);
bool RTCom3Rec_checkFrame(P_RTCom3DMA dmaAddr);
void RTCom3Rec_dmaRecData(P_RTCom3DMA dmaAddr);

void RTCom3Rec_createEvent(void);
bool RTCom3Rec_popEvent(void* dataAddr);
bool RTCom3Rec_pushEvent(void* dataAddr, uint16_t dataLen);
uint16_t RTCom3Rec_lengthEvent(void);
void RTCom3Rec_destoryEvent(void);


#endif
