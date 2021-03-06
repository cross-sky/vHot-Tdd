#ifndef __MAINDATA_H
#define __MAINDATA_H

#include "commdata.h"

void MainData_txSetHz(uint8_t hz);
uint8_t MainData_txGetHz(void);
void MainData_txSetCode(uint8_t code);
uint8_t MainData_txGetCode(void);
void MainData_txSetUpspeed(uint8_t upspeed);
uint8_t MainData_txGetUpspeed(void);
void MainData_txSetDownspeed(uint8_t downspeed);
uint8_t MainData_txGetDownspeed(void);

void MainData_rxConvert(P_RTCom3RFrame1 rec1);
uint8_t MainData_rxDrGetStatus(void);

void MainData_initPara(void);
uint16_t MainData_printDriverMessage(char* dst, uint16_t maxSize);

uint16_t MainData_getPower(void);

#endif

