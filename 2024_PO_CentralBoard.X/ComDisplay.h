/* 
 * File:   ComDisplay.h
 * Author: choffre
 *
 * Created on 13. septembre 2024, 14:02
 */

#ifndef COMDISPLAY_H
#define	COMDISPLAY_H


#use rs232(UART2, baud=9600, xmit=PIN_D0, rcv=PIN_B0, stream=INF)



#define MODE_RUNNING_TIME   1
#define MODE_NET_TIME       0

#define COLOR_RED           0
#define COLOR_ORANGE        1
#define COLOR_GREEN         2
#define COLOR_BLUE          3


void ComDisplay_Time(int secondes, int centiemes);
void ComDisplay_Mode(int mode);
void ComDisplay_Color(int color);



#endif	/* COMDISPLAY_H */

