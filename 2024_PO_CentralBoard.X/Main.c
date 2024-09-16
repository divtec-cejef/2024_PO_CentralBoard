/*----------------------------------------------------------------------------*/
/* Project     : programmeCircuit                                             */
/* Author      : Cristhian Valencia                                           */
/* Target      : PIC 18F47Q10                                                 */
/*----------------------------------------------------------------------------*/

#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT
#use delay (clock=64000000)

#include "MiniLCD_SPI_2x16v2.c"

//==============================================================================
//définition des entrées
//==============================================================================

#define BUZZER  PIN_B4 

//==============================================================================
//définition des sorties
//==============================================================================

#define LEDD_1  PIN_A4
#define LEDD_2  PIN_A5
#define LEDD_3  PIN_A6

//==============================================================================
//définition des états
//==============================================================================

#define STARTING    1
#define WAIT        2
#define READY       3
#define COUNTDOWN   4
#define RUN         5
#define TIME        6
#define ENDING      7

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

    //== Déclaration des variables ==//                                               
    int8 state = 0;     // État //     
    int8 timer = 0;     // Chrono // 
    int8 blink = 0;     // Animation - leds clignotent - O X O - X O X //
    int8 secon = 0;     // Secondes //
    int8 dseco = 0;     // Dixièmes de secondes //
    
    //== Entrées ==//                            
    int8 buzzr = 0;     // Bouton buzzer principal //
    
    //== Sorties ==//
    int8 lled1 = 0;     // 3 Feux //
    int8 lled2 = 0;
    int8 lled3 = 0;
    
    //== Flancs descendants ==//                                                         
    int8 prBzr = 0;     // Pour faire l'antirebond du Buzzer //
     
    //== Initialisation ==//   
    state = STARTING;   
    
    //== Boucle infinie ==//   
    while(true)
    {
        
        //== Lecture des entrées ==//        
        buzzr = input(BUZZER);
        
        //== Lecture des états ==//
        switch (state)
        {
            case STARTING:
                
                break;

//============================================================================//
                
            case WAIT:
                
                break;
            
//============================================================================//                
          
            case READY:
                
                break;
            
//============================================================================//                
            
            case COUNTDOWN:
                
                break;
            
//============================================================================//                
         
            case RUN:
                
                break;
            
//============================================================================//                
            
            case TIME:
                
                break;
            
//============================================================================//                
            
            case ENDING:
                
                break;
            
//============================================================================//
            
            
        }
        
    }        
                                                            
    //== Flancs descendants ==//           
     
    prBzr = buzzr;
    
        //== Écriture des sorties ==//               
        output_bit(LEDD_1, lled1);
        output_bit(LEDD_2, lled2);
        output_bit(LEDD_3, lled3);
        
}