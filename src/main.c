#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_tim.h"
#include "stm32g0xx_hal_rcc.h"
#include "stm32g0xx_hal_dma.h"
#include "ws2812b.h"

void GPIO_PA6_Init();
void SystemClock_Config();
void startWledTimer(uint8_t *ledData);

#define PIXELS

int main()
{
  HAL_Init();
  SystemClock_Config();
  
  GPIO_PA6_Init();
  
  uint8_t *arr = ws2812b_init();
  startWledTimer(arr);
  ws2812b_clear_all();
  while (1)
  {
      // ws2812b_clear_pixel(0);
      // HAL_Delay(600);
      ws2812b_set_pixel(0, 0xff0000);
      ws2812b_set_pixel(2, 0x00ff00);
      HAL_Delay(600);
  }

  return 0;
}


void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM3)
  {
  }
}


void GPIO_PA6_Init()
{
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = GPIO_AF1_TIM3;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

TIM_HandleTypeDef hTim3;

void startWledTimer(uint8_t *ledData)
{
  __HAL_RCC_TIM3_CLK_ENABLE();

  hTim3.Instance = TIM3;
  hTim3.Init.Prescaler = 16 - 1;
  hTim3.Init.Period = 4 - 1;
  hTim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  hTim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&hTim3);
  HAL_TIM_PWM_Init(&hTim3);

  TIM_OC_InitTypeDef sConfigOC = {0};
  sConfigOC.OCMode = TIM_OCMODE_PWM1;

  HAL_TIM_PWM_ConfigChannel(&hTim3, &sConfigOC, TIM_CHANNEL_1);

  HAL_TIM_PWM_Start_DMA(&hTim3, TIM_CHANNEL_1, (uint32_t *)ledData, PIXELS_DATA_SIZE);
}

DMA_HandleTypeDef hdma_tim3_ch1;

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim_base)
{
  if (htim_base->Instance == TIM3)
  {
    __HAL_RCC_DMA1_CLK_ENABLE();

    hdma_tim3_ch1.Instance = DMA1_Channel1;
    hdma_tim3_ch1.Init.Request = DMA_REQUEST_TIM3_CH1;
    hdma_tim3_ch1.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_tim3_ch1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_tim3_ch1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_tim3_ch1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_tim3_ch1.Init.MemDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_tim3_ch1.Init.Mode = DMA_CIRCULAR;
    // hdma_tim3_ch1.XferHalfCpltCallback = HAL_DMA_XferHalfCpltCallback;

    HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
    HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
    HAL_DMA_Init(&hdma_tim3_ch1);

    __HAL_LINKDMA(htim_base, hdma[TIM_DMA_ID_CC1], hdma_tim3_ch1);
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  // Initializes the CPU, AHB and APB buses clocks
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

void DMA1_Channel1_IRQHandler(void)
{
  HAL_DMA_IRQHandler(&hdma_tim3_ch1);
}
