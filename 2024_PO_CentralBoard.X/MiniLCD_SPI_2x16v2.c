/********************************************************************\
* Nom du fichier    : LCD_SPI_2x16.c
*
* Auteur            : Claude Fahrni
* Descripion        : Driver pour LCD via SPI sur EXPLORER8
*********************************************************************
* Creation du projet: 13.07.2021
* Date MAJ.         : -
*
* Version           :   1.0
* Historique        :
*********************************************************************
* Editeur Version   :   MPLAB X 5.45
* Compileur Version :   CCS 5
\********************************************************************/
#define CURIOHPC1 //EXP8_J32 EXP8_J32, CURIO, CURIOHPC1, CURIOHPC2

#ifdef EXP8_J35
#define CS           PIN_A4   //A4 pour J35 ou E2 pour J32
#define CS2          PIN_A1   //A1 pour J35 ou E0 pour J32
#define RS           PIN_B7   // B7 pour J35 ou D7 pour J32
#define SPI_DO       PIN_C7   //D4 pour J32 ou C7 J35
#define SPI_DI       PIN_B4  //D5 pour J32 ou B4 J35
#define SPI_CLK      PIN_B6  //D6 pour J32 ou B6  J35
#endif 

#ifdef EXP8_J32
#define CS           PIN_E2   //A4 pour J35 ou E2 pour J32
#define CS2          PIN_E0   //A1 pour J35 ou E0 pour J32
#define RS           PIN_D7   // B7 pour J35 ou D7 pour J32
#define SPI_DO       PIN_D4   //D4 pour J32 ou C7 J35
#define SPI_DI       PIN_D5  //D5 pour J32 ou B4 J35
#define SPI_CLK      PIN_D6  //D6 pour J32 ou B6  J35
#endif 

#ifdef CURIOHPC1
#define CS           PIN_A3   
#define CS2          PIN_A1
#define RS           PIN_D0
#define SPI_DO       PIN_B3
#define SPI_DI       PIN_B2
#define SPI_CLK      PIN_B1
#endif 
//#use SPI(MASTER, DO=PIN_B4, DI=PIN_C7, CLK=PIN_B6, BITS=8, Mode=0)
#use SPI (MASTER, DO = SPI_DO , DI = SPI_DI , CLK = SPI_CLK , BITS = 8, Mode = 0)


#define IO_DEVICE_ADDRESS_WRITE   0x40

#define IODIRA        0x00      //Data Direction Register for PORTA
#define IODIRB        0x01      //Data Direction Register for PORTB
#define IPOLA         0x02      //Input Polarity Register for PORTA
#define IPOLB         0x03      //Input Polarity Register for PORTB
#define GPINTENA      0x04      //Interrupt-on-change enable Register for PORTA
#define GPINTENB      0x05      //Interrupt-on-change enable Register for PORTB
#define DEFVALA       0x06      //Default Value Register for PORTA
#define DEFVALB       0x07      //Default Value Register for PORTB
#define INTCONA       0x08      //Interrupt-on-change control Register for PORTA
#define INTCONB       0x09      //Interrupt-on-change control Register for PORTB
#define IOCON         0x0A      //Configuration register for device
#define GPPUA         0x0C      //100kOhm pullup resistor register for PORTA (sets pin to input when set)
#define GPPUB         0x0D      //100kOhm pullup resistor register for PORTB (sets pin to input when set)
#define INTFA         0x0E      //Interrupt flag Register for PORTA
#define INTFB         0x0F      //Interrupt flag Register for PORTB
#define INTCAPA       0x10      //Interrupt captured value Register for PORTA
#define INTCAPB       0x11      //Interrupt captured value Register for PORTB
#define GPIOA         0x12      //General purpose I/O Register for PORTA
#define GPIOB         0x13      //General purpose I/O Register for PORTB
#define OLATA         0x14      //Output latch Register for PORTA
#define OLATB         0x15      //Output latch Register for PORTB

void lcd_sendNibble(int nibble, int RSbit);
/**
 * Envoie une valeur à un registre du MCP23S17.
 * Suite de trois envois sur SPI: mode écriture - adresse - valeur
 * @param adresse adresse du registre
 * @param valeur valeur à transmettre
 */
void MCP23S17_write(unsigned char adresse, unsigned char valeur)
{
    output_low(CS);
  //  spi_xfer(IO_DEVICE_ADDRESS_WRITE);
    spi_xfer(0b01000000);
    spi_xfer(adresse);
    spi_xfer(valeur);
    output_high(CS);
}

/**
 * Initialisation du MCP23S17
 */
void setup_MCP23S17() {
    // séquence d'initialisation du circuit
    output_high(CS);
    delay_us(50);
    output_low(RS);
    delay_us(50);
    output_high(RS);
    // Port A et B en sortie
    MCP23S17_write(IODIRA, 0x00); // port A en sortie
    MCP23S17_write(IODIRB, 0x00); // port B en sortie
}

