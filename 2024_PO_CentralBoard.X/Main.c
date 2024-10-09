/*----------------------------------------------------------------------------*/
/* Project     : programmeCircuit                                             */
/* Author      : Cristhian Valencia                                           */
/* Target      : PIC 18F47Q10                                                 */
/*----------------------------------------------------------------------------*/

#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT
#use delay (clock=64000000)

//#include "DFPlayer.c"
#include "ComDisplay.c"
#include "ComINF.c"
//#include "ComFeux.c"

//==============================================================================
//DÉFINITION DES ENTRÉES
//==============================================================================

#define BUZZER_PIN  PIN_B4
#define SIGNAL_PIN  PIN_C3
#define FINAL_PIN   PIN_C5

#define MUSIK       PIN_D2

//==============================================================================
//DÉFINITION DES SORTIES
//==============================================================================
                                                                                // Seront remplacés par les feux //
#define LED_1       PIN_A6                                                      
#define LED_2       PIN_A5
#define LED_3       PIN_A4
#define LED_X       PIN_D1

//==============================================================================
//DÉFINITION DES ÉTATS
//==============================================================================

#define STARTING    1
#define WAIT        2
#define READY       3
#define COUNTDOWN   4
#define LAUNCHING   5
#define RACE        6
#define TIME        7
#define ENDING      8

#define DELAY_FALSE_START 200

#define DEBUG_COM 

//==============================================================================
//VARIABLES GLOBALES
//==============================================================================

int8 secondaryCounterActivator = 0;                                             // Activateur compteur pour opérations non-liées au chronomètre //
int8 counterActivator = 0;                                                      // Activateur compteur du chronomètre //
int8 toDisplay = 0;                                                             // Sert à afficher le temps sans les centièmes //

int16 tertiaryCounter = 0;
int16 secondaryCounter = 0;                                                     // Compteur secondaire //
int16 counter = 0;                                                              // Compteur principal pour le chronomètre //

//==============================================================================
//INTÉRRUPTION COMPTEURS 
//==============================================================================

#INT_TIMER0
void Timer0_ISR()
{
    set_timer0(25536);                                                          // 10 millisecondes //
    
    if(counterActivator == 1)
    {
        counter++;
        
        if(counter % 10 == 0)
        {
            toDisplay = 1;  
        }
    }
    
    if(secondaryCounterActivator == 1)
    {
        secondaryCounter++;
    }
    
    tertiaryCounter++;
    if(tertiaryCounter == 18)
        {
            output_bit(PIN_C2,1);
        }
        if(tertiaryCounter == 36)
        {
            output_bit(PIN_C2,0);
            tertiaryCounter = 0;
        }
}


//==============================================================================
//PROGRAMME PRINCIPAL
//==============================================================================

