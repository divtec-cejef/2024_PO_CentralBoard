#include "I2C.h"

//== FONCTION D'ÉCRITURE I2C POUR MCP23017 ==//
void mcp23017_write(unsigned char reg, unsigned char value) 
{
    i2c_start();                                                                // Démarrer la communication I2C
    i2c_write(MCP23017_ADDRESS);                                                // Envoi de l'adresse I2C du MCP23017
    i2c_write(reg);                                                             // Sélection du registre à écrire
    i2c_write(value);                                                           // Envoi des données
    i2c_stop();                                                                 // Arrêter la communication I2C
}

void setup_mcp23017()
{
    mcp23017_write(IODIRA, 0x00);                                               // Configurer toutes les broches du port A (GPA0 à GPA7) comme sorties
    mcp23017_write(IODIRB, 0x01);                                               // Configurer toutes les broches du port B (GPB0 à GPB7) comme sorties 
}

//== ACTIVER GPB7 ==//
void set_gpb7_high() 
{     
    setup_mcp23017();
    mcp23017_write(GPIOB, 0b11100000);                                          // Mettre GPB7 à HIGH (bit 7 à 1)
}

//== DÉSACTIVER GPB7 ==//
void set_gpb7_low()
{
    setup_mcp23017();
    mcp23017_write(GPIOB, 0b00000000);                                          // Mettre GPB7 à LOW (bit 7 à 0) 
}