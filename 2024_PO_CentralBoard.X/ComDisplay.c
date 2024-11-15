#include "ComDisplay.h"

//== SET TIME ==// 
void ComDisplay_Time(int secondes, int centiemes)
{
    if(secondes > 99)                                                        
    {
        secondes = 99;
    }
    
    if (centiemes >99)
    {
        centiemes = 99;
    }
    
    printf("%02d%02d",secondes,centiemes);
}

//== SHOW THE LAST DIGIT ==//
void ComDisplay_Mode(int mode)
{
    if(mode == MODE_RUNNING_TIME)
    {
        printf("run1");
    }
    else if(mode == MODE_NET_TIME)
    {
        printf("run2");
    }   
}

 //== CHANGE COLOR ==//
void ComDisplay_Color(int color)
{
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
}