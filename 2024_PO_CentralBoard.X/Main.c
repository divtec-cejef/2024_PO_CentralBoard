/*----------------------------------------------------------------------------*/
/* Project     : programmeCircuit                                             */
/* Author      : Cristhian Valencia                                           */
/* Target      : PIC 18F47Q10                                                 */
/*----------------------------------------------------------------------------*/

#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT ,NOEXTOSC
#use delay (clock=64000000)
#include "ComINF.c"
#include "DFPlayer.c"
#include "ComDisplay.c"
#include "ComFeux.c"

//==============================================================================
//D�FINITION DES ENTR�ES
//==============================================================================

#define BUZZER_PIN  PIN_C5
#define SIGNAL_PIN  PIN_C3 
#define FINAL_PIN   PIN_C4

#define MULTIPLEXER_SELECT_PIN_1  PIN_B0 
#define MULTIPLEXER_SELECT_PIN_2  PIN_B1

//==============================================================================
//D�FINITION DES �TATS
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

//==============================================================================
//VARIABLES GLOBALES
//==============================================================================

int8 secondaryCounterActivator = 0;                                             // Activateur compteur pour op�rations non-li�es au chronom�tre //
int8 counterActivator = 0;                                                      // Activateur compteur du chronom�tre //
int8 toDisplay = 0;                                                             // Sert � afficher le temps sans les centi�mes //

int16 tertiaryCounter = 0;
int16 secondaryCounter = 0;                                                     // Compteur secondaire //
int16 counter = 0;                                                              // Compteur principal pour le chronom�tre //

//==============================================================================
//INT�RRUPTION COMPTEURS 
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
//PROGRAMME MULTIPLEXEUR 
//==============================================================================

void select_multiplexer_channel(int channel) 
{
    switch(channel) 
    {
        case 0:
            output_low(MULTIPLEXER_SELECT_PIN_1);
            output_low(MULTIPLEXER_SELECT_PIN_2);
            break;
            
        case 1:
            output_high(MULTIPLEXER_SELECT_PIN_1);
            output_low(MULTIPLEXER_SELECT_PIN_2);
            break;
            
        case 2:
            output_low(MULTIPLEXER_SELECT_PIN_1);
            output_high(MULTIPLEXER_SELECT_PIN_2);
            break;
            
        case 3:
            output_high(MULTIPLEXER_SELECT_PIN_1);
            output_high(MULTIPLEXER_SELECT_PIN_2);
            break;
    }
}

//==============================================================================
//PROGRAMME PRINCIPAL
//==============================================================================

