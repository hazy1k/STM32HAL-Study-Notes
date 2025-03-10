/**
  ******************************************************************************
  * @file    startup.c
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   <Add here what does this driver do>
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT YYYY STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/** @addtogroup MAIN_GROUP< OPTIONAL >
  * @{
  */

/** @defgroup SUB_GROUP< OPTIONAL >
  * @brief 
  * @{
  */

/* External variables --------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0x00FF /* LSE as RTC clock */
/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
RTC_HandleTypeDef RtcHandle;
/**
  * @brief  Configure the current time and date.
  * @param  None
  * @retval None
  */
void RTC_Init(void)
{
  
  /*##-1- Configure the RTC peripheral #######################################*/
  /* Configure RTC prescaler and RTC data registers */
  /* RTC configured as follow:
  - Hour Format    = Format 24
  - Asynch Prediv  = Value according to source clock
  - Synch Prediv   = Value according to source clock
  - OutPut         = Output Disable
  - OutPutPolarity = High Polarity
  - OutPutType     = Open Drain */ 
  RtcHandle.Instance = RTC;
  RtcHandle.Init.HourFormat = RTC_HOURFORMAT_24;
  RtcHandle.Init.AsynchPrediv = RTC_ASYNCH_PREDIV;
  RtcHandle.Init.SynchPrediv = RTC_SYNCH_PREDIV;
  RtcHandle.Init.OutPut = RTC_OUTPUT_DISABLE;
  RtcHandle.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
  RtcHandle.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
  
  if(HAL_RTC_Init(&RtcHandle) != HAL_OK)
  {
  }
}

/**
  * @brief RTC MSP Initialization 
  *        This function configures the hardware resources used in this application: 
  *           - Peripheral's clock enable
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
  RCC_OscInitTypeDef Osc;
  
  /*##-1- Configue LSE as RTC clock soucre ###################################*/ 
  Osc.OscillatorType = RCC_OSCILLATORTYPE_LSE;
  Osc.LSEState = RCC_LSE_ON;
  Osc.HSIState = RCC_HSI_OFF;
  Osc.HSICalibrationValue = 0;
  Osc.LSIState = RCC_LSI_OFF;
  Osc.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&Osc);
  
  __HAL_RCC_RTC_CLKPRESCALER(RCC_RTCCLKSOURCE_LSE);
  __HAL_RCC_RTC_CONFIG(RCC_RTCCLKSOURCE_LSE);
  
  /*##-2- Enable RTC peripheral Clocks #######################################*/ 
  /* Enable RTC Clock */ 
  __HAL_RCC_RTC_ENABLE(); 
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in this application:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
  /*##-1- Reset peripherals ##################################################*/
   __HAL_RCC_RTC_DISABLE();
}


/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in this application:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
void BACKUP_SaveParameter(uint32_t address, uint32_t data)
{
  HAL_RTCEx_BKUPWrite(&RtcHandle,address,data);  
}

/**
  * @brief RTC MSP De-Initialization 
  *        This function frees the hardware resources used in this application:
  *          - Disable the Peripheral's clock
  * @param hrtc: RTC handle pointer
  * @retval None
  */
uint32_t BACKUP_RestoreParameter(uint32_t address)
{
   return HAL_RTCEx_BKUPRead(&RtcHandle,address);  
}
/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