void main()
{
    //== DÉCLARATION DES VARIABLES ==//                                                 
    int16 stopTime = 0;                                                         // Sauvegarde temps tournant //
    int16 finalTime = 0;                                                        // Sauvegarde temps final //
    int16 reactionTime = 0;                                                     // Sauvegarde temps de réaction //
    
    int8 state = 0;                                                             // État // 
    
    int8 zero = 0;                                                              // Reset de l'affichage du chrono //
    
    int8 cell = 3;                                                              // Sert à bien numéroter les messages //
    
    //== ENTRÉES ==//                            
    int8 buzzer = 0;                                                            // Bouton buzzer principal //

    //== SORTIES ==//
    int8 led1 = 0;                                                              // 4 Feux //
    int8 led2 = 0;
    int8 led3 = 0;
    int8 ledX = 0;

    //== SIGNAUX CELLULES ==//
    int8 interSignal = 0;                                                       // Cellule temps intermédiaire (simulation bouton S2)//
    int8 finalSignal = 0;                                                       // Cellule finale (simulation bouton extra)//
//    int8 fastElevator = 0; 
//    int8 slowElevator = 0;

    //== FLANCS DESCENDANTS ==//                         
    int8 prevBuzzer = 0;                                                        // Antirebond Buzzer //
    int8 prevSignal = 0;                                                        // Antirebond Bouton S2 //
    int8 prevFinal = 0;                                                         // Antirebond Bouton extra //
    
    //== INITIALISATION TIMER ==//      
    setup_timer_0(T0_INTERNAL|T0_DIV_4);                                        // Fosc/4, divisé par 4 //
    set_timer0(25536);
    enable_interrupts(INT_TIMER0);                                              
    enable_interrupts(GLOBAL);
    ComDisplay_Color(COLOR_RED);
    ComDisplay_Mode(MODE_RUNNING_TIME); 
    
    //== INITIALISATION MUSIQUE ==//
    //DFPlayer_Init();            
    //DFPlayer_PlaySongNb(1);
    
    
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    
    //== INITIALISATION PROGRAMME PRINCIPAL ==//
    state = STARTING;

    //== DÉBUT BOUCLE INFINIE ==//
    while(TRUE)
    { 
        //== LECTURE DES ENTRÉES ==//        
        buzzer = input(BUZZER_PIN);
        finalSignal = input (FINAL_PIN);
        interSignal = input(SIGNAL_PIN);
        
        output_bit(PIN_C1,1);

//==============================================================================        
//LECTURE DES ÉTATS
//==============================================================================
        
        switch (state)
        {                                                   
            case STARTING:                                                      // Reset de toutes les valeurs //
                
                //DFPlayer_PlaySongNb(1);
                
                counterActivator = 0;
                secondaryCounterActivator = 0;
                secondaryCounter = 0;
                counter = 0;
                
                cell = 3;
                
                byteNumber = 0;
                
                bonus = 0;
                
                bonusBlock = 0;
                
                stopTime = 0;
                finalTime = 0;
                reactionTime = 0;
                
                ComDisplay_Time(zero, zero);
                ComDisplay_Color(COLOR_RED);
                ComDisplay_Mode(MODE_RUNNING_TIME);
                
                #ifndef DEBUG_COM
                ComFeuAllume(0,0,0,0,5,3);
                
                ComFeuAnim(1,3);                                                // Active animation Feux //
                #endif
                
                led1 = 1;
                led2 = 0;
                led3 = 1;
                ledX = 0;
                
                delay_ms(1000);                                                 // Délai pour éviter perturbations //
                
                state = WAIT;
 
            break;

//============================================================================//
                
            case WAIT:                                                          // État de stand-by //
                
                secondaryCounterActivator = 1;
                
                if(buzzer == 0 && prevBuzzer == 1 && bonus == 1)
                {      
                    bonusBlock = 1; 
                    
                    led1 = 0;                                                   // Éteindre les feux //
                    led2 = 0;
                    led3 = 0;
                    ledX = 0;
                    
                    #ifndef DEBUG_COM
                    ComFeuAnim(0);
                    #endif
                    
                    #ifndef DEBUG_COM
                    ComINF_MessageInfo(1111, 1, 6969);                          // Envoi message prêt //
                    #endif
                    
                    secondaryCounter = 0;                                       
                    secondaryCounterActivator = 0;                              // Reset compteur secondaire pour commencer à 0 //                    
                    
                    //DFPlayer_NextSong(); //2
                    
                    delay_ms(1000);                                             // Délai pour éviter perturbations //
                    
                    state = READY;
                }
                
                if(secondaryCounter == 200)                                     // Animation feux temporaire //
                {
                    led1 = 0;
                    led2 = 1;
                    led3 = 0;
                    ledX = 1;
                }

                if(secondaryCounter == 400)                                     // Animation feux temporaire //
                {
                    led1 = 1;
                    led2 = 0;
                    led3 = 1;
                    ledX = 0;
                    
                    secondaryCounter = 0;
                }
                                        
            break;
            
//============================================================================//                
          
            case READY:                                                         // Prêt à démarrer la séquence de compte à  rebours //
                
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    //DFPlayer_NextSong(); //3
                    
                    led1 = 1;                                                   // RED //
                    
                    #ifndef DEBUG_COM
                    ComFeuAllume(1,0,0,0,5,3);
                    #endif
                    
                    secondaryCounter = 0;
                                        
                    state = COUNTDOWN;                    
                }
                
            break;
            
//============================================================================//                
            
            case COUNTDOWN:                                                     // Séquence compte à rebours // 
            
                    secondaryCounterActivator = 1;
                
                    if(secondaryCounter == 100)
                    {
                        led1 = 1;                                               // RED //
                        led2 = 1;                                               // RED //
                        #ifndef DEBUG_COM
                        ComFeuAllume(1,1,0,0,5,3);
                        #endif
                    }
                    
                    if(secondaryCounter == 200)
                    {
                        led1 = 1;                                               // RED //
                        led2 = 1;                                               // RED //
                        led3 = 1;                                               // RED //
                        #ifndef DEBUG_COM
                        ComFeuAllume(1,1,1,0,5,3);
                        #endif
                    }
                    
                    if(secondaryCounter == 300)    
                    {
                        led1 = 1;                                               // RED //
                        led2 = 1;                                               // RED //
                        led3 = 1;                                               // RED //
                        ledX = 1;                                               // EXTRA LED - RED //
                        
                        #ifndef DEBUG_COM
                        ComFeuAllume(1,1,1,1,5,3);
                        #endif
                    }
                    
                    //== BON DÉPART ==//
                    if(secondaryCounter == 400)
                    {
                        led1 = 1;                                               // GREEN // 
                        led2 = 1;                                               // GREEN //
                        led3 = 1;                                               // GREEN //
                        ledX = 1;                                               // EXTRA LED - GREEN //
                        #ifndef DEBUG_COM
                        ComFeuAllume(1,1,1,1,6,3);
                        #endif      
                                        
                        counter = 0;
                        counterActivator = 1;                                   // Démarre le chronomètre //   
                      
                        secondaryCounter = 0;
                        secondaryCounterActivator = 0;                          // Reset le timer secondaire //
                        
                        state = LAUNCHING;                       
                    } 
                
                //== FAUX DÉPART ==//
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    #ifndef DEBUG_COM
                    ComINF_MessageInfo(  1111, 3, 0000);                        // Envoie message faux départ //
                    #endif
  

                    //DFPlayer_PlaySongNb(5); //5
                    
                    secondaryCounterActivator = 0;                              // Stop séquence compte à rebours //
                    secondaryCounter = 0;
                    counterActivator = 1;                                       // Démarre le chronomètre //     
                    
                    counter = DELAY_FALSE_START;                                // Pénalité de 2s //

                    led1 = 0;                                                   // GREEN // 
                    led2 = 0;                                                   // GREEN //
                    led3 = 0;                                                   // GREEN //
                    ledX = 1;                                                   // EXTRA LED - GREEN //
                    
                    #ifndef DEBUG_COM
                    ComFeuAllume(1,1,1,1,6,3);
                    #endif
                    
                    secondaryCounter = 0;
                    
                    state = RACE;
                }
                                
            break;
            
//============================================================================//                  
            
            case LAUNCHING:                                                     // Lancement de la voiture //
                
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    reactionTime = counter;                                     // Enregistre temps de réaction //                
                     
                    ledX = 1;
                        
                    #ifndef DEBUG_COM
                    ComINF_MessageInfo(1111, 2, reactionTime);                  // Envoie temps de réaction //
                    #endif           
                    
                    state = RACE;
                }
                
                //== MISE À JOUR CHRONOMÈTRE ==//
                if(toDisplay == 1)                                              // Démarre chronomètre en rouge //
                {
                    toDisplay = 0;  
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_RUNNING_TIME);
                    ComDisplay_Time(counter/100,counter%100);                    
                }
                
            break;
            
