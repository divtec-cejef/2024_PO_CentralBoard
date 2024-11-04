/*
 * #use rs232(UART2, baud=9600, xmit=PIN_D1, rcv=PIN_B0, stream=INF)

unsigned int send_buf_Inf[9];
unsigned int rx_buf[8];

void ComINF_SendData();
*/


#include "ComINF.h"
void ComINF_Init()
{
    //Enable the interrupt INT_RDA1 because it's UART2 to receive data
    //enable_interrupts(INT_RDA1);
    //enable_interrupts(GLOBAL);
}

void ComINF_Test()
{
    send_buf_Inf[0]='1';
    send_buf_Inf[1]='2';
    send_buf_Inf[2]='3';
    send_buf_Inf[3]='4';
    send_buf_Inf[4]='5';
    send_buf_Inf[5]='6';
    send_buf_Inf[6]='7';
        
    ComINF_SendData();
}

    void ComINF_MessageInfo(int16 idCar, int8 numMessage, int16 time)
    {
        printf("%04Lu,%02d,%04Lu",idCar,numMessage,time);
    }
  
void ComINF_SendData()
{
    unsigned int i;
    for(i = 0; i < 8; i ++){
      printf("%c",send_buf_Inf[i]);
   };
}

//Interruption de réception de caractère
#INT_RDA
void Rx_Da_INT(void)
{ 
    //output_toggle(PIN_B4);
    rxBuffer[byteNumber] = fgetc(UART1);
    
    byteNumber++;
    if(byteNumber == 8)
    {
        byteNumber=0;
        
        if(bonusBlock == 0)
        {
            bonusFull = 1;
            
            for(int i = 0; i<8;i++)
            {
                fputc(rxBuffer[i],UART1);
            }

            //output_toggle(PIN_B4);
        }
        
    }
}