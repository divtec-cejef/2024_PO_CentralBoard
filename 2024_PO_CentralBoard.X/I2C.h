#ifndef I2C_H
#define	I2C_H

//== ADRESSE I2C DU MCP23017 (A0, A1, A2 --> GND = 0x20) ==//
#define MCP23017_ADDRESS 0x40                                                   // CCS utilise l'adresse décalée de 1 bit (7 bits à gauche)
#define IODIRA 0x00                                                             // Registre pour définir la direction des broches GPA
#define IODIRB 0x01                                                             // Registre pour définir la direction des broches GPA

#define GPIOA  0x12                                                             // Registre GPIO pour contrôler GPA
#define GPIOB  0x13                                                             // Registre GPIO pour contrôler GPA

#use i2c(Master, sda=PIN_C4, scl=PIN_C3, FAST=400000)                           // Configuration I2C rapide (400kHz)

void mcp23017_write(unsigned char reg, unsigned char value); 
void setup_mcp23017(); 
void set_gpb7_high(); 
void set_gpb7_low(); 

#endif	/* I2C_H */