//============================================================================//    
         
            case RACE:                                                          // Course //
                
                led1 = 0;  
                led2 = 0;  
                led3 = 0;                                                           
                ledX = 1; 
                
                //== TEMPS INTERMÉDIAIRE ==//
                if(interSignal == 0 && prevSignal == 1)
                {
                    stopTime = counter;                                         // Enregistre temps de réaction //
                    
                    cell++;                                                     // Pour montrer correctement le message aux INFOS // 
                    
                    ComDisplay_Color(COLOR_GREEN);
                    ComDisplay_Mode(MODE_NET_TIME);                             
                    ComDisplay_Time(stopTime/100,stopTime%100);                 // Montre temps intermédiaire au centième de seconde //
                    
                    #ifndef DEBUG_COM
                    ComINF_MessageInfo(1111, cell, stopTime);                    // Envoi temps intermédiaire //
                    #endif      
                    
                    secondaryCounter = 0;
                    
                    state = TIME;
                }
                
//                if(fastElevator == 1)
//                {
//                    //Activer ascenseur rapide
//                }
//                
//                if(slowElevator == 1)
//                {
//                    //Activer ascenseur lent
//                }
               
                //== FIN COURSE ==//
                else if(finalSignal == 0 && prevFinal == 1)
                {
                    finalTime = counter;                                        // Enregistre temps final //
                    
                   //DFPlayer_PlaySongNb(4);
                    
                    ComDisplay_Mode(MODE_NET_TIME);                             // Montre temps final avec centièmes //
                    ComDisplay_Time(finalTime/100,finalTime%100);  
                    
                    secondaryCounter = 0;
                    
                    #ifndef DEBUG_COM
                    ComINF_MessageInfo(1111, 00, finalTime);                    // Envoie message temps final //
                    #endif      

                    state = ENDING;
                }                    
                
                //== MISE À JOUR CHRONOMÈTRE ==//
                else if(toDisplay == 1)
                {
                    toDisplay = 0;  
                    ComDisplay_Color(COLOR_BLUE);                               // Montre chronomètre en bleu dès qu'on lance la voiture //
                    ComDisplay_Mode(MODE_RUNNING_TIME);
                    ComDisplay_Time(counter/100,counter%100);  
                }
                
                //== COURSE TROP LONGUE ==//
                else if(counter == 9999)
                {
                    //DFPlayer_PlaySongNb(4);
                    
                    finalTime = counter;                                        // Affiche 9999 //
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_NET_TIME);
                    ComDisplay_Time(finalTime/100,finalTime%100); 
                    
                    state = ENDING;
                }
                                
            break;
            