/**
 * Transmission d'une instruction ou d'une donnée sur LCD via le IO Expander
 * @param type 0 pour instruction, 1 pour donnée
 * @param valeur octet à transmettre
 **/
/*
void lcd_send_byte(int type, int valeur) {
    if (!type) {
        MCP23S17_write(GPIOB, 0x00); // RS=0, E=0
    } else {
        MCP23S17_write(GPIOB, 0x04); // RS=1, E=0
    }
    
    
    if (!type) {
        delay_us(100);
    MCP23S17_write(GPIOB, (valeur & 0xF0)+0x08);
    delay_us(100);
    MCP23S17_write(GPIOB, (valeur<<4 & 0xF0)+0x08);
    delay_us(10);
       // MCP23S17_write(GPIOB, 0x08); // RS=0, E=1
    } else {
        delay_us(100);
    MCP23S17_write(GPIOB, (valeur & 0xF0)+0x0F);
    delay_us(100);
    MCP23S17_write(GPIOB, (valeur<<4 & 0xF0)+0x0F);
    delay_us(10);
       // MCP23S17_write(GPIOB, 0x0F); // RS=1, E=1
    }
    delay_us(20);
    MCP23S17_write(GPIOB, 0x00);
}
*/
void lcd_send_byte(int type, int valeur) {
    int valeurHigh = valeur >>4;
    int valeurLow = valeur & 0xF;
    int packetHigh = (valeurHigh << 4) | (type << 2);
    int packetLow = (valeurLow << 4) | (type << 2);
    
    MCP23S17_write(OLATB, packetHigh);
    delay_ms(2);
    MCP23S17_write(OLATB, packetHigh | (1<<3));
     delay_ms(2);
    MCP23S17_write(OLATB, packetLow);
    delay_ms(2);
    MCP23S17_write(OLATB, packetLow | (1<<3)); 
    delay_ms(2); 
     
}

/**
 * Initialisation du LCD et du IO Expander qui le contrôle
 */
void lcd_init() {
        delay_ms(55);
    output_high(CS);
    
    output_low(CS2); 
    spi_xfer(0);
    spi_xfer(60);
    delay_ms(2);
    output_high(CS2);//
    delay_ms(100);
delay_ms(55);
    // initialiser le OI Expander
    setup_MCP23S17();
    // séquence d'initialisation
    
    MCP23S17_write(GPIOB, 0x00);
    delay_ms(40);
    lcd_send_byte(0,0b11); //0011NFxx
    delay_ms(10);
    lcd_send_byte(0,0b11); //0011NFxx
    delay_ms(10);
    lcd_send_byte(0,0b11); //0011NFxx
    delay_ms(10);
    lcd_send_byte(0,0x0C); // écran on, curseur off
    delay_us(50);
    lcd_send_byte(0,0x01); // effacer écran
    delay_us(50);
    lcd_send_byte(0,0x06); // mode incrément
    delay_ms(2);
    
  //      LATCbits.LATC6 = 1; /* set LCDMini_nCS output high */
  //  LATCbits.LATC2 = 1; /* set LCDMini_nCS2 output high */
   // LATAbits.LATA4 = 1; /* set LCDMini_nReset output high */
  //  expander_setup();
  //  expander_setOutput(0);
   // DELAY_milliseconds(40);
  
    lcd_sendNibble(0x2,0);
    lcd_send_byte(0,0x2C);
    lcd_send_byte(0,0b1100);
    lcd_send_byte(0,0x06);
    lcd_send_byte(0,0x0C);
    delay_ms(2);
    lcd_send_byte(0,0b10);
    delay_ms(2);
     lcd_send_byte(0,0b1);
    delay_ms(2); 
    

    
}

void lcd_sendNibble(int nibble, int RSbit){
    int packet = (nibble << 4) | (RSbit << 2);
    MCP23S17_write(OLATB,packet);
    MCP23S17_write(OLATB,packet | (1<<3));
    delay_ms(1);
    MCP23S17_write(OLATB,packet);
    delay_ms(2);
}


/**
 * Envoi un caractère au LCD
 * @param c le caratère à envoyer (\f, \n, \b supportés)
 */
void lcd_putc(char c)
{
 switch(c)
   {
    case '\f':
      // efface l'écran
      lcd_send_byte(0,1);
      delay_ms(2);
      break;
    case '\n':
       // Retour à la ligne
       lcd_send_byte(0,0xC0);
       break;
    case '\b':
       // retour en arrière
       lcd_send_byte(0,0x10);
       break;
    default:
       // tout autre caratère
       lcd_send_byte(1,c);
       break;
   }
}

/**
 * Positionne le curseur à un endroit bien précis sur le LCD
 * @param x numéro de colonne, commence à 1
 * @param y numéro de ligne, commence à 1
 */
void lcd_gotoxy(int8 colonne, int8 ligne) {
    int adresse;
    // calculer l'adresse selon la position donnée
    if (ligne != 1)
        adresse = 0x40;
    else
        adresse = 0;
    adresse += colonne - 1;
    // envoi de la commande
    lcd_send_byte(0, 0x80 | adresse);
}
