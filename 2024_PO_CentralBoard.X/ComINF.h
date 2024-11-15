#ifndef COMINF_H
#define	COMINF_H

#use rs232(UART1, baud=9600, xmit=PIN_C0, rcv=PIN_C1, stream=UART1)

#define TAILLE_BUFFER 20

unsigned int send_buf_Inf[TAILLE_BUFFER];
unsigned int rxBuffer[20]; 

int8 bonusFull = 0;
int8 elevatorBonus = 0;
int8 bonusBlock = 0;
int8 byteNumber = 0;

void ComINF_SendData();
void ComINF_Init();
void ComINF_Test();
void ComINF_MessageInfo(int16 idCar, int8 numMessage, int16 time);

#endif	/* COMINF_H */

