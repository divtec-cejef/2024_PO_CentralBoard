#include "DFPlayer.h"

/*
!!! Attention l'ordre des chansons est donné par l'ordre dans lequel vous les copiez !!!
*/

void DFPlayer_Init()
{   
    DFPlayer_SetVolume(20);                                                     // Set volume max //
    DFPlayer_PlaySongNb(1);                                                     // Plays first song //
}

void DFPlayer_NextSong()
{
   send_buf[0] = 0x7E;                                                          // Start byte //
   send_buf[1] = 0xFF;                                                          // Version //
   send_buf[2] = 0x06;                                                          // Len //
   send_buf[3] = 0x01;                                                          // Command 0X01 Next //
   send_buf[4] = 0x00;                                                          // Feedback, always 0 => 0 //
   send_buf[5] = 0x00;                                                          // High byte (DH), para 1 //
   send_buf[6] = 0x00;                                                          // Low byte (DL), para 2 //
//!send_buf[7] = 0xFF;                                                          // checksum highbyte //
//!send_buf[8] = 0xE6;                                                          // checksum low byte //
   send_buf[7] = 0xEF;                                                          // end //
   
   DFPlayer_SendData();
}

//== SET VOLUME FROM 0 TO 30 ==//
void DFPlayer_SetVolume(int volume)
{
   unsigned int i;
   send_buf[0] = 0x7E;                                                          // Start byte //
   send_buf[1] = 0xFF;                                                          // Version //
   send_buf[2] = 0x06;                                                          // Len //
   send_buf[3] = 0x06;                                                          // Command 0X01 Next //
   send_buf[4] = 0x00;                                                          // Feedback, always 0 => 0 //
   send_buf[5] = 0x00;                                                          // High byte (DH), para 1 //
   send_buf[6] = volume;                                                        // Low byte (DL), para 2 //
//!send_buf[7] = 0xFF;                                                          // checksum highbyte //
//!send_buf[8] = 0xE6;                                                          // checksum low byte //
   send_buf[7] = 0xEF;                                                          // end //
   
   DFPlayer_SendData();
}

void DFPlayer_Pause()
{
   unsigned int i;
   send_buf[0] = 0x7E;                                                          // Start byte //
   send_buf[1] = 0xFF;                                                          // Version //
   send_buf[2] = 0x06;                                                          // Len //
   send_buf[3] = 0x0E;                                                          // Command, 0x0E Pause //
   send_buf[4] = 0x00;                                                          // Feedback, always 0 => 0 //
   send_buf[5] = 0x00;                                                          // High byte (DH), para 1 //
   send_buf[6] = 0x00;                                                          // Low byte (DL), para 2 //
//!send_buf[7] = 0xFF;                                                          // checksum highbyte //
//!send_buf[8] = 0xE6;                                                          // checksum low byte //
   send_buf[7] = 0xEF;                                                          // end //
   
   DFPlayer_SendData();
}

void DFPlayer_PlayBack()
{
   unsigned int i;
   send_buf[0] = 0x7E;                                                          // Start byte //
   send_buf[1] = 0xFF;                                                          // Version //
   send_buf[2] = 0x06;                                                          // Len //
   send_buf[3] = 0x0D;                                                          // Command, 0x0E Playback //
   send_buf[4] = 0x00;                                                          // Feedback, always 0 => 0 //
   send_buf[5] = 0x00;                                                          // High byte (DH), para 1 //
   send_buf[6] = 0x00;                                                          // Low byte (DL), para 2 //
//!send_buf[7] = 0xFF;                                                          // checksum highbyte //
//!send_buf[8] = 0xE6;                                                          // checksum low byte //
   send_buf[7] = 0xEF;                                                          // end //
   
   DFPlayer_SendData();
}

//Numéro de chanson
//Attention l'ordre des chansons est donné par l'ordre dans lequel vous les 
//copiez
void DFPlayer_PlaySongNb(int numSong)
{
   unsigned int i;
   send_buf[0] = 0x7E;                                                          // Start byte //
   send_buf[1] = 0xFF;                                                          // Version //
   send_buf[2] = 0x06;                                                          // Len //
   send_buf[3] = 0x03;                                                          // Command, see 3.2 //
   send_buf[4] = 0x00;                                                          // Feedback, always 0 => 0 //
   send_buf[5] = 0x00;                                                          // High byte (DH), para 1 //
   send_buf[6] = numSong;                                                       // Low byte (DL), para 2 //
//!send_buf[7] = 0xFF;                                                          // checksum highbyte //
//!send_buf[8] = 0xE6;                                                          // checksum low byte //
   send_buf[7] = 0xEF;                                                          // end //
   
   DFPlayer_SendData();
}

void DFPlayer_SendData()
{
    unsigned int i;
    for(i = 0; i < 8; i ++)
    {    
      printf("%c",send_buf[i]);
    }
}