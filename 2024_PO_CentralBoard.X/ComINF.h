/* 
 * File:   ComINF.h
 * Author: choffre
 *
 * Created on 13. septembre 2024, 14:41
 */

#ifndef COMINF_H
#define	COMINF_H

#use rs232(UART1, baud=9600, xmit=PIN_C0, rcv=PIN_C1, stream=UART1)

unsigned int send_buf_Inf[9];
unsigned int rxBuffer[20];
int8 bonus = 0;
int8 bonusFull = 0;
int8 elevatorBonus = 0;
int8 bonusBlock = 0;
int8 byteNumber = 0;

void ComINF_SendData();
void ComINF_Init();
void ComINF_Test();

void ComINF_MessageInfo(int16 idCar, int8 numMessage, int16 time);

#endif	/* COMINF_H */

