/**
  ******************************************************************************
  * @file    I2C/I2C_EEPROM/Inc/main.h
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    29-January-2016
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include "stm32446e_eval.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* User can use this section to tailor I2Cx/I2Cx instance used and associated
   resources */
/* Definition for I2Cx clock resources */
#define I2Cx                             FMPI2C1
#define I2Cx_CLK_ENABLE()                __HAL_RCC_FMPI2C1_CLK_ENABLE()
#define I2Cx_DMA_CLK_ENABLE()            __HAL_RCC_DMA1_CLK_ENABLE()
#define I2Cx_SDA_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()
#define I2Cx_SCL_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOD_CLK_ENABLE()

#define I2Cx_FORCE_RESET()               __HAL_RCC_FMPI2C1_FORCE_RESET()
#define I2Cx_RELEASE_RESET()             __HAL_RCC_FMPI2C1_RELEASE_RESET()

/* Definition for I2Cx Pins */
#define I2Cx_SCL_PIN                    GPIO_PIN_12
#define I2Cx_SCL_GPIO_PORT              GPIOD
#define I2Cx_SCL_AF                     GPIO_AF4_FMPI2C1
#define I2Cx_SDA_PIN                    GPIO_PIN_13
#define I2Cx_SDA_GPIO_PORT              GPIOD
#define I2Cx_SDA_AF                     GPIO_AF4_FMPI2C1

/* Definition for I2Cx's DMA */
#define I2Cx_TX_DMA_CHANNEL             DMA_CHANNEL_2
#define I2Cx_RX_DMA_CHANNEL             DMA_CHANNEL_2
#define I2Cx_DMA_INSTANCE_TX            DMA1_Stream5
#define I2Cx_DMA_INSTANCE_RX            DMA1_Stream2

/* Definition for I2Cx's NVIC */
#define I2Cx_DMA_TX_IRQn                DMA1_Stream5_IRQn
#define I2Cx_DMA_RX_IRQn                DMA1_Stream2_IRQn
#define I2Cx_DMA_TX_IRQHandler          DMA1_Stream5_IRQHandler
#define I2Cx_DMA_RX_IRQHandler          DMA1_Stream2_IRQHandler

/* Size of Transmission buffer */
#define TXBUFFERSIZE                      (COUNTOF(aTxBuffer) - 1)
/* Size of Reception buffer */
#define RXBUFFERSIZE                      TXBUFFERSIZE

/* Exported macro ------------------------------------------------------------*/
#define COUNTOF(__BUFFER__)   (sizeof(__BUFFER__) / sizeof(*(__BUFFER__)))
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
