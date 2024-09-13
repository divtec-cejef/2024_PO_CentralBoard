/******************************************************************************/
/*                                                                            */
/*                                CEJEF                                       */
/*                          Division Technique                                */
/*                            EMT Porrentruy                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Project     : lecteur MP3                                                  */
/* Author      : Choffat Frédéric                                             */
/* Target      : PIC 18F47Q10                                                 */
/*                                                                            */
/* File name   : Main.c                                                       */
/* Description : Gestion de la carte centrale du circuit                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* IDE version : MPLAB X IDE v6.0                                             */
/* Compiler v. : CCS C Compiler v5.116                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Historique  :                                                              */
/*                                                                            */
/* <date>     <name>      <rev>   <description>                               */
/* 03.09.24    CHF         1.0    First version                               */
/*                                                                            */
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/*                          INCLUDES                                          */
/*----------------------------------------------------------------------------*/

#include "18F47Q10.h"


#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT, NOEXTOSC
//#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, NOCLKOUT
#use delay (clock=64000000)
//#use RS232(UART1, baud=9600, ERRORS, stream=STR1)
//#use RS232(UART1, baud=9600)
//#use rs232(baud=9600, xmit=PIN_B2, rcv=PIN_B1, stream=MUSIC)
#include "ComDisplay.c"
#include "MiniLCD_SPI_2x16v2.c"
#include "DFPlayer.c"
//#include "ComINF.c"
//#include "ComDisplay.c"

/*----------------------------------------------------------------------------*/
/*                          LOCAL VARIABLES                                   */
/*----------------------------------------------------------------------------*/
int Tab[10] = {1,3,5,7,10,15,12,1,8,9};
int i, numTrack=0;
int IndexMin;
int IndexMax;
int TabMin = Tab[0];
int TabMax = Tab[0];
int temp =0, counter = 0;

/*----------------------------------------------------------------------------*/
/*                          FUNCTIONS                                         */
/*----------------------------------------------------------------------------*/
//unsigned int8 send_buf[9];
int8 state=0;

void init();

int16 time = 0, timeold = 0;


#INT_TIMER0
void Timer0_ISR()
{
    set_timer0(25536);
    output_toggle(PIN_A4);
    time++;
    
}

void main(){ 
    init();
    while(1)
    {
        //output_toggle(PIN_D1);
        if(!input(PIN_B4))
        {
            //volume_max(0x06,0);
            DFPlayer_PlaySongNb(numTrack);
            delay_ms(1000);
            numTrack++;
            //play_1(0x01,0);
        }
        if(!input(PIN_C5))
        {
            if(state == 0)
            {
                output_toggle(PIN_A6);
                //play_1(0x05,0);
                //DFPlayer_NextSong();
                //DFPlayer_Pause();
                //delay_ms(200);
                //ComINF_Test();
                //delay_ms(500);
                ComDisplay_Mode(MODE_RUNNING_TIME);
                ComDisplay_Color(COLOR_GREEN);
                delay_ms(500);
                //ComINF_Message(1,3,9);
                state =1;
            }
            else if (state == 1)
            {
                output_toggle(PIN_A6);
                //DFPlayer_PlayBack();
                //ComINF_Message1();
                ComDisplay_Mode(MODE_NET_TIME);
                ComDisplay_Color(COLOR_RED);
                state = 0;
            }
        }
        if((time != timeold) &&((time%10)== 0))
        {
           
                timeold= time;
                ComDisplay_Time(time/100, time%100);
            
        }
        //delay_ms(200);
        
        
        output_toggle(PIN_A5);
    }
   
}
void init(){
   setup_timer_0(T0_INTERNAL|T0_DIV_4);
   enable_interrupts(INT_TIMER0);
   enable_interrupts(GLOBAL);
   DFPlayer_Init();
   ComDisplay_Mode(MODE_RUNNING_TIME);
   
}
