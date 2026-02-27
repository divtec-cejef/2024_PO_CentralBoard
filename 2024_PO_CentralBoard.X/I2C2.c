#include "I2C2.h"

//== FONCTION D'ÉCRITURE I2C POUR MCP23017 ==//
void mcp23017_write_2(unsigned char reg, unsigned char value) 
{
    i2c_start();                                                                // Démarrer la communication I2C
    i2c_write(MCP23017_ADDRESS_2);                                              // Envoi de l'adresse I2C du MCP23017
    i2c_write(reg);                                                             // Sélection du registre à écrire
    i2c_write(value);                                                           // Envoi des données
    i2c_stop();                                                                 // Arrêter la communication I2C
}

void bonus_activator()
{
    mcp23017_write_2(IODIRA_2, 0x01);                                           // Configurer toutes les broches du port A (GPA0 à GPA7) comme sorties
    mcp23017_write_2(IODIRB_2, 0x01);                                           // Configurer toutes les broches du port B (GPB0 à GPB7) comme sorties 
}

// TOUS LES PORTS SONT A LA SUITE, MANQUE UNIQUEMENT AU 8eme PORT ET APRES LE 10e (en commençant a compter depuis les entrées des capteurs)

//== ACTIVER BONUS ASCENSEUR 1 ==//
void activate_bonus_1_lever() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x80; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB9 à HIGH (bit 9 à 1)
}

//== DÉSACTIVER BONUS ASCENSEUR 1 ==// 
void deactivate_bonus_1_lever()
{
    bonus_activator();
    stateGpiob_2 &= 0x7F;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB9 à LOW (bit 9 à 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS ASCENSEUR 2 ==//
void activate_bonus_2_lever() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x40; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB8 à HIGH (bit 8 à 1)
}

//== DÉSACTIVER BONUS ASCENSEUR 2 ==//
void deactivate_bonus_2_lever()
{
    bonus_activator();
    stateGpiob_2 &= 0xBF;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB8 à LOW (bit 8 à 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER ACCELERATION BACKUP (soufflette) ==//                              
void activate_XLR8_backup()
{
    bonus_activator();                                                          // Mettre GPB1 à HIGH (PIN 2, J20, port num.7)
    stateGpiob_2 |= 0x02; //--> 0b00000010
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    
}

//== DESACTIVER ACCELERATION BACKUP (soufflette) ==//                           
void deactivate_XLR8_backup()
{
    bonus_activator();                                                      // Mettre GPB1 à LOW (PIN 2, J20, port num.7)
    stateGpiob_2 &= 0xFD;
    mcp23017_write_2(GPIOB_2, stateGpiob_2); 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 3 ==//
void activate_bonus_3_lever() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x20; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB5 à HIGH (bit 5 à 1)
}

//== DÉSACTIVER BONUS 3 ==//
void deactivate_bonus_3_lever()
{
    bonus_activator();
    stateGpiob_2 &= 0xDF;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB5 à LOW (bit 5 à 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 4 ==//
void activate_bonus_4_elevator() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x10; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB4 à HIGH (bit 4 à 1)
}

//== DÉSACTIVER BONUS 4 ==//
void deactivate_bonus_4_elevator()
{
    bonus_activator();
    stateGpiob_2 &= 0xEF;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB4 à LOW (bit 4 à 0) 
}


////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 5 ==//
void activate_bonus_5_elevator()
{     
    bonus_activator();
    stateGpiob_2 |= 0x08; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB3 à HIGH (bit 3 à 1)
}

//== DÉSACTIVER BONUS 5 ==//
void deactivate_bonus_5_elevator()
{
    bonus_activator();
    stateGpiob_2 &= 0xF7;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB3 à LOW (bit 3 à 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 6 ==//
void activate_bonus_6_blower()
{     
    bonus_activator();
    stateGpiob_2 |= 0x04; //--> 0b00000100
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB2 à HIGH (bit 2 à 1)
}

//== DÉSACTIVER BONUS 6 ==//
void deactivate_bonus_6_blower()
{
    bonus_activator();
    stateGpiob_2 &= 0xFB;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB2 à LOW (bit 2 à 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 7 ==//
void activate_bonus_7_blower() 
{     
    bonus_activator();
    stateGpioa_2 |= 0x40;   //0x40 -> 0b01000000
    mcp23017_write_2(GPIOA_2, stateGpioa_2);                                    // Mettre GPA6 à HIGH (PIN 27, J23, port num.10)
}

//== DÉSACTIVER BONUS 7 ==//
void deactivate_bonus_7_blower()
{
    bonus_activator();                                                          // Mettre GPA6 à LOW (PIN 27, J23, port num.10) 
    stateGpioa_2 &= 0xBF;  //0b10111111
    mcp23017_write_2(GPIOA_2, stateGpioa_2);       
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 8 ==//
void activate_bonus_8_startBlower()
{     
    bonus_activator();
    stateGpioa_2 |= 0x80; 
    mcp23017_write_2(GPIOA_2, stateGpioa_2);                                    // Mettre GPA7 à HIGH (PIN 28, J22, port num.9)
}

//== DÉSACTIVER BONUS 8 ==//
void deactivate_bonus_8_startBlower()
{
    bonus_activator();
    stateGpioa_2 &= 0x7F;
    mcp23017_write_2(GPIOA_2, stateGpioa_2);                                    // Mettre GPA7 à LOW (PIN 28, J22, port num.9)
}