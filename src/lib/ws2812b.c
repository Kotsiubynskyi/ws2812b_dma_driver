#include "ws2812b.h"

#define CLEAR_RG_MASK ~(0x00ff0000 | 0x0000ff00)

static uint32_t pixels_data[PIXELS_AMOUNT];
static uint8_t pixels_dma_data[2 * COLOR_BITS];

uint32_t *ws2812b_init()
{
    for (uint16_t i = 0; i < 2 * COLOR_BITS; i++)
    {
        pixels_dma_data[i] = 1;
    }

    return (uint32_t *)pixels_dma_data;
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
    pixels_data[index] = grb;
}

const uint32_t mask = 0xfffffe;
static uint32_t counter = 0;
void ws2812b_dma_complete_callback()
{
    if (counter > PIXELS_AMOUNT - 1)
    {
        for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
        {
            uint32_t ind = COLOR_BITS + bit;
            pixels_dma_data[ind] = 0;
        }
        if (counter > (RESET_LEDS + PIXELS_AMOUNT))
        {
            counter = 0;
        }
    }
    else
    {
        uint32_t grb = pixels_data[(counter)];
        for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
        {
            uint8_t bitOn = (grb >> (COLOR_BITS - 1 - bit) & ~(mask));
            uint32_t ind = COLOR_BITS + bit;
            pixels_dma_data[ind] = (bitOn == 1) ? 3 : 1;
        }
    }
    counter++;
}

void ws2812b_dma_half_callback()
{
    if (counter > PIXELS_AMOUNT - 1)
    {
        for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
        {
            pixels_dma_data[bit] = 0;
        }
    }
    else
    {
        uint32_t grb = pixels_data[(counter)];
        for (uint16_t bit = 0; bit < COLOR_BITS; bit++)
        {
            uint8_t bitOn = (grb >> (COLOR_BITS - 1 - bit) & ~(mask));
            pixels_dma_data[bit] = (bitOn == 1) ? 3 : 1;
        }
    }
    counter++;
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
    uint8_t green = (rgb >> 8) & 0xFF;
    uint8_t red = (rgb >> 16) & 0xFF;

    return (rgb & CLEAR_RG_MASK) | (red << 8) | (green << 16);
}