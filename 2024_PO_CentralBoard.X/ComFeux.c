#include "ComFeux.h"


void ComFeuAllume(int f1, int f2, int f3, int f4, int clr, int lum)
{
    printf("%d%d%d%d%d%d",f1,f2,f3,f4,clr,lum);
}
    
void ComFeuAnim(int state)
{
    if(state == 1)
    {
        printf("0,0,0,0,0,0");
    }
      else
    {
        printf("anim13");
    }
        
}
        
        
    

