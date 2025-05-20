This is just another STM32 HAL driver library for controling __WS2812B__ RGB LED.
Library features:
* low memory usage
* static memory allocation
* using DMA+Timer for protocol controling
* based on STM32 HAL for portability

Requirements:
Developer has to start Timer on 1 MHz speed by himself as well as DMA. Call **ws2812b_init** which returns array which hast be passed then to **HAL_TIM_PWM_Start_DMA**. That's all for start using library. 

Tested on ***STM32G030*** and ***STM32WB55***.

Project structure:
* Demo for STM32G030 - **src/main.c**
* Library itself:
* * Core - **src/lib/ws2812b.c**
* * Basic animations - **src/lib/ws2812b_animations.c**


## How to build and flash (for Ubuntu)

1. Install needed ARM development toolchain and other utilities:
    ```bash
    apt update; apt install -y cmake git usbutils ninja-build gcc-arm-none-eabi stlink-tools
    ```
1. Clone the project:
    ```bash
    git clone https://github.com/Kotsiubynskyi/ws2812b_dma_driver.git
    ```
1. Generate build files:
    ```bash
    cd ws2812b_dma_driver
    mkdir build
    cd build
    cmake .. -G Ninja
    ```
1. Build project from ***build*** folder:
    ```bash
    ninja
    ```
1. Insert ST-LINK programmer USB stick and make sure it's visible by OS:
    ```bash
	lsusb|grep STMicroelectronics
    ```
1. Upload built firmware to MCU:
    ```bash
    st-flash erase && st-flash --reset write wled.bin 0x8000000
    ```
