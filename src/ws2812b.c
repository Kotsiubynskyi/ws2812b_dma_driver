#include "ws2812b.h"
#include <stdlib.h>

static uint32_t pixels_data[PIXELS_AMOUNT];
static uint8_t pixels_dma_data[2*COLOR_BITS];

uint8_t *ws2812b_init()
{
    for (uint16_t i = 0; i < 2*COLOR_BITS; i++)
    {
        pixels_dma_data[i] = 1;
    }

    return pixels_dma_data;
}

void ws2812b_clear_all()
{
    for (uint16_t i = 0; i < PIXELS_AMOUNT; i++)
    {
        ws2812b_clear_pixel(i);
    }
}

void ws2812b_clear_pixel(uint16_t index)
{
    if (index >= PIXELS_AMOUNT)
    {
        return;
    }

    pixels_data[index] = 0;
}

void ws2812b_set_pixel(uint16_t index, uint32_t rgbColor)
{
    if (index >= PIXELS_AMOUNT)
    {
        return;
    }

    uint32_t grb = convertRgbToGrb(rgbColor);

    // uint32_t val = (0x1 << 24) | grb;
    uint32_t val = grb;

    pixels_data[index] = val;

    // uint32_t mask = 0xfffffe;
    // for (uint16_t i = 0; i < COLOR_BITS; i++)
    // {
    //     uint8_t bitOn = (grb >> (COLOR_BITS - 1 - i) & ~(mask));
    //     pixels_data[COLOR_BITS * index + i] = (bitOn == 1) ? 3 : 1;
    // }
}

uint32_t mask = 0xfffffe;
uint32_t counter = 0;
void ws2812b_dma_complete_callback()
{
    if (counter++ >= PIXELS_AMOUNT)
    {
        for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
        {
            uint32_t ind = 1 * COLOR_BITS + bit;
            pixels_dma_data[ind] = 0;
        }
        if (counter > (RESET_LEDS+PIXELS_AMOUNT))
        {
            counter = 0;
        }
        return;
    }

    uint32_t grb = pixels_data[(counter-1)];
    for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
    {
        uint8_t bitOn = (grb >> (COLOR_BITS - 1 - bit) & ~(mask));
        uint32_t ind = 1 * COLOR_BITS + bit;
        pixels_dma_data[ind] = (bitOn == 1) ? 3 : 1;
    }
}

void ws2812b_dma_half_callback()
{
    if (counter++ >= PIXELS_AMOUNT)
    {
        for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
        {
            uint32_t ind = 0 * COLOR_BITS + bit;
            pixels_dma_data[ind] = 0;
        }
        return;
    }

    uint32_t grb = pixels_data[(counter-1)];
    for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
    {
        uint8_t bitOn = (grb >> (COLOR_BITS - 1 - bit) & ~(mask));
        uint32_t ind = 0 * COLOR_BITS + bit;
        pixels_dma_data[ind] = (bitOn == 1) ? 3 : 1;
    }
}

void ws2812b_allOn(uint32_t rgbColor)
{
    for (uint16_t i = 0; i < PIXELS_AMOUNT; i++)
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