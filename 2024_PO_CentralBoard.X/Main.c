/*============================================================================*/
/* Project     : programmeCircuit                                             */
/* Author      : Cristhian Valencia                                           */
/* Target      : PIC 18F47Q10                                                 */
/*============================================================================*/

#include "18F47Q10.h"
#fuses RSTOSC_HFINTRC_64MHZ, NOWDT, NOPUT, NOBROWNOUT, LVP, NOCLKOUT ,NOEXTOSC, NOLVP, MCLR
#use delay (clock=64000000)

#include "ComINF.c"
#include "DFPlayer.c"
#include "ComXLR8.c"
#include "ComDisplay.c"
#include "ComFeux.c"
#include "I2C.c"
#include "I2C2.c"

//==============================================================================
//D�FINITION DES ENTR�ES ET SORTIES
//==============================================================================

#define BUZZER_PIN      PIN_C5                                                  // Buzzer //
#define BUZZER_LED      PIN_C2

#define PIN_1_Q         PIN_D1                                                  // Inter 1,2 and final // 
#define PIN_1_QI        PIN_A4 

#define PIN_2_Q         PIN_D2
#define PIN_2_QI        PIN_A3

#define FINAL_PIN_Q     PIN_D0
#define FINAL_PIN_QI    PIN_A5

#define MULTIPLEXER_SELECT_PIN_1  PIN_B0
#define MULTIPLEXER_SELECT_PIN_2  PIN_B1

//==============================================================================
//D�FINITION DES �TATS
//==============================================================================

#define STARTING    1                                                           // �tat initial //
#define WAIT        2                                                           // Stand-by //
#define READY       3                                                           // Pr�t � lancer // 
#define COUNTDOWN   4                                                           // Compte � rebours //
#define LAUNCHING   5                                                           // Lancement voiture //
#define RACE        6                                                           // Course //
#define TIME        7                                                           // Temps interm�diaire //
#define ENDING      8                                                           // �tat final //

#define DELAY_FALSE_START 200                                                   // P�nalit� faux d�part //

//==============================================================================
//D�FINITION DES BONUS POUR TESTS
//==============================================================================

#define AUTOMATIQUE                                                             // � mettre en commentaire pour mode normal //

