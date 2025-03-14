/**
  ******************************************************************************
  * @file    RTC/RTC_TimeStamp/Src/main.c 
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   This sample code shows how to use STM32F4xx RTC HAL API to configure 
  *          Time and Date.
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

/** @addtogroup RTC_TimeStamp
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
typedef struct {
  uint8_t tab[9];
} Table_TypeDef;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
RTC_HandleTypeDef RtcHandle;

#if !defined USE_LCD 
/* Buffers used for displaying Time and Date */
 uint8_t aShowTime[50] = {0}, aShowTimeStamp[50] = {0};
 uint8_t aShowDate[50] = {0}, aShowDateStamp[50] = {0};
#endif /* USE_LCD */
 
FlagStatus TamperStatus = RESET;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void RTC_TimeStampConfig(void);
static void RTC_CalendarShow(void);
#ifdef USE_LCD 
static Table_TypeDef RTC_Get_Time(RTC_TimeTypeDef* RTC_TimeStructure);
static void RTC_Time_display(uint8_t Line, uint32_t Color_x, Table_TypeDef table);
#endif /* USE_LCD */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{    
  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure LED1, LED2, LED3 and LED4 */
  BSP_LED_Init(LED1);
  BSP_LED_Init(LED2);
  BSP_LED_Init(LED3);
  BSP_LED_Init(LED4);

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();
  
  /* Turn LED1 on */
  BSP_LED_On(LED1);
  
#ifdef USE_LCD 
  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(1, LCD_FB_START_ADDRESS);
  BSP_LCD_SelectLayer(1);
    
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_ClearStringLine(0);
  BSP_LCD_DisplayStringAtLine(0, (uint8_t*)"       TimeStamp Example     ");
#endif /* USE_LCD */

  /*##-1- Configure the RTC peripheral #######################################*/
  RtcHandle.Instance = RTC;

  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 12
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_12;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }

  /*##-2- Configure RTC TimeStamp#############################################*/
  RTC_TimeStampConfig();
  
  /* Infinite loop */  
  while (1)
  {
    /*##-3- Display the updated Time and Date ################################*/
    RTC_CalendarShow();
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
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
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
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Activate the Over Drive feature (available only for STM32F42xxx/43xxx devices)*/
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* Turn LED3 on */
  BSP_LED_On(LED3);
  while(1)
  {
  }
}

/**
  * @brief  Configures the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_TimeStampConfig(void)
{
  RTC_DateTypeDef sdatestructure;
  RTC_TimeTypeDef stimestructure;
  RTC_TamperTypeDef stamperstructure;
        
  /*##-1- Configure the Tamper ###############################################*/
  /* Configure Tamper registers */
  /* RTC Tamper configured as follow:
      - Pamper = Tamper1
      - Pin selection = PC13
      - Trigger = Falling Edge
      - TimeStamp On Tamper Detection = Enable */ 
  stamperstructure.Filter =  RTC_TAMPERFILTER_DISABLE;
  stamperstructure.Tamper = RTC_TAMPER_1;
  stamperstructure.PinSelection = RTC_TAMPERPIN_PC13;
  stamperstructure.Trigger = RTC_TAMPERTRIGGER_FALLINGEDGE;
  stamperstructure.SamplingFrequency = RTC_TAMPERSAMPLINGFREQ_RTCCLK_DIV4096;
  stamperstructure.PrechargeDuration = RTC_TAMPERPRECHARGEDURATION_1RTCCLK;
  stamperstructure.TamperPullUp = RTC_TAMPER_PULLUP_DISABLE;
  stamperstructure.TimeStampOnTamperDetection = RTC_TIMESTAMPONTAMPERDETECTION_ENABLE;
  HAL_RTCEx_SetTamper(&RtcHandle, &stamperstructure);
  
  /*##-2- Configure the Time Stamp peripheral ################################*/
  /*  RTC TimeStamp flag Generation: TimeStamp Rising Edge on PC13 Pin */
  HAL_RTCEx_SetTimeStamp_IT(&RtcHandle, RTC_TIMESTAMPEDGE_RISING, RTC_TIMESTAMPPIN_PC13);
  
  /* Clear the TIMESTAMP interrupt pending bit */
  __HAL_RTC_TIMESTAMP_CLEAR_FLAG(&RtcHandle,RTC_FLAG_TSF);
  
  /*##-3- Configure the Date #################################################*/
  /* Set Date: Tuesday February 18th 2014 */
  sdatestructure.Year = 0x14;
  sdatestructure.Month = RTC_MONTH_FEBRUARY;
  sdatestructure.Date = 0x18;
  sdatestructure.WeekDay = RTC_WEEKDAY_TUESDAY;
  
  if(HAL_RTC_SetDate(&RtcHandle,&sdatestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  } 
  
  /*##-4- Configure the Time #################################################*/
  /* Set Time: 08:10:00 */
  stimestructure.Hours = 0x08;
  stimestructure.Minutes = 0x10;
  stimestructure.Seconds = 0x00;
  stimestructure.TimeFormat = RTC_HOURFORMAT12_AM;
  stimestructure.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  stimestructure.StoreOperation = RTC_STOREOPERATION_RESET;
  
  if(HAL_RTC_SetTime(&RtcHandle,&stimestructure,RTC_FORMAT_BCD) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler(); 
  }
}

