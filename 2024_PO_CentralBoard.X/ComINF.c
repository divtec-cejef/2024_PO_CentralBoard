#include "ComINF.h"
void ComINF_Init()
{

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
    for(i = 0; i < TAILLE_BUFFER; i ++)
    {
      printf("%c",send_buf_Inf[i]);
    }
}

//== INTERRUPTION DE RÉCEPTION DE CARACTÈRE ==//
#INT_RDA
void Rx_Da_INT(void)
{  
    rxBuffer[byteNumber] = fgetc(UART1);
    
    byteNumber++;
    if(byteNumber == TAILLE_BUFFER)
    {
        byteNumber=0;
        
        if(bonusBlock == 0)
        {
            bonusFull = 1;
            
            for(int i = 0; i<TAILLE_BUFFER ;i++)
            {
                fputc(rxBuffer[i],UART1);
            } 
        }
    }
}

