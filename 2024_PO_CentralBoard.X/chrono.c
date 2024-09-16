#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT
#use delay (clock=64000000)

#include "ComDisplay.c"

int j = 0;

//==============================================================================
//Interruption timer
//==============================================================================

#INT_TIMER0
void Timer0_ISR()
{
    set_timer0(25536);
    output_toggle(PIN_A4);
    j++;
}

//==============================================================================
//Programme principal
//==============================================================================

void main()
{
    int8 x=0;
    int8 y=0;
 
    //== Initialisation affichage & timer ==//      
    setup_timer_0(T0_INTERNAL|T0_DIV_4);  //fosc/4, divisé par 4
    set_timer0(25536);
    enable_interrupts(INT_TIMER0);                                              
    enable_interrupts(GLOBAL);
    ComDisplay_Color(COLOR_GREEN);
    ComDisplay_Mode(MODE_RUNNING_TIME);
   
    //== Boucle infinie ==//        
    while(TRUE)
    {
        
        if(j>=10)
        {
            j=0;
            y++;
            output_toggle(PIN_A5);
            
            if(y>=10)
            {
                y=0;
                x++;
                ComDisplay_Time(x,x/10);
            } 
                        
        }            
    }    
}
