#include "I2C.h"

//== FONCTION D'�CRITURE I2C POUR MCP23017 ==//
void mcp23017_write(unsigned char reg, unsigned char value) 
{
    i2c_start();                                                                // D�marrer la communication I2C
    i2c_write(MCP23017_ADDRESS);                                                // Envoi de l'adresse I2C du MCP23017
    i2c_write(reg);                                                             // S�lection du registre � �crire
    i2c_write(value);                                                           // Envoi des donn�es
    i2c_stop();                                                                 // Arr�ter la communication I2C
}

void setup_mcp23017()
{
    mcp23017_write(IODIRA, 0x00);                                               // Configurer toutes les broches du port A (GPA0 � GPA7) comme sorties
    mcp23017_write(IODIRB, 0x01);                                               // Configurer toutes les broches du port B (GPB0 � GPB7) comme sorties 
}

//== ACTIVER GPB7 ==//
void set_gpb7_high() 
{     
    setup_mcp23017();
    mcp23017_write(GPIOB, 0b11100000);                                          // Mettre GPB7 � HIGH (bit 7 � 1)
}

//== D�SACTIVER GPB7 ==//
void set_gpb7_low()
{
    setup_mcp23017();
    mcp23017_write(GPIOB, 0b00000000);                                          // Mettre GPB7 � LOW (bit 7 � 0) 
}