#include "ws2812b.h"

#include "ws2812b_animations.h"

static uint32_t wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        return ((uint32_t)(255 - pos * 3) << 16) | ((uint32_t)(0) << 8) | (pos * 3);
    }
    if (pos < 170) {
        pos -= 85;
        return ((uint32_t)(0) << 16) | ((uint32_t)(pos * 3) << 8) | (255 - pos * 3);
    }
    pos -= 170;
    return ((uint32_t)(pos * 3) << 16) | ((uint32_t)(255 - pos * 3) << 8) | (0);
}

void rainbow(void) {
    uint8_t j = 0;
    while (1) {
        for (uint32_t i = 0; i < PIXELS_AMOUNT; i++) {
            ws2812b_set_pixel(i, wheel(((i * 256 / PIXELS_AMOUNT) + j) & 255));
        }
        HAL_Delay(20);
        j++;
        if (j >= 256) j = 0;
    }
}
