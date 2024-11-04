#include "ComDisplay.h"

//Send message to INF
void ComDisplay_Time(int secondes, int centiemes)
{
    //output_toggle(PIN_A6);
    //format message "m:sscc"    
    //m : num�ro de message
    //ss: secondes 0-99
    //cc: centi�mes 0-99    
    //S�curit� pour �viter erreur
    if(secondes > 99)
    {
        secondes = 99;
    }
    if (centiemes >99)
    {
        centiemes = 99;
    }
    printf("%02d%02d",secondes,centiemes);
    //printf("run2");
    //printf("1234");
    //printf("clrg");
    //output_toggle(PIN_A6);
}
void ComDisplay_Mode(int mode)
{
    //output_toggle(PIN_A6);
    if(mode == MODE_RUNNING_TIME)
    {
        printf("run1");
    }
    else if(mode == MODE_NET_TIME)
    {
        printf("run2");
    }   
    //output_toggle(PIN_A6);
}
void ComDisplay_Color(int color)
{
    //output_toggle(PIN_A6);

    switch (color)
    {
        case COLOR_RED:
            printf("clrr");
            break;
        case COLOR_GREEN:
            printf("clrg");
            break;
        case COLOR_ORANGE:
            printf("clro");
            break;
        case COLOR_BLUE:
            printf("clrb");
            break;
        default:
            printf("clrg");                   
    }//output_toggle(PIN_A6);
}