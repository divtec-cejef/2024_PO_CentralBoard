#ifndef WS2812_H
#define WS2812_H

#include <stdint.h>

// Configuration des LEDs
#define WS2812_LED_COUNT 24

#define WS2812_PIN PIN_C2

// Fonctions
void ws2812_init(void);
void ws2812_sendBit(uint8_t bit);
void ws2812_sendByte(uint8_t val);
void ws2812_sendColor(uint8_t re, uint8_t gr, uint8_t bl);
void ws2812_showAll(uint8_t* r_array, uint8_t* g_array, uint8_t* b_array, uint8_t count);
void ws2812_wheel(uint8_t pos, uint8_t* re, uint8_t* gr, uint8_t* bl);

#endif

