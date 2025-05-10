This is just another STM32 HAL driver library for controlling __WS2812B__ RGB LED.

Library features:
* low memory usage
* static memory allocation
* using DMA+Timer for driving LEDs without MCU core
* based on STM32 HAL for portability

How to use:
Developer has to start Timer on 4 MHz speed by himself as well as DMA. Call **ws2812b_init** which returns an array which has be passed then to **HAL_TIM_PWM_Start_DMA**. That's all for start using library. See demo in **src/main.c**

Project structure:
* Demo for [STM32G030](https://github.com/Kotsiubynskyi/stm32g0_ws2812b_example) - **src/main.c**
* Library itself:
  * Core - **src/lib/ws2812b.c**
  * Basic animations - **src/lib/ws2812b_animations.c**

Tested on ***[STM32G030](https://github.com/Kotsiubynskyi/stm32g0_ws2812b_example)*** and ***[STM32WB55](https://github.com/Kotsiubynskyi/stm32wb55_wled)***.
