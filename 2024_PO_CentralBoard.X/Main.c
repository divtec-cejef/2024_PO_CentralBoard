/******************************************************************************/
/*                                                                            */
/*                                CEJEF                                       */
/*                          Division Technique                                */
/*                            EMT Porrentruy                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Project     : lecteur MP3                                                  */
/* Author      : Pena                                         */
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
#use rs232(baud=9600, xmit=PIN_B2, rcv=PIN_B1, stream=MUSIC)

#include "MiniLCD_SPI_2x16v2.c"
#include "DFPlayer.c"

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
/*
void play_1(unsigned int8 cmd, unsigned int8 feedback);
void volume_max(unsigned int8 cmd, unsigned int8 feedback);
void DFPlayer_NextSong();
void DFPlayer_PlaySongNb(int numSong);
void DFPlayer_Pause();
void DFPlayer_SetVolume(int volume);
void DFPlayer_PlayBack();*/
void init();

#INT_TIMER0
void Timer0_ISR()
{
    //set_timer0(TIMER0_2S);
    output_toggle(PIN_A4);
    
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
                DFPlayer_Pause();
                
                state =1;
            }
            else if (state == 1)
            {
                output_toggle(PIN_A6);
                DFPlayer_PlayBack();
                state = 0;
            }
        }
        delay_ms(200);
        
        output_toggle(PIN_A5);
    }
   
}
void init(){
   //setup_timer_0(T0_INTERNAL);
   //enable_interrupts(INT_TIMER0);
   //enable_interrupts(GLOBAL);
    DFPlayer_SetVolume(10);
   
}
/*
void volume_max(unsigned int8 cmd, unsigned int8 feedback){
   unsigned int8 i;
   send_buf[0] = 0x7E;           // Start byte
   send_buf[1] = 0xFF;           // Version
   send_buf[2] = 0x06;           // Len
   send_buf[3] = cmd;            // Command, see 3.2
   send_buf[4] = feedback;       // Feedback, always 0 => 0
   send_buf[5] = 0x00;           // High byte (DH), para 1
   send_buf[6] = 0x1E;           // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xD5;           // checksum low byte
   send_buf[7] = 0xEF;           // end
   send_buf[8] = '\0';
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
   };
}

void play_1(unsigned int8 cmd, unsigned int8 feedback){
   unsigned int8 i;
   send_buf[0] = 0x7E;           // Start byte
   send_buf[1] = 0xFF;           // Version
   send_buf[2] = 0x06;           // Len
   send_buf[3] = cmd;            // Command, see 3.2
   send_buf[4] = feedback;       // Feedback, always 0 => 0
   send_buf[5] = 0x00;           // High byte (DH), para 1
   send_buf[6] = 0x01;           // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xE6;           // checksum low byte
   send_buf[7] = 0xEF;           // end
   
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
      //putc(send_buf[i]);
   };
   
  
   //puts(send_buf);
   
}

//Attention l'ordre des chansons est donné par l'ordre dans lequel vous les 
//copiez


void DFPlayer_NextSong()
{
   unsigned int8 i;
   send_buf[0] = 0x7E;              // Start byte
   send_buf[1] = 0xFF;              // Version
   send_buf[2] = 0x06;              // Len
   send_buf[3] = 0x01;              // Command 0X01 Next
   send_buf[4] = 0x00;              // Feedback, always 0 => 0
   send_buf[5] = 0x00;              // High byte (DH), para 1
   send_buf[6] = 0x00;              // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xE6;           // checksum low byte
   send_buf[7] = 0xEF;              // end
   
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
      //putc(send_buf[i]);
   }; 
}

void DFPlayer_SetVolume(int volume)
{
   unsigned int8 i;
   send_buf[0] = 0x7E;              // Start byte
   send_buf[1] = 0xFF;              // Version
   send_buf[2] = 0x06;              // Len
   send_buf[3] = 0x06;              // Command 0X01 Next
   send_buf[4] = 0x00;              // Feedback, always 0 => 0
   send_buf[5] = 0x00;              // High byte (DH), para 1
   send_buf[6] = volume;              // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xE6;           // checksum low byte
   send_buf[7] = 0xEF;              // end
   
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
      //putc(send_buf[i]);
   }; 
}
void DFPlayer_Pause()
{
   unsigned int8 i;
   send_buf[0] = 0x7E;              // Start byte
   send_buf[1] = 0xFF;              // Version
   send_buf[2] = 0x06;              // Len
   send_buf[3] = 0x0E;              // Command, 0x0E Pause
   send_buf[4] = 0x00;              // Feedback, always 0 => 0
   send_buf[5] = 0x00;              // High byte (DH), para 1
   send_buf[6] = 0x00;              // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xE6;           // checksum low byte
   send_buf[7] = 0xEF;              // end
   
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
      //putc(send_buf[i]);
   };
}
void DFPlayer_PlayBack()
{
   unsigned int8 i;
   send_buf[0] = 0x7E;              // Start byte
   send_buf[1] = 0xFF;              // Version
   send_buf[2] = 0x06;              // Len
   send_buf[3] = 0x0D;              // Command, 0x0E Playback
   send_buf[4] = 0x00;              // Feedback, always 0 => 0
   send_buf[5] = 0x00;              // High byte (DH), para 1
   send_buf[6] = 0x00;              // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xE6;           // checksum low byte
   send_buf[7] = 0xEF;              // end
   
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
      //putc(send_buf[i]);
   };
}

void DFPlayer_PlaySongNb(int numSong)
{
   unsigned int8 i;
   send_buf[0] = 0x7E;           // Start byte
   send_buf[1] = 0xFF;           // Version
   send_buf[2] = 0x06;           // Len
   send_buf[3] = 0x03;            // Command, see 3.2
   send_buf[4] = 0x00;       // Feedback, always 0 => 0
   send_buf[5] = 0x00;           // High byte (DH), para 1
   send_buf[6] = numSong;           // Low byte (DL), para 2
//!   send_buf[7] = 0xFF;           // checksum highbyte
//!   send_buf[8] = 0xE6;           // checksum low byte
   send_buf[7] = 0xEF;           // end
   
   for(i = 0; i < 8; i ++){
      printf("%c",send_buf[i]);
      //putc(send_buf[i]);
   };
    
}
*/