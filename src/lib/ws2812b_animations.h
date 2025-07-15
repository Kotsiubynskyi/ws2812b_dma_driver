#ifndef WS2812B_ANIMATIONS_H
#define WS2812B_ANIMATIONS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "stdint.h"

    /**
     * Moving rainbow
     */
    void rainbow();

    /**
     * Single color glowing - fade in and out
     */
    void glowing(uint32_t rgbColor);

#ifdef __cplusplus
}
#endif

#endif // WS2812B_ANIMATIONS_H