void main()
{
    output_bit(PIN_E0, 1); 
    
    //== INITIALISATION BONUS ==//
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    
    //== D�CLARATION DES VARIABLES ==//                                                 
    int16 stopTime = 0;                                                         // Sauvegarde temps tournant //
    int16 finalTime = 0;                                                        // Sauvegarde temps final //
    int16 reactionTime = 0;                                                     // Sauvegarde temps de r�action //
    
    int8 state = 0;                                                             // �tat // 
    
    int8 zero = 0;                                                              // Reset de l'affichage du chrono //
    
    int8 cell = 3;                                                              // Sert � bien num�roter les messages //
    
    //== ENTR�ES ==//                            
    int8 buzzer = 0;                                                            // Bouton buzzer principal //

    //== SIGNAUX CELLULES ==//
    int8 interSignal = 0;                                                       // Cellule temps interm�diaire (simulation bouton S2)//
    int8 finalSignal = 0;                                                       // Cellule finale (simulation bouton extra)//
//    int8 fastElevator = 0; 
//    int8 slowElevator = 0;

    //== FLANCS DESCENDANTS ==//                         
    int8 prevBuzzer = 0;                                                        // Antirebond Buzzer //
    int8 prevSignal = 0;                                                        // Antirebond Bouton S2 //
    int8 prevFinal = 0;                                                         // Antirebond Bouton extra //
    
    //== INITIALISATION TIMER ==//      
    setup_timer_0(T0_INTERNAL|T0_DIV_4);                                        // Fosc/4, divis� par 4 //
    set_timer0(25536);
    enable_interrupts(INT_TIMER0);                                              
    enable_interrupts(GLOBAL);
    
    select_multiplexer_channel(0);
    //ComDisplay_Color(COLOR_RED);
    //ComDisplay_Mode(MODE_RUNNING_TIME); 
    
    //== INITIALISATION MUSIQUE ==//
    select_multiplexer_channel(2);
    DFPlayer_Init();            
    DFPlayer_PlaySongNb(1);
    
    
    //== INITIALISATION MULTIPLEXEUR ==//
    output_bit(MULTIPLEXER_SELECT_PIN_1 , 0);
    output_bit(MULTIPLEXER_SELECT_PIN_2 , 0);
    
    //== INITIALISATION PROGRAMME PRINCIPAL ==//
    state = STARTING;

    //== D�BUT BOUCLE INFINIE ==//
    while(TRUE)
    { 
        //== LECTURE DES ENTR�ES ==//        
        buzzer = input(BUZZER_PIN);
        finalSignal = input (FINAL_PIN);
        interSignal = input(SIGNAL_PIN);

        
//==============================================================================        
//LECTURE DES �TATS
//==============================================================================
        
        switch (state)
        {                                                   
            case STARTING:                                                      // Reset de toutes les valeurs //
                
                select_multiplexer_channel(2);
                DFPlayer_PlaySongNb(1);
                
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
                
                select_multiplexer_channel(0);
                ComDisplay_Time(zero, zero);
                ComDisplay_Color(COLOR_RED);
                ComDisplay_Mode(MODE_RUNNING_TIME);
                
                
                select_multiplexer_channel(1);
                ComFeuAllume(0,0,0,0,5,3);
                ComFeuAnim(0);                                                  // Active animation Feux //
                
                
                delay_ms(1000);                                                 // D�lai pour �viter perturbations //
                
                state = WAIT;
 
            break;

//============================================================================//
                
            case WAIT:                                                          // �tat de stand-by //
                
                secondaryCounterActivator = 1;
                
                if(buzzer == 0 && prevBuzzer == 1 && bonus == 1)
                {      
                    bonusBlock = 1;                     
                    
                    select_multiplexer_channel(1);
                    ComFeuAnim(0);
                    
                    ComINF_MessageInfo(1111, 1, 6969);                          // Envoi message pr�t //
                    
                    secondaryCounter = 0;                                       
                    secondaryCounterActivator = 0;                              // Reset compteur secondaire pour commencer � 0 //                    
                    
                    select_multiplexer_channel(2);
                    DFPlayer_NextSong(); //2
                    
                    delay_ms(1000);                                             // D�lai pour �viter perturbations //
                    
                    state = READY;
                }
                
                                        
            break;
            
//============================================================================//                
          
            case READY:                                                         // Pr�t � d�marrer la s�quence de compte �  rebours //
                
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    select_multiplexer_channel(2);
                    DFPlayer_NextSong(); //3
                    
                    ComFeuAllume(1,0,0,0,5,3);
                    
                    secondaryCounter = 0;
                                        
                    state = COUNTDOWN;                    
                }
                
            break;
            
//============================================================================//                
            
            case COUNTDOWN:                                                     // S�quence compte � rebours // 
            
                    secondaryCounterActivator = 1;
                
                    if(secondaryCounter == 100)
                    { 
                        select_multiplexer_channel(1);
                        ComFeuAllume(1,1,0,0,5,3);
                    }
                    
                    if(secondaryCounter == 200)
                    {
                        select_multiplexer_channel(1);
                        ComFeuAllume(1,1,1,0,5,3);
                    }
                    
                    if(secondaryCounter == 300)    
                    {   
                        select_multiplexer_channel(1);
                        ComFeuAllume(1,1,1,1,5,3);
                    }
                    
                    //== BON D�PART ==//
                    if(secondaryCounter == 400)
                    { 
                        select_multiplexer_channel(1);
                        ComFeuAllume(1,1,1,1,6,3);
                                        
                        counter = 0;
                        counterActivator = 1;                                   // D�marre le chronom�tre //   
                      
                        secondaryCounter = 0;
                        secondaryCounterActivator = 0;                          // Reset le timer secondaire //
                        
                        state = LAUNCHING;                       
                    } 
                
                //== FAUX D�PART ==//
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    ComINF_MessageInfo(  1111, 3, 0000);                        // Envoie message faux d�part //
  
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(5); //5
                    
                    secondaryCounterActivator = 0;                              // Stop s�quence compte � rebours //
                    secondaryCounter = 0;
                    counterActivator = 1;                                       // D�marre le chronom�tre //     
                    
                    counter = DELAY_FALSE_START;                                // P�nalit� de 2s //

                    select_multiplexer_channel(1);
                    ComFeuAllume(1,1,1,1,6,3);
                    
                    secondaryCounter = 0;
                    
                    state = RACE;
                }
                                
            break;
            
//============================================================================//                  
            
            case LAUNCHING:                                                     // Lancement de la voiture //
                
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    reactionTime = counter;                                     // Enregistre temps de r�action //               
                    
                    ComINF_MessageInfo(1111, 2, reactionTime);                  // Envoie temps de r�action //
                    
                    state = RACE;
                }
                
                //== MISE � JOUR CHRONOM�TRE ==//
                if(toDisplay == 1)                                              // D�marre chronom�tre en rouge //
                {
                    select_multiplexer_channel(0);
                    toDisplay = 0;  
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_RUNNING_TIME);
                    ComDisplay_Time(counter/100,counter%100);                    
                }
                
            break;
            
