#include "ws2812.h"
#include <stdint.h>
//#use delay(clock=16000000)

void ws2812_init(void) 
{
    output_low(WS2812_PIN);
    output_drive(WS2812_PIN);
}

void ws2812_sendBit(uint8_t bit) 
{
    if (bit) 
    {
        output_high(WS2812_PIN);     // T1H
        delay_cycles(13);            // ? 812 ns
        output_low(WS2812_PIN);      // T1L
        delay_cycles(7);             // ? 437 ns
    } 
    else 
    {
        output_high(WS2812_PIN);     // T0H
        delay_cycles(6);             // ? 375 ns
        output_low(WS2812_PIN);      // T0L
        delay_cycles(14);            // ? 875 ns
    }
}

void ws2812_sendByte(uint8_t val) 
{
    for (int8_t i = 7; i >= 0; i--) 
    {
        ws2812_sendBit((val >> i) & 0x01);
    }
}

void ws2812_sendColor(uint8_t re, uint8_t gr, uint8_t bl) 
{
    ws2812_sendByte(gr);
    ws2812_sendByte(re);
    ws2812_sendByte(bl);
}

void ws2812_showAll(uint8_t* r_array, uint8_t* g_array, uint8_t* b_array, uint8_t count) 
{
    for (uint8_t i = 0; i < count; i++) 
    {
        ws2812_sendColor(r_array[i], g_array[i], b_array[i]);
    }
    delay_us(60); // Reset time
}

void ws2812_wheel(uint8_t pos, uint8_t* re, uint8_t* gr, uint8_t* bl) 
{
    if (pos < 85) 
    {
        *re = pos * 3;
        *gr = 255 - pos * 3;
        *bl = 0;
    } 
    else if (pos < 170) 
    {
        pos -= 85;
        *re = 255 - pos * 3;
        *gr = 0;
        *bl = pos * 3;
    } 
    else 
    {
        pos -= 170;
        *re = 0;
        *gr = pos * 3;
        *bl = 255 - pos * 3;
    }
}