/**
  * @brief  Tamper callback 
  * @param  hrtc: RTC handle
  * @retval None
  */
void HAL_RTCEx_TimeStampEventCallback(RTC_HandleTypeDef *hrtc)
{
  RTC_DateTypeDef sTimeStampDateget;
  RTC_TimeTypeDef sTimeStampget;
  
  /* Toggle LED4 */
  BSP_LED_Toggle(LED4);
  
#ifdef USE_LCD 
  HAL_RTCEx_GetTimeStamp(&RtcHandle, &sTimeStampget, &sTimeStampDateget, RTC_FORMAT_BCD);
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAtLine(8, (uint8_t *)"TimeStamp Display");
  /* Display Time Stamp */
  RTC_Time_display(9, LCD_COLOR_BLACK , RTC_Get_Time(&sTimeStampget));
#else
    
  HAL_RTCEx_GetTimeStamp(&RtcHandle, &sTimeStampget, &sTimeStampDateget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char*)aShowTimeStamp,"%0.2d:%0.2d:%0.2d", sTimeStampget.Hours, sTimeStampget.Minutes, sTimeStampget.Seconds);
  /* Display date Format : mm-dd */
  sprintf((char*)aShowDateStamp,"%0.2d-%0.2d-%0.2d", sTimeStampDateget.Month, sTimeStampDateget.Date, 2013); 
#endif /* USE_LCD */
}

/**
  * @brief  Displays the current time and date.
  * @param  None
  * @retval None
  */
static void RTC_CalendarShow(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;
  
#ifdef USE_LCD   
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BCD);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BCD);
  /* Set the Back Color */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  /* Set the Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_DisplayStringAtLine(6, (uint8_t *)"Current Time Display");

  RTC_Time_display(7, LCD_COLOR_BLACK , RTC_Get_Time(&stimestructureget));
#else
  /* Get the RTC current Time */
  HAL_RTC_GetTime(&RtcHandle, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&RtcHandle, &sdatestructureget, RTC_FORMAT_BIN);
  /* Display time Format : hh:mm:ss */
  sprintf((char*)aShowTime,"%0.2d:%0.2d:%0.2d", stimestructureget.Hours, stimestructureget.Minutes, stimestructureget.Seconds);
  /* Display date Format : mm-dd-yy */
  sprintf((char*)aShowDate,"%0.2d-%0.2d-%0.2d", sdatestructureget.Month, sdatestructureget.Date, 2000 + sdatestructureget.Year); 
#endif /* USE_LCD */
} 

#ifdef USE_LCD 
/**
  * @brief  Returns the current time and sub second.
  * @param  Secondfraction: the sub second fraction.
  * @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
  *         contains the current time values. 
  * @retval table : return current time and sub second in a table form
  */
static Table_TypeDef RTC_Get_Time(RTC_TimeTypeDef* RTC_TimeStructure)
{
  Table_TypeDef table2;

  /* Fill the table2 fields with the current Time*/
  table2.tab[0] = (((uint8_t)(RTC_TimeStructure->Hours & 0xF0) >> 0x04) + 0x30);
  table2.tab[1] = (((uint8_t)(RTC_TimeStructure->Hours & 0x0F))+ 0x30);
  table2.tab[2] = 0x3A;
  
  table2.tab[3] = (((uint8_t)(RTC_TimeStructure->Minutes & 0xF0) >> 0x04) + 0x30);
  table2.tab[4] = (((uint8_t)(RTC_TimeStructure->Minutes & 0x0F))+ (uint8_t)0x30);
  table2.tab[5] = 0x3A;

  table2.tab[6] = (((uint8_t)(RTC_TimeStructure->Seconds & 0xF0) >> 0x04)+ 0x30);
  table2.tab[7] = (((uint8_t)(RTC_TimeStructure->Seconds & 0x0F)) + 0x30);
  table2.tab[8] = 0xA0;
  
  /* Return table2 */
  return table2;
}

/**
  * @brief  Displays the current Time on the LCD.
  * @param  Line:  the Line where to display the Current time .
  *           This parameter can be one of the following values:
  *             @arg Linex: where x can be 0..9
  * @param  Color_x: specifies the Background Color.
  * @param  table: the Current time and sub second.
  * @retval None
  */
static void RTC_Time_display(uint8_t Line, uint32_t Color_x, Table_TypeDef table )
{   
  uint8_t i = 0;

  /* Initialize table */
  BSP_LCD_SetTextColor(Color_x);
  
  for(i=0; i<8; i++)
  {
    /* Display char on the LCD */
    BSP_LCD_DisplayChar((150 + (20 *i)),LINE(Line), table.tab[i]);
  }  
}
#endif /* USE_LCD */

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
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
