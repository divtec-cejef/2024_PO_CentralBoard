#ifndef I2C2_H
#define	I2C2_H

//== Adresse I2C du MCP23017 (A0, A1, A2 à GND = 0x20) ==//
#define MCP23017_ADDRESS_2 0x48                                                 // CCS utilise l'adresse décalée de 1 bit (7 bits à gauche)
#define IODIRA_2 0x00                                                           // Registre pour définir la direction des broches GPA
#define IODIRB_2 0x01                                                           // Registre pour définir la direction des broches GPA

#define GPIOA_2  0x12                                                           // Registre GPIO pour contrôler GPA
#define GPIOB_2  0x13                                                           // Registre GPIO pour contrôler GPA

#use i2c(Master, sda=PIN_C4, scl=PIN_C3, FAST=400000)                           // Configuration I2C rapide (400kHz)

int8 stateGpiob_2 = 0;

void mcp23017_write_2(unsigned char reg, unsigned char value); 
void bonus_activator(); 

void activate_bonus_1_lever(); 
void deactivate_bonus_1_lever(); 

void activate_bonus_2_lever(); 
void deactivate_bonus_2_lever();

void activate_bonus_3_lever(); 
void deactivate_bonus_3_lever();

void activate_bonus_4_elevator(); 
void deactivate_bonus_4_elevator();

void activate_bonus_5_elevator(); 
void deactivate_bonus_5_elevator();

void activate_bonus_6_blower(); 
void deactivate_bonus_6_blower();

void activate_bonus_7_blower(); 
void deactivate_bonus_7_blower();

void activate_bonus_8(); 
void deactivate_bonus_8();

#endif	/* I2C2_H */