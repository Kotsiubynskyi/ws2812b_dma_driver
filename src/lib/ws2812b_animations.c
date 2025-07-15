#include "ws2812b.h"
#include "ws2812b_animations.h"

static uint32_t wheel(uint8_t pos)
{
    uint8_t red, green, blue;
    pos = 255 - pos;

    if (pos < 85)
    {
        red = 255 - pos * 3;
        green = 0;
        blue = pos * 3;
    }
    else if (pos < 170)
    {
        pos -= 85;
        red = 0;
        green = pos * 3;
        blue = 255 - pos * 3;
    }
    else
    {
        pos -= 170;
        red = pos * 3;
        green = 255 - pos * 3;
        blue = 0;
    }

    return (uint32_t)red << 16 | (uint32_t)green << 8 | blue;
}

void rainbow(void)
{

    uint8_t position = 0;
    while (1)
    {
        for (uint32_t pixel = 0; pixel < PIXELS_AMOUNT; pixel++)
        {
            uint8_t color_index = ((pixel * 256 / PIXELS_AMOUNT) + position) & 255;
            ws2812b_set_pixel(pixel, wheel(color_index));
        }
        delay(15);
        position = (position + 1) & 255;
    }
}