#define BONUS_LEVER_1       1
#define BONUS_LEVER_2       1
#define BONUS_ELEVATOR_1    1
#define BONUS_ELEVATOR_2    1
#define BONUS_XLR8          1
#define BONUS_BLOWER        1
#define BONUS_7             1
#define BONUS_8             1

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
    if(tertiaryCounter == 100)
        {
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
        case 0:                                                                 // Display // 
            output_low(MULTIPLEXER_SELECT_PIN_1);
            output_low(MULTIPLEXER_SELECT_PIN_2);
            break;
            
        case 1:                                                                 // Feux //
            output_high(MULTIPLEXER_SELECT_PIN_1);
            output_low(MULTIPLEXER_SELECT_PIN_2);
            break;
            
        case 2:                                                                 // Musique //
            output_low(MULTIPLEXER_SELECT_PIN_1);
            output_high(MULTIPLEXER_SELECT_PIN_2);
            break;
            
        case 3:                                                                 // Acc�l�rateur //
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
    delay_ms(2000);
    
    //== INITIALISATION BONUS ==//
    enable_interrupts(INT_RDA);
    enable_interrupts(GLOBAL);
    
    //== D�CLARATION DES VARIABLES ==//                                                 
    int16 stopTime = 0;                                                         // Sauvegarde temps tournant //
    int16 finalTime = 0;                                                        // Sauvegarde temps final //
    int16 reactionTime = 0;                                                     // Sauvegarde temps de r�action //
    
    int8 state = 0;                                                             // �tat // 
    
    int8 zero = 0;                                                              // Reset de l'affichage du chrono //
    
    char r = 'r';                                                               // Sert � mettre la bonne couleur aux feux //
    char g = 'g';
    
    //== ENTR�ES ==//                            
    int8 buzzer = 0;                                                            // Bouton buzzer principal //

    //== SIGNAUX CELLULES ==//                                                      
    int8 inter1Q = 0;                                                           // Premier temps interm�diaire
    int8 inter1QI = 0;
    
    int8 inter2Q = 0;                                                           // Deuxi�me temps interm�diaire //    
    int8 inter2QI = 0; 
    
    int8 finalSignalQ = 0;                                                      // Cellule finale normale //
    int8 finalSignalQI = 0;                                                     
    
    //== BONUS ==//                                                      
    int16 carNumber = 0;
    
    int8 bonusLever1 = 0;
    int8 bonusLever2 = 0;
    int8 bonusElevator1 = 0;
    int8 bonusElevator2 = 0;
    int8 bonusXLR8 = 0;
    int8 bonusBlower = 0;
    int8 bonus7 = 0;
    int8 bonus8 = 0; 
    
    //== FLANCS DESCENDANTS ==//                         
    int8 prevBuzzer = 0;       
    
    //== INITIALISATION TIMER ==//      
    setup_timer_0(T0_INTERNAL|T0_DIV_4);                                        // Fosc/4, divis� par 4 //
    set_timer0(25536);
    enable_interrupts(INT_TIMER0);
    enable_interrupts(GLOBAL);
    
    //== INITIALISATION CHRONOM�TRE ==//
    select_multiplexer_channel(0);
    ComDisplay_Color(COLOR_RED);
    ComDisplay_Mode(MODE_RUNNING_TIME); 
       
    //== INITIALISATION FEUX ==//
    delay_ms(5);
    select_multiplexer_channel(1);
    delay_ms(5);
    ComFeuAnim(0);
    delay_ms(5);
    
    //== INITIALISATION MUSIQUE ==//
    select_multiplexer_channel(2);
    DFPlayer_Init();
    delay_ms(5);
    DFPlayer_PlaySongNb(1);
    delay_ms(5);
    
    //== MISE HORS TENSION DES CELLULES ==//
    setup_mcp23017();
    set_gpb7_low();
    delay_ms(5);
    
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
        
        inter1Q = input(PIN_1_Q);
        inter1QI = input(PIN_1_QI);
        
        inter2Q = input(PIN_2_Q);
        inter2QI = input(PIN_2_QI);
        
        finalSignalQ = input(FINAL_PIN_Q);
        finalSignalQI = input(FINAL_PIN_QI);

        
//==============================================================================        
//LECTURE DES �TATS
//==============================================================================
        
        switch (state)
        {                                                   
            case STARTING:                                                      // Reset de toutes les valeurs //
                
                setup_mcp23017();
                set_gpb7_low();
                
                bonus_activator();
                deactivate_bonus_1_lever();
                deactivate_bonus_2_lever();  
                deactivate_bonus_3_lever();
                deactivate_bonus_4_elevator();
                deactivate_bonus_5_elevator();
                deactivate_bonus_6_blower();
                deactivate_bonus_7_blower();
                deactivate_bonus_8_startBlower();

                select_multiplexer_channel(2);
                DFPlayer_PlaySongNb(1);
                delay_ms(5);
                
                counterActivator = 0;
                counter = 0;
                secondaryCounterActivator = 0;
                secondaryCounter = 0;
                
                byteNumber = 0;
                
                elevatorBonus = 0;
                bonusFull = 0;
                bonusBlock = 0;
                
                bonusLever1 = 0;
                bonusLever2 = 0;
                bonusElevator1 = 0;
                bonusElevator2 = 0;
                bonusXLR8 = 0;
                bonusBlower = 0;
                bonus7 = 0;
                bonus8 = 0; 
                
                carNumber = 0;
                
                stopTime = 0;
                finalTime = 0;
                reactionTime = 0;
                
                delay_ms(5);
                select_multiplexer_channel(0);
                ComDisplay_Time(zero, zero);                                    // Affiche 0:0 sur l'afficheur //
                ComDisplay_Color(COLOR_RED);
                ComDisplay_Mode(MODE_RUNNING_TIME);
               
                delay_ms(2000);
                select_multiplexer_channel(1);
                delay_ms(5);
                ComFeuAnim(1);                                                  // Active animation Feux //
                
                delay_ms(1000);                                                 // D�lai pour �viter perturbations //
                 
                state = WAIT;
 
            break;

//============================================================================//

            
       
//== MANUEL ==//
#ifndef AUTOMATIQUE
            
            case WAIT:                                                          // �tat de stand-by //
                
                secondaryCounterActivator = 1;                                  // ??? //
                               
                if(buzzer == 0 && prevBuzzer == 1 && bonusFull == 1)
                {      
                    carNumber = (rxBuffer[0]-48)*1000 + (rxBuffer[1]-48)*100 + (rxBuffer[2]-48)*10 + rxBuffer[3]-48;                    
                    
                    bonusLever1     =(rxBuffer[5 ]-48);
                    bonusLever2     =(rxBuffer[7 ]-48);
                    bonusElevator1  =(rxBuffer[9 ]-48);
                    bonusElevator2  =(rxBuffer[11]-48);
                    bonusXLR8       =(rxBuffer[13]-48);
                    bonusBlower     =(rxBuffer[15]-48);
                    bonus7          =(rxBuffer[17]-48);
                    bonus8          =(rxBuffer[19]-48);
                    
                    bonusBlock = 1;                     
                    
                    select_multiplexer_channel(1);
                    ComFeuAnim(0);                                              // Arr�te l'animation des feux //
                    delay_ms(5);
                              
                    ComINF_MessageInfo(1111,1,0000);                            // Envoi message pr�t //
                    
                    secondaryCounter = 0;                                       
                    secondaryCounterActivator = 0;                              // Reset compteur secondaire pour commencer � 0 //                    
                    
                    select_multiplexer_channel(2);
                    DFPlayer_NextSong(); //2                                    // Change la musique //
                    
                    delay_ms(1000);                                             // D�lai pour �viter perturbations //
                    
                    state = READY;
                }          
                
            break;
            
#endif 
                  
//== AUTOMATIQUE== //             
#ifdef AUTOMATIQUE                                                                 
                    
            case WAIT:                                                          // �tat de stand-by //
                    
                secondaryCounterActivator = 1;
                               
                if(buzzer == 0 && prevBuzzer == 1)
                {      
                    
                    carNumber = (rxBuffer[0]-48)*1000 + (rxBuffer[1]-48)*100 + (rxBuffer[2]-48)*10 + rxBuffer[3]-48;                    
                    
                    bonusLever1     = BONUS_LEVER_1;
                    bonusLever2     = BONUS_LEVER_2;
                    bonusElevator1  = BONUS_ELEVATOR_1;
                    bonusElevator2  = BONUS_ELEVATOR_2;
                    bonusXLR8       = BONUS_XLR8;
                    bonusBlower     = BONUS_BLOWER;
                    bonus7          = BONUS_7;
                    bonus8          = BONUS_8;
                    
                    bonusBlock = 1;                     
                    
                    select_multiplexer_channel(1);
                    ComFeuAnim(0);                                              // Arr�te l'animation des feux //
                    delay_ms(5);
                              
                    ComINF_MessageInfo(1111,1,0000);                            // Envoi message pr�t //
                    
                    secondaryCounter = 0;                                       
                    secondaryCounterActivator = 0;                              // Reset compteur secondaire pour commencer � 0 //                    
                    
                    delay_ms(1000);
                    
                    select_multiplexer_channel(2);
                    //DFPlayer_NextSong(); //2                                    // Change la musique //
                    
                    delay_ms(1000);                                             // D�lai pour �viter perturbations //
                    
                    
                    state = READY;
                }          
                
            break;
#endif
            
//============================================================================//                
          
            case READY:                                                         // Pr�t � d�marrer la s�quence de compte �  rebours //
                
//                if(buzzer == 0 && prevBuzzer == 1)
//                {
                    select_multiplexer_channel(2);
                    DFPlayer_NextSong(); //2
                    delay_ms(5);
                    
                    select_multiplexer_channel(1);
                    ComFeuAllume(1,0,0,0,r,3);                                  // Feu 1 � 1 - Feu 2 � 0 - Feu 3 � 0 - Feu 4 � 0 - Rouge - luminosit� max //
                    
                    secondaryCounter = 0;
                                        
                    state = COUNTDOWN;                    
//                }
                
            break;
            
//============================================================================//                
            
            case COUNTDOWN:                                                     // S�quence compte � rebours // 
            
                secondaryCounterActivator = 1;

                if(secondaryCounter == 100)
                { 
                    ComFeuAllume(1,1,0,0,r,3);
                }

                if(secondaryCounter == 200)
                {
                    ComFeuAllume(1,1,1,0,r,3);
                }

                if(secondaryCounter == 300)    
                {   
                    ComFeuAllume(1,1,1,1,r,3);
                }

                //== BON D�PART ==//
                if(secondaryCounter == 400)
                { 
                    ComFeuAllume(1,1,1,1,g,3);
                    delay_ms(5);
                    
                    counter = 0;
                    counterActivator = 1;                                       // D�marre le chronom�tre //   
                
                    secondaryCounter = 0;
                    secondaryCounterActivator = 0;                              // Reset le timer secondaire //

                    state = LAUNCHING;                       
                } 
                
                //== FAUX D�PART ==//
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    bonus_activator();
                    
                    delay_ms(5);                    
                    select_multiplexer_channel(3);
                    startXLR8(bonusXLR8);
                    delay_ms(5);
                    
                    if(bonusBlower == 1)
                    {
                        bonus_activator();
                        activate_bonus_8_startBlower();
                    }
                    
                    setup_mcp23017();
                    set_gpb7_high();
                    
                    
                    ComINF_MessageInfo(carNumber,2,0000);                       // Envoie message faux d�part //
                    delay_ms(5);
                    
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(4); //CHANGE (5)
                    delay_ms(5);
                    
                    secondaryCounterActivator = 0;                              // Stop s�quence compte � rebours //
                    secondaryCounter = 0;
                    
                    counterActivator = 1;                                       // D�marre le chronom�tre //     
                    
                    counter = DELAY_FALSE_START;                                // P�nalit� de 2s //

                    select_multiplexer_channel(1);
                    ComFeuAllume(1,1,1,1,g,3);                      
                    delay_ms(5);
                    
                    secondaryCounter = 0;
                    
                    state = RACE;
                }
                                
            break;
            
//============================================================================//                  
            
            case LAUNCHING:                                                     // Lancement de la voiture //
                
                if(buzzer == 0 && prevBuzzer == 1)
                {
                    delay_ms(5);                    
                    select_multiplexer_channel(3);
                    startXLR8(bonusXLR8);
                    delay_ms(5);
                    
                    reactionTime = counter;                                     // Enregistre temps de r�action //               
                    
                    ComINF_MessageInfo(carNumber,2,reactionTime);               // Envoie temps de r�action //
                    
                    setup_mcp23017();
                    set_gpb7_high();
                    
                    bonus_activator();
                    
                    if(bonusBlower == 1)
                    {
                        bonus_activator();
                        activate_bonus_8_startBlower();
                    }
                    
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
                
                bonus_activator();
                
                //== TEMPS INTERM�DIAIRE 1 ==//
                if(inter1Q == 1 && inter1QI == 0)
                {        
                    bonus_activator();
                    deactivate_bonus_8_startBlower();
                    
                    if(bonusElevator1 == 1)
                    {
                        activate_bonus_4_elevator();
                    }
                    
                    if(bonusLever1 == 1 && bonusLever2 == 1)
                    {
                        activate_bonus_1_lever();
                        deactivate_bonus_2_lever();
                        deactivate_bonus_3_lever();
                    }
                    
                    if(bonusLever1 == 0 && bonusLever2 == 0)
                    {
                        activate_bonus_1_lever();
                        activate_bonus_2_lever();
                        activate_bonus_3_lever();
                    }
                    
                    if((bonusLever1 == 0 && bonusLever2 == 1)||(bonusLever1 == 1 && bonusLever2 == 0))
                    {
                        deactivate_bonus_1_lever();
                        deactivate_bonus_2_lever();
                        deactivate_bonus_3_lever();
                    }    
                    activate_bonus_6_blower();
                    
                    stopTime = counter;                                         // Enregistre temps interm�diaire //
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_GREEN);
                    ComDisplay_Mode(MODE_NET_TIME);                             
                    ComDisplay_Time(stopTime/100,stopTime%100);                 // Montre temps interm�diaire au centi�me de seconde //
                    delay_ms(5);                    
                    
                    ComINF_MessageInfo(carNumber, 4, stopTime);                 // Envoi temps interm�diaire //
                    
                    secondaryCounter = 0;
                    
                    state = TIME;
                }
                
                //== TEMPS INTERM�DIAIRE 2 ==//
                if(inter2Q == 1 && inter2QI == 0)
                {      
                    deactivate_bonus_4_elevator();
                    
                    if(bonusElevator2 == 1)
                    {
                        activate_bonus_5_elevator();
                    }
                    
                    bonus_activator();
                    deactivate_bonus_1_lever();
                    deactivate_bonus_2_lever();
                    deactivate_bonus_3_lever();  
                    
                    deactivate_bonus_6_blower();
                    
                    activate_bonus_7_blower();
                    
                    stopTime = counter;                                         // Enregistre temps de r�action //
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_GREEN);
                    ComDisplay_Mode(MODE_NET_TIME);                             
                    ComDisplay_Time(stopTime/100,stopTime%100);                 // Montre temps interm�diaire au centi�me de seconde //
                    delay_ms(5);                    
                    
                    ComINF_MessageInfo(carNumber,5,stopTime);                   // Envoi temps interm�diaire //
                    
                    secondaryCounter = 0;
                    
                    state = TIME;
                } 
               
                //== FIN COURSE ==//
                else if(finalSignalQ == 1  && finalSignalQI == 0)
                {
                    deactivate_bonus_5_elevator();
                    
                    finalTime = counter;                                        // Enregistre temps final //
                   
                    bonus_activator();
                    
                    deactivate_bonus_7_blower();
                    
                    
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(3);
                    delay_ms(5);
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Mode(MODE_NET_TIME);                             // Montre temps final avec centi�mes //
                    ComDisplay_Time(finalTime/100,finalTime%100);  
                    delay_ms(5);
                    
                    secondaryCounter = 0;
                    
                    ComINF_MessageInfo(carNumber,6,finalTime);                  // Envoie message temps final //

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
                else if(counter >= 9998)
                {
                    deactivate_bonus_7_blower();
                    
                    select_multiplexer_channel(1);
                    ComFeuAllume(1,1,1,1,r,3);
                    delay_ms(5);
                    
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(3);
                    delay_ms(5);
                    
                    finalTime = 9999;                                             // Affiche 9999 //
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_NET_TIME);
                    ComDisplay_Time(99,99); 
                    delay_ms(5);
                    
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
                
                //== COURSE TROP LONGUE ==//
                if(counter >= 9999)
                {
                    deactivate_bonus_7_blower();
                    
                    select_multiplexer_channel(1);
                    ComFeuAllume(1,1,1,1,r,3);
                    delay_ms(5);
                    
                    select_multiplexer_channel(2);
                    DFPlayer_PlaySongNb(4);
                    delay_ms(5);
                    
                    finalTime = counter;                                        // Affiche 9999 //
                    
                    select_multiplexer_channel(0);
                    ComDisplay_Color(COLOR_RED);
                    ComDisplay_Mode(MODE_NET_TIME);
                    ComDisplay_Time(finalTime/100,finalTime%100); 
                    delay_ms(5);
                    
                    state = ENDING;
                }
                
            break;
            
//============================================================================//                
            
            case ENDING:                                                        // �tat final pour afficher le score //
                
                secondaryCounterActivator = 1;
                
                delay_ms(5);
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
    
        delay_us(10);
        
    //== FLANCS DESCENDANTS ==//           
    prevBuzzer = buzzer;
  
    delay_ms(2);
    
    }   //== FIN BOUCLE INFINIE ==//      
   
}