/* 
 * File:   ComINF.h
 * Author: choffre
 *
 * Created on 13. septembre 2024, 14:41
 */

#ifndef COMINF_H
#define	COMINF_H

#use rs232(UART2, baud=9600, xmit=PIN_D1, rcv=PIN_B0, stream=INF)

unsigned int send_buf_Inf[9];
unsigned int rx_buf[8];

void ComINF_SendData();
void ComINF_Init();
void ComINF_Test();
void ComINF_Message(int messageNb, int secondes, int centiemes);

#endif	/* COMINF_H */

