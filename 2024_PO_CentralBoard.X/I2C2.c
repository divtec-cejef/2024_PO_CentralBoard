#include "I2C2.h"

//== FONCTION D'�CRITURE I2C POUR MCP23017 ==//
void mcp23017_write_2(unsigned char reg, unsigned char value) 
{
    i2c_start();                                                                // D�marrer la communication I2C
    i2c_write(MCP23017_ADDRESS_2);                                              // Envoi de l'adresse I2C du MCP23017
    i2c_write(reg);                                                             // S�lection du registre � �crire
    i2c_write(value);                                                           // Envoi des donn�es
    i2c_stop();                                                                 // Arr�ter la communication I2C
}

void bonus_activator()
{
    mcp23017_write_2(IODIRA_2, 0x01);                                           // Configurer toutes les broches du port A (GPA0 � GPA7) comme sorties
    mcp23017_write_2(IODIRB_2, 0x01);                                           // Configurer toutes les broches du port B (GPB0 � GPB7) comme sorties 
}

//== ACTIVER BONUS ASCENSEUR 1 ==//
void activate_bonus_1_lever() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x80; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB7 � HIGH (bit 7 � 1)
}

//== D�SACTIVER BONUS ASCENSEUR 1 ==//
void deactivate_bonus_1_lever()
{
    bonus_activator();
    stateGpiob_2 &= 0x7F;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB7 � LOW (bit 7 � 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS ASCENSEUR 2 ==//
void activate_bonus_2_lever() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x40; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB6 � HIGH (bit 6 � 1)
}

//== D�SACTIVER BONUS ASCENSEUR 2 ==//
void deactivate_bonus_2_lever()
{
    bonus_activator();
    stateGpiob_2 &= 0xBF;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB6 � LOW (bit 6 � 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 3 ==//
void activate_bonus_3_lever() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x20; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB5 � HIGH (bit 5 � 1)
}

//== D�SACTIVER BONUS 3 ==//
void deactivate_bonus_3_lever()
{
    bonus_activator();
    stateGpiob_2 &= 0xDF;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB5 � LOW (bit 5 � 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 4 ==//
void activate_bonus_4_elevator() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x10; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB4 � HIGH (bit 4 � 1)
}

//== D�SACTIVER BONUS 4 ==//
void deactivate_bonus_4_elevator()
{
    bonus_activator();
    stateGpiob_2 &= 0xEF;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB4 � LOW (bit 4 � 0) 
}


////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 5 ==//
void activate_bonus_5_elevator()
{     
    bonus_activator();
    stateGpiob_2 |= 0x08; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB3 � HIGH (bit 3 � 1)
}

//== D�SACTIVER BONUS 5 ==//
void deactivate_bonus_5_elevator()
{
    bonus_activator();
    stateGpiob_2 &= 0xF7;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB3 � LOW (bit 3 � 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 6 ==//
void activate_bonus_6_blower()
{     
    bonus_activator();
    stateGpiob_2 |= 0x04; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB2 � HIGH (bit 2 � 1)
}

//== D�SACTIVER BONUS 6 ==//
void deactivate_bonus_6_blower()
{
    bonus_activator();
    stateGpiob_2 &= 0xFB;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB2 � LOW (bit 2 � 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 7 ==//
void activate_bonus_7_blower() 
{     
    bonus_activator();
    stateGpiob_2 |= 0x02; 
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB1 � HIGH (bit 1 � 1)
}

//== D�SACTIVER BONUS 7 ==//
void deactivate_bonus_7_blower()
{
    bonus_activator();
    stateGpiob_2 &= 0xFD;
    mcp23017_write_2(GPIOB_2, stateGpiob_2);                                    // Mettre GPB1 � LOW (bit 1 � 0) 
}

////////////////////////////////////////////////////////////////////////////////

//== ACTIVER BONUS 8 ==//
void activate_bonus_8_startBlower()
{     
    bonus_activator();
    stateGpioa_2 |= 0x80; 
    mcp23017_write_2(GPIOA_2, stateGpioa_2);                                    // Mettre GPB0 � HIGH (bit 0 � 1)
}

//== D�SACTIVER BONUS 8 ==//
void deactivate_bonus_8_startBlower()
{
    bonus_activator();
    stateGpioa_2 &= 0x7F;
    mcp23017_write_2(GPIOA_2, stateGpioa_2);                                    // Mettre GPB0 � LOW (bit 0 � 0) 
}