//============================================================================//                
            
            case TIME:                                                          // Montre temps intermédiaire pendant 2s //               
                
                secondaryCounterActivator = 1;
                
                if(secondaryCounter == 200)
                {
                    secondaryCounterActivator = 0;                              
                                                                                                 
                    state = RACE;
                }                
                
            break;
            
//============================================================================//                
            
            case ENDING:                                                        // État final pour afficher le score //
                
                secondaryCounterActivator = 1;
                
                if(secondaryCounter == 50)                                      // Animation couleurs chronomètre //
                {
                    ComDisplay_Time(finalTime/100,finalTime%100);
                    ComDisplay_Color(COLOR_RED);
                }
                
                if(secondaryCounter == 100)
                {
                    ComDisplay_Time(finalTime/100,finalTime%100);
                    ComDisplay_Color(COLOR_GREEN);
                }
                
                if(secondaryCounter == 150)
                {
                    ComDisplay_Time(finalTime/100,finalTime%100);
                    ComDisplay_Color(COLOR_BLUE);
                }
                
                if(secondaryCounter == 200)
                {
                    ComDisplay_Time(finalTime/100,finalTime%100);
                    ComDisplay_Color(COLOR_ORANGE);
                }
                
                if(secondaryCounter == 210)
                {
                    secondaryCounter = 0;
                }
                    
                if(buzzer == 0 && prevBuzzer == 1)                              // Envoie à l'état initial //
                {    
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_RUNNING_TIME);
                    ComDisplay_Time(zero/100,zero%100);
                    state = STARTING;
                }
                
            break;
            
//============================================================================//
//FIN LECTURE DES ÉTATS
//============================================================================//               
        }
        
    //== FLANCS DESCENDANTS ==//           
    prevBuzzer = buzzer;
    prevFinal = finalSignal;
    prevSignal = interSignal;
    
    //== ÉCRITURE DES SORTIES ==//               
    output_bit(LED_1, led1);
    output_bit(LED_2, led2);
    output_bit(LED_3, led3);
    output_bit(LED_X, ledX);
    
    }   //== FIN BOUCLE INFINIE ==//      
   
}