/*----------------------------------------------------------------------------*/
/* Project     : testPO                                                       */
/* Author      : Cristhian Valencia                                           */
/* Target      : PIC 18F47Q10                                                 */
/*----------------------------------------------------------------------------*/

#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT
#use delay (clock=64000000)

#include "MiniLCD_SPI_2x16v2.c"

#define BUZZER      PIN_B4

//==============================================================================
//Interruption timer
//==============================================================================

int count = 0;

#INT_TIMER0
void Timer0_ISR()
{
    set_timer0(25536);
    count++;
}

//==============================================================================
//Programme principal
//==============================================================================

void main()
{
    int8 secon=0;
    int8 dseco=0;
    int8 buzzr=0;
    int8 prBuz=0;
    int8 x=0;
    
    //== Initialisation affichage & timer ==//      
    setup_timer_0(T0_INTERNAL|T0_DIV_4);  //fosc/4, divisé par 4
    set_timer0(25536);
    enable_interrupts(INT_TIMER0);                                              
    enable_interrupts(GLOBAL);
    lcd_init();
 
    //== Boucle infinie ==//        
    while(TRUE)
    {
        
        buzzr = input(BUZZER);
    
          
        if(buzzr == 1 && prBuz == 0)
        {
            x++;
        }
        /*else
        {
            output_bit(PIN_A4,0);
        }*/
        if(x == 1)
        {
            if(count>=10)
            {
                count=0;
                dseco++;

                if(dseco>=10)   
                {
                    dseco=0;
                    secon++;
                } 

                lcd_gotoxy(1,2);
                printf(lcd_putc," %d, %d ",secon ,dseco);         
            }
        }
        
        delay_ms(10);
    }    
}