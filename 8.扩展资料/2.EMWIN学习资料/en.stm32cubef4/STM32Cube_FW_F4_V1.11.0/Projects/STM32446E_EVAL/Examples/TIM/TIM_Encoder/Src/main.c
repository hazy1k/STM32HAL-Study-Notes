/**
  ******************************************************************************
  * @file    TIM/TIM_Encoder/Src/main.c
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    29-January-2016
  * @brief   This example shows how to command 2 Timers as slaves (TIM3 & TIM4)
  *          using a Timer as master (TIM2)
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup TIM_Encoder
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define EMU_PERIOD        (((SystemCoreClock/4)/10000) - 1)
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t uwDirection = 0;

/* Timer handler declaration that emulates a quadrature encoder outputs */
TIM_HandleTypeDef    EmulatorHandle;

/* Timer handler declaration */
TIM_HandleTypeDef    Encoder_Handle;

/* Timer Output Compare Configuration Structure declaration */
TIM_OC_InitTypeDef sConfig;

/* Timer Encoder Configuration Structure declaration */
TIM_Encoder_InitTypeDef sEncoderConfig;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void Init_TIM_Emulator(TIM_HandleTypeDef* htim);
static void Emulate_Forward_Direction(TIM_HandleTypeDef* htim);
static void Emulate_Backward_Direction(TIM_HandleTypeDef* htim);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch
       - Systick timer is configured by default as source of time base, but user 
         can eventually implement his proper time base source (a general purpose 
         timer for example or other time source), keeping in mind that Time base 
         duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
         handled in milliseconds basis.
       - Set NVIC Group Priority to 4
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Initialize LED1 & LED3 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED3);

  /* Initialize TIM3 to emulate a quadrature encoder outputs */
  Init_TIM_Emulator(&EmulatorHandle);

  /* -1- Initialize TIM1 to handle the encoder sensor */
  /* Initialize TIM1 peripheral as follow:
       + Period = 65535
       + Prescaler = 0
       + ClockDivision = 0
       + Counter direction = Up
  */
  Encoder_Handle.Instance = TIM1;

  Encoder_Handle.Init.Period            = 65535;
  Encoder_Handle.Init.Prescaler         = 0;
  Encoder_Handle.Init.ClockDivision     = 0;
  Encoder_Handle.Init.CounterMode       = TIM_COUNTERMODE_UP;
  Encoder_Handle.Init.RepetitionCounter = 0;

  sEncoderConfig.EncoderMode        = TIM_ENCODERMODE_TI12;

  sEncoderConfig.IC1Polarity        = TIM_ICPOLARITY_RISING;
  sEncoderConfig.IC1Selection       = TIM_ICSELECTION_DIRECTTI;
  sEncoderConfig.IC1Prescaler       = TIM_ICPSC_DIV1;
  sEncoderConfig.IC1Filter          = 0;

  sEncoderConfig.IC2Polarity        = TIM_ICPOLARITY_RISING;
  sEncoderConfig.IC2Selection       = TIM_ICSELECTION_DIRECTTI;
  sEncoderConfig.IC2Prescaler       = TIM_ICPSC_DIV1;
  sEncoderConfig.IC2Filter          = 0;

  if(HAL_TIM_Encoder_Init(&Encoder_Handle, &sEncoderConfig) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Start the encoder interface */
  HAL_TIM_Encoder_Start(&Encoder_Handle, TIM_CHANNEL_ALL);

  /* Infinite loop */
  while (1)
  {
    /* Step 1: */
    /* Emulate a Forward direction */
    Emulate_Forward_Direction(&EmulatorHandle);
    /* Insert 1s delay */
    HAL_Delay(1000);
    /* Get the current direction */
    uwDirection = __HAL_TIM_DIRECTION_STATUS(&Encoder_Handle);

    /* Step 2: */
    /* Emulate a Backward direction */
    Emulate_Backward_Direction(&EmulatorHandle);
    /* Insert 1s delay */
    HAL_Delay(1000);
    /* Get the current direction */
    uwDirection = __HAL_TIM_DIRECTION_STATUS(&Encoder_Handle);
  }
}


/**
  * @brief  Initialize a Timer to emulate an encoder sensor
  * @param  htim : TIM handle
  * @retval None
  */
static void Init_TIM_Emulator(TIM_HandleTypeDef* htim)
{
  /* Initialize TIM3 peripheral as follow:
       + Prescaler         = 0
       + Period            = 65535
       + ClockDivision     = 0
       + Counter direction = Up
  */
  htim->Instance = TIM3;

  htim->Init.Period        = EMU_PERIOD;
  htim->Init.Prescaler     = 0;
  htim->Init.ClockDivision = 0;
  htim->Init.CounterMode   = TIM_COUNTERMODE_UP;
  if(HAL_TIM_OC_Init(htim) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /*## Configure the Output Compare channels #########################################*/
  /* Output Compare Toggle Mode configuration: Channel1 */
  sConfig.OCMode = TIM_OCMODE_TOGGLE;
  sConfig.Pulse = (EMU_PERIOD * 1 )/4;
  sConfig.OCPolarity = TIM_OCPOLARITY_LOW;
  if(HAL_TIM_OC_ConfigChannel(htim, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /* Output Compare Toggle Mode configuration: Channel2 */
  sConfig.Pulse = (EMU_PERIOD * 3 )/4;
  if(HAL_TIM_OC_ConfigChannel(htim, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
}

/**
  * @brief  Configures a Timer to emulate an encoder sensor outputs in Forward
  *         direction
  * @param  htim : TIM handle
  * @retval None
  */
static void Emulate_Forward_Direction(TIM_HandleTypeDef* htim)
{
  /*## -1- Re-Configure the Pulse  ########################################## */
  sConfig.Pulse = (EMU_PERIOD * 1 )/4;
  if(HAL_TIM_OC_ConfigChannel(htim, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }
  sConfig.Pulse = (EMU_PERIOD * 3 )/4;
  if(HAL_TIM_OC_ConfigChannel(htim, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*## -2- Start signals generation ######################################### */
  if(HAL_TIM_OC_Start(htim, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  if(HAL_TIM_OC_Start(htim, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
}

/**
  * @brief  Configures a Timer to emulate an encoder sensor outputs in Backward
  *         direction
  * @param  htim : TIM handle
  * @retval None
  */
static void Emulate_Backward_Direction(TIM_HandleTypeDef* htim)
{
  /*## -1- Re-Configure the Pulse  ########################################## */
  sConfig.Pulse = (EMU_PERIOD * 3 )/4;
  if(HAL_TIM_OC_ConfigChannel(htim, &sConfig, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  sConfig.Pulse = (EMU_PERIOD * 1 )/4;
  if(HAL_TIM_OC_ConfigChannel(htim, &sConfig, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Configuration Error */
    Error_Handler();
  }

  /*## -2- Start signals generation ######################################### */
  if(HAL_TIM_OC_Start(htim, TIM_CHANNEL_1) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }
  if(HAL_TIM_OC_Start(htim, TIM_CHANNEL_2) != HAL_OK)
  {
    /* Starting Error */
    Error_Handler();
  }

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 8
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 2;
  
  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Activate the OverDrive to reach the 180 MHz Frequency */  
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  
  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
