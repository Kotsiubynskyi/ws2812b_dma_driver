#ifndef WS2812B_H
#define WS2812B_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define COLOR_BITS 24
#define RESET_LEDS 12

typedef struct {
    uint16_t numLeds;
    uint8_t dataPin;
} ws2812b_config_t;

uint8_t* ws2812b_init(uint16_t pixels);
void ws2812b_clear_all(void);
void ws2812b_clear_pixel(uint16_t index);
void ws2812b_set_pixel(uint16_t index, uint32_t rgbColor);
void ws2812b_allOn(uint32_t rgbColor);
void ws2812b_cleanup(void);
uint32_t convertRgbToGrb(uint32_t rgb);

#ifdef __cplusplus
}
#endif

#endif // WS2812B_H