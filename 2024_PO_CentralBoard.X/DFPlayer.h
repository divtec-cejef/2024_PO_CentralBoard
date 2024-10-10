/* 
 * File:   DFPlayer.h
 * Author: choffre
 *
 * Created on 13. septembre 2024, 14:45
 */

#ifndef DFPLAYER_H
#define	DFPLAYER_H

#use rs232(UART2, baud=9600, xmit=PIN_B4, rcv=PIN_B2, stream=UART2)

//Attention l'ordre des chansons est donné par l'ordre dans lequel vous les 
//copiez
unsigned int send_buf[9];

void DFPlayer_SendData();
void DFPlayer_SetVolume(int volume);
void DFPlayer_NextSong();
void DFPlayer_Pause();
void DFPlayer_PlayBack();
void DFPlayer_PlaySongNb(int numSong);
void DFPlayer_Init();

#endif	/* DFPLAYER_H */

