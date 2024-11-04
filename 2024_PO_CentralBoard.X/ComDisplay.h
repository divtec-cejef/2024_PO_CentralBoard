#ifndef COMDISPLAY_H
#define	COMDISPLAY_H

//#use rs232(UART2, baud=9600, xmit=PIN_B4, rcv=PIN_B2, stream=DISPLAY)

#define MODE_RUNNING_TIME   1
#define MODE_NET_TIME       0

#define COLOR_RED           0
#define COLOR_ORANGE        1
#define COLOR_GREEN         2
#define COLOR_BLUE          3

void ComDisplay_Time(int secondes, int centiemes);
void ComDisplay_Mode(int mode);
void ComDisplay_Color(int color);

#endif	

/* COMDISPLAY_H */