//============================================================================//    
         
            case RACE:                                                          // Course //
                 
                //== TEMPS INTERM�DIAIRE ==//
                if(interSignal == 0 && prevSignal == 1)
                {
                    stopTime = counter;                                         // Enregistre temps de r�action //
                    
                    cell++;                                                     // Pour montrer correctement le message aux INFOS // 
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_GREEN);
                    ComDisplay_Mode(MODE_NET_TIME);                             
                    ComDisplay_Time(stopTime/100,stopTime%100);                 // Montre temps interm�diaire au centi�me de seconde //
                    
                    ComINF_MessageInfo(1111, cell, stopTime);                    // Envoi temps interm�diaire //
                    
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
                   
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(4);
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Mode(MODE_NET_TIME);                             // Montre temps final avec centi�mes //
                    ComDisplay_Time(finalTime/100,finalTime%100);  
                    
                    secondaryCounter = 0;
                    
                    ComINF_MessageInfo(1111, 00, finalTime);                    // Envoie message temps final //

                    state = ENDING;
                }                    
                
                //== MISE � JOUR CHRONOM�TRE ==//
                else if(toDisplay == 1)
                {
                    select_multiplexer_channel(0);
                    toDisplay = 0;  
                    ComDisplay_Color(COLOR_BLUE);                               // Montre chronom�tre en bleu d�s qu'on lance la voiture //
                    ComDisplay_Mode(MODE_RUNNING_TIME);
                    ComDisplay_Time(counter/100,counter%100);  
                }
                
                //== COURSE TROP LONGUE ==//
                else if(counter == 9999)
                {
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(4);
                    
                    finalTime = counter;                                        // Affiche 9999 //
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_NET_TIME);
                    ComDisplay_Time(finalTime/100,finalTime%100); 
                    
                    state = ENDING;
                }
                                
            break;
            
//============================================================================//                
            
            case TIME:                                                          // Montre temps interm�diaire pendant 2s //               
                
                secondaryCounterActivator = 1;
                
                if(secondaryCounter == 200)
                {
                    secondaryCounterActivator = 0;                              
                                                                                                 
                    state = RACE;
                }                
                
            break;
            
//============================================================================//                
            
            case ENDING:                                                        // �tat final pour afficher le score //
                
                secondaryCounterActivator = 1;
                
                select_multiplexer_channel(0);

                if(secondaryCounter == 50)                                      // Animation couleurs chronom�tre //
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
                    
                if(buzzer == 0 && prevBuzzer == 1)                              // Envoie � l'�tat initial //
                {    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_RUNNING_TIME);
                    ComDisplay_Time(zero/100,zero%100);
                    state = STARTING;
                }
                
            break;
            
//============================================================================//
//FIN LECTURE DES �TATS
//============================================================================//               
        }
        
    //== FLANCS DESCENDANTS ==//           
    prevBuzzer = buzzer;
    prevFinal = finalSignal;
    prevSignal = interSignal;
  
    }   //== FIN BOUCLE INFINIE ==//      
   
}