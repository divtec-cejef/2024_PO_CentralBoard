/*----------------------------------------------------------------------------*/
/* Project     : programmeCircuit                                             */
/* Author      : Cristhian Valencia                                           */
/* Target      : PIC 18F47Q10                                                 */
/*----------------------------------------------------------------------------*/

#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT
#use delay (clock=64000000)

#include "MiniLCD_SPI_2x16v2.c"
#include "DFPlayer.c"
#include "ComDisplay.c"
//==============================================================================
//définition des entrées
//==============================================================================

#define BUZZER      PIN_B4

//==============================================================================
//définition des sorties
//==============================================================================

#define LEDD_1      PIN_A4
#define LEDD_2      PIN_A5
#define LEDD_3      PIN_A6

//==============================================================================
//définition des états
//==============================================================================

#define STARTING    1
#define WAIT        2
#define READY       3
#define COUNTDOWN   4
#define LAUNCHING   5
#define RACE        6
#define TIME        7
#define ENDING      8

//==============================================================================
//Interruption timer
//==============================================================================

int16 count = 0;

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
    
    //== Pulsations ==//
    int8 pulse = 0;
    int8 final = 0;
    int8 fastE = 0; // fast elevator //
    int8 slowE = 0;
    
    //== Flancs descendants ==//                                                         
    int8 prBzr = 0;     // Pour faire l'antirebond du Buzzer //
     
    //== Initialisation ==//   
    state = STARTING;   
    
    //== Boucle infinie ==//   
    while(true)
    {
        
        //== Lecture des entrées ==//        
        buzzr = input(BUZZER);
        
//==============================================================================        
//Lecture des états 
//==============================================================================
        
        switch (state)
        {
            case STARTING:
                
                timer = 0;
                
                lled1 = 1;
                lled2 = 0;
                lled3 = 1;
                
                //KARTING MUSIC
                
                state = WAIT;
                
            break;

//============================================================================//
                
            case WAIT:
                
                if(buzzr == 1 && prBuz == 0 /*&&BONUS RECEIVED*/)
                {                                        
                    lled1 = 0;  // Éteindre toutes les LEDs //
                    lled2 = 0;
                    lled3 = 0;
                    
                    count = 0;  // Arrêter compteur //
                    
                    //STOP KARTING MUSIC
                    
                    //CAR ENGINE SOUND EFFECT & PRE-START MUSIC
                    
                    state = READY;
                    
                }
                
                else
                {
                    if(count == 200)
                    {
                        lled1 = 0;
                        lled2 = 1;
                        lled3 = 0;
                    }
                    
                    if(count == 400)
                    {
                        lled1 = 1;
                        lled2 = 0;
                        lled3 = 1;
                        count = 0;
                    }
                                        
                }
                
            break;
            
//============================================================================//                
          
            case READY:
                
                if(buzzr == 0 && prBuz == 1)
                {
                    //SEND READY MESSAGE
                    
                    //STOP PRE-START MUSIC
                    //DFPlayer_PlaySongNb(1);
                    
                    //CAR ENGINE SOUND EFFECT 
                    
                    //COUNTDOWN SOUND EFFECT
                                        
                    lled1 = 1;  // RED //
                    
                    state = COUNTDOWN;                    
                }
                
            break;
            
//============================================================================//                
            
            case COUNTDOWN:
                
                if(buzzr == 0)
                {
                    if(count == 100)
                    {
                        lled1 = 1;  // RED //
                        lled2 = 1;  // RED //
                    }
                    
                    if(count == 200)
                    {
                        lled1 = 1;  // RED //
                        lled2 = 1;  // RED //
                        lled3 = 1;  // RED //
                    }
                    
                    if(count == 300)    
                    {
                        lled1 = 1;  // RED //
                        lled2 = 1;  // RED //
                        lled3 = 1;  // RED //
                        PIN_D1 = 1; // EXTRA LED - RED //
                    }
                    
                    if(count == 400)
                    {
                        lled1 = 1;  // GREEN // 
                        lled2 = 1;  // GREEN //
                        lled3 = 1;  // GREEN //
                        PIN_D1 = 1; // EXTRA LED - GREEN //
                        
                        //START CHRONOMETER
                        //START RACE MUSIC
                        //SHOW RUNNING TIME
                        
                        count = 0;
                        state = LAUNCHING;                       
                    } 
                    count++;
                }
                
                if(buzzr == 1)
                {
                    //SEND ERROR MESSAGE
                    //FART SOUND EFFECT
                    //STOP COUNTDOWN SOUND EFFECT
                    //START CHRONOMETER
                    //ADD 2 SECONDS TO THE CHRONOMETER
                    //START RACE MUSIC
                    
                    lled1 = 1;  // GREEN // 
                    lled2 = 1;  // GREEN //
                    lled3 = 1;  // GREEN //
                    PIN_D1 = 1; // EXTRA LED - GREEN //
                    
                    count = 0;
                    
                    state = RACE;
                }
                
                
            break;
            
//============================================================================//                  
            
            case LAUNCHING:
                
                if(buzzr == 1)
                {
                    //SAVE REACTION TIME
                    //SEND REACTION TIME
                    
                    state = RACE;
                }
                
                //CHRONOMETER UPDATE
                
            break;
            
//============================================================================//    
         
            case RACE:
                
                if(pulse == 1)
                {
                    state = TIME;
                }
                
                if(fastE == 1)
                {
                    //Activer ascenseur rapide
                }
                
                if(slowE == 1)
                {
                    //Activer ascenseur lent
                }
                
                if(final == 1)
                {
                    //stop time live
                    //show total time
                    //stop race music
                    //goal reached music
                    //start ending music 
                    state = ENDING;
                }                    
                
                
                //CHRONOMETER UPDATE
                
            break;
            
//============================================================================//                
            
            case TIME:
                
                //HIDE RUNNING TIME
                //SHOW INTERMEDIATE TIME
                count++;
                
                if(count == 200)
                {
                    //HIDE INTERMEDIATE TIME
                    //SHOW RUNNING TIME
                    
                    count = 0;
                    
                    state = RACE;
                }
                
                //CHRONOMETER UPDATE
                
            break;
            
//============================================================================//                
            
            case ENDING:
                
                if(buzzr == 1)
                {
                    state = STARTING;
                }
                
            break;
            
//============================================================================//
//Fin lecture des états
//============================================================================//               
        }
        
    //== Flancs descendants ==//           
    prBzr = buzzr;
    
    //== Écriture des sorties ==//               
    output_bit(LEDD_1, lled1);
    output_bit(LEDD_2, lled2);
    output_bit(LEDD_3, lled3);
    
    }   //== fin boucle infinie ==//      
   
}