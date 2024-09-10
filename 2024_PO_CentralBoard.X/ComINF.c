#use rs232(UART2, baud=115200, xmit=PIN_D1, rcv=PIN_B0, stream=INF)

unsigned int send_buf_Inf[9];
unsigned int rx_buf[8];

void ComINF_SendData();

void ComINF_Init()
{
    //Enable the interrupt INT_RDA2 because it's UART2 to receive data
    enable_interrupts(INT_RDA2);
    enable_interrupts(GLOBAL);
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

//Send message to INF
void ComINF_Message(int messageNb, int secondes, int centiemes)
{
    output_toggle(PIN_A6);
    //format message "m:sscc"    
    //m : numéro de message
    //ss: secondes 0-99
    //cc: centièmes 0-99
    printf("%d:%02d%02d",messageNb,secondes,centiemes);
    output_toggle(PIN_A6);
}

void ComINF_SendData()
{
    unsigned int i;
    for(i = 0; i < 8; i ++){
      printf("%c",send_buf_Inf[i]);
   };
}

//Interruption de réception de caractère
#INT_RDA2
void Rx_Da_INT(void){
    static int8 nb=0;
    
    //output_toggle(PIN_A6);
    rx_buf[nb] = fgetc(INF);
    
    nb++;
    if(nb == 6)
    {
        //printf("finiFred");
        
        for(int i =0; i<6;i++)
        {
            fputc(rx_buf[i],INF);
        }
        nb=0;
        output_toggle(PIN_A6);
    }
    

}
