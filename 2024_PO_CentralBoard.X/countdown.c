#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT
#use delay (clock=64000000)

#include "MiniLCD_SPI_2x16v2.c"

int16 count = 0;

//==============================================================================
//Interruption timer
//==============================================================================

#INT_TIMER0
void Timer0_ISR()
{
    set_timer0(25536);   
    count++;    
}

#define LED_1       PIN_A6
#define LED_2       PIN_A5
#define LED_3       PIN_A4
#define LED_X       PIN_D1

#define BUZZER      PIN_B4

#define READY       1
#define COUNTDOWN   2
#define FALSE       3
#define REAL        4

void main()
{
    int8 led1 = 0;
    int8 led2 = 0;
    int8 led3 = 0;
    int8 ledX = 0;
       
    int8 state = 0;
    
    int8 prBuz = 0;
    int8 buzzr = 0;

    state = READY;
    
    setup_timer_0(T0_INTERNAL|T0_DIV_4);  //fosc/4, divisé par 4
    set_timer0(25536);
    enable_interrupts(INT_TIMER0);                                              
    enable_interrupts(GLOBAL);
    
    while(TRUE)
    {       
        
        buzzr = input(BUZZER);
    
        switch (state)
        {          
//============================================================================//               
            
            case READY:

                if(buzzr == 0 && prBuz == 1)
                {
                    state = COUNTDOWN;
                }
                
            break;
//============================================================================//          
            case COUNTDOWN:
            
                                                                            
                    if(count == 100)
                    {
                        led1 = 1;  // RED //
                    }
                    
                    if(count == 200)
                    {
                        led1 = 1;  // RED //
                        led2 = 1;  // RED //
                    }
                    
                    if(count == 300)    
                    {
                        led1 = 1;  // RED //
                        led2 = 1;  // RED //
                        led3 = 1;  // RED //
                    }
                    
                    if(count == 400)
                    {
                        led1 = 1;   // RED // 
                        led2 = 1;   // RED //
                        led3 = 1;   // RED //
                        ledX = 1;   // EXTRA LED - RED //                                                            
                    }
                    
                    if(count == 500)
                    {
                        led1 = 1;   // GREEN // 
                        led2 = 1;   // GREEN //
                        led3 = 1;   // GREEN //
                        ledX = 1;   // EXTRA LED - GREEN //
                        count = 0;
                        state = REAL; 
                    }         
                        
                    if(buzzr == 0 && prBuz == 1)
                    {   
                        count = 0;

                        state = FALSE;
                    }                
                
            break;
//============================================================================//          
            case FALSE:

                led1 = 1;   // GREEN // 
                led2 = 1;   // GREEN //
                led3 = 1;   // GREEN //
                ledX = 1;

            break;
//============================================================================//      
            case REAL:

                ledX = 1;
             
                led1 = 0;  
                led2 = 0;   
                led3 = 0; 

            break;
            
            }
        
        ///////////////////////////////////
        
        prBuz = buzzr;        

        output_bit(LED_1, led1);
        output_bit(LED_2, led2);
        output_bit(LED_3, led3);  
        output_bit(LED_X, ledX);
    }
    
}