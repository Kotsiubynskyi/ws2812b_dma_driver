#include "ws2812b.h"
#include <stdlib.h>

#define COLOR_BITS 24
#define RESET_LEDS 12

static uint8_t *pixels_data;
static uint16_t pixels_amount;

uint8_t* ws2812b_init(uint16_t pixels)
{
    pixels_amount = pixels;
    uint16_t arr_size = (pixels_amount + RESET_LEDS) * COLOR_BITS;
    pixels_data = (uint8_t *)calloc(arr_size, sizeof(uint8_t));

    for (uint16_t i = 0; i < pixels_amount; i++)
    {
        for (uint16_t k = 0; k < COLOR_BITS; k++)
        {
            pixels_data[COLOR_BITS * i + k] = 1;
        }
    }

    return pixels_data;
}

void ws2812b_clear_all()
{
    for (uint16_t i = 0; i < pixels_amount * COLOR_BITS; i++)
    {
        ws2812b_clear_pixel(i);
    }
}

void ws2812b_clear_pixel(uint16_t index)
{
    if (index >= pixels_amount)
    {
        return;
    }
    for (uint16_t i = 0; i < COLOR_BITS; i++)
    {
        pixels_data[COLOR_BITS * index + i] = 1;
    }
}

uint32_t mask = 0xfffffe;
void ws2812b_set_pixel(uint16_t index, uint32_t rgbColor)
{
    if (index >= pixels_amount)
    {
        return;
    }

    uint32_t grb = convertRgbToGrb(rgbColor);

    for (uint16_t i = 0; i < COLOR_BITS; i++)
    {
        uint8_t bitOn = (grb >> (COLOR_BITS - 1 - i) & ~(mask));
        pixels_data[COLOR_BITS * index + i] = (bitOn == 1) ? 3 : 1;
    }
}

void ws2812b_allOn(uint32_t rgbColor)
{
    for (uint16_t i = 0; i < pixels_amount; i++)
    {
        ws2812b_set_pixel(i, rgbColor);
    }
}

uint32_t convertRgbToGrb(uint32_t rgb)
{
    uint32_t clearRedGreenMask = ~(0x00ff0000 | 0x0000ff00);
    uint8_t green = (rgb >> 8) & 0xFF;
    uint8_t red = (rgb >> 16) & 0xFF;

    uint32_t grb = (rgb & clearRedGreenMask) | (red << 8) | (green << 16);
    return grb;
}