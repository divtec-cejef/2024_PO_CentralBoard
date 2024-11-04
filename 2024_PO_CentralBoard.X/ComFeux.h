#ifndef COMFEUX_H
#define	COMFEUX_H
//#use rs232(UART2, baud=9600, xmit=PIN_B4, rcv=PIN_C6, stream=FEUX)
#define ETEINT  0
#define ALLUME  1
#define RED     5
#define GREEN   6
void ComFeuAllume(int f1, int f2, int f3, int f4, int clr, int lum);
void ComFeuAnim(int lumi);
#endif	/* COMFEUX_H */