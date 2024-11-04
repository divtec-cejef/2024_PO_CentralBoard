#include "ComFeux.h"

void ComFeuAllume(int f1, int f2, int f3, int f4, char clr, int lum)
{
    printf("%d%d%d%d%c%d",f1,f2,f3,f4,clr,lum);
}
    
//animation leds
void ComFeuAnim(int lumi)
{
    if(lumi == 1)
    {
        printf("anim13");
    }
    
    else
    {
        printf("000000");
    }
}