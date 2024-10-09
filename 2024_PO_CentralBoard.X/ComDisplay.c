#include "ComDisplay.h"


/*
#define MODE_RUNNING_TIME   1
#define MODE_NET_TIME       0

#define COLOR_RED           0
#define COLOR_ORANGE        1
#define COLOR_GREEN         2
#define COLOR_BLUE          3
*/

//Send message to INF
void ComDisplay_Time(int secondes, int centiemes)
{
    //output_toggle(PIN_A6);
    //format message "m:sscc"    
    //m : numéro de message
    //ss: secondes 0-99
    //cc: centièmes 0-99
    
    //Sécurité pour éviter erreur
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
            
          
    }
    //output_toggle(PIN_A6);
}

