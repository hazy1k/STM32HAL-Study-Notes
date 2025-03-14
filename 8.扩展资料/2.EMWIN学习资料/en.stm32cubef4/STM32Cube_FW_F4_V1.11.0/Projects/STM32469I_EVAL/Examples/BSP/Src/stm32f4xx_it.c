/**
  ******************************************************************************
  * @file    BSP/Src/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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
#include "stm32f4xx_it.h"


/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup BSP
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern DMA_HandleTypeDef   hdma;
extern QSPI_HandleTypeDef QSPIHandle;
/*DMA status declared in "sdram_dma.c" file */
extern uint32_t uwDMA_Transfer_Complete;
/* SAI handler declared in "stm32469i_eval_audio.c" file */
extern SAI_HandleTypeDef haudio_out_sai;
/* I2S handler declared in "stm32469ieval_audio.c" file */
extern I2S_HandleTypeDef haudio_in_i2s;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern void BSP_SDRAM_DMA_IRQHandler(void);
extern void BSP_SRAM_DMA_IRQHandler(void);

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
  HAL_IncTick();
  Toggle_Leds();
  /* Check periodically the buffer state and fill played buffer with new data
     following the state that has been updated by the BSP_AUDIO_OUT_TransferComplete_CallBack()
     and BSP_AUDIO_OUT_HalfTransfer_CallBack() */
  AUDIO_Process();
}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(WAKEUP_BUTTON_PIN);
}

/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
   HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}

/**
  * @brief  This function handles External line 9_5 interrupt request.
  * @param  None
  * @retval None
  */
void MFX_IRQOUT_EXTI_IRQnHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(MFX_IRQOUT_PIN);
}

/**
  * @brief  This function handles External line 15_10 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI15_10_IRQHandler(void)
{
  HAL_GPIO_EXTI_IRQHandler(TAMPER_BUTTON_PIN | KEY_BUTTON_PIN);
}

/**
  * @brief This function handles DMA1 Stream 2 interrupt request.
  * @param None
  * @retval None
  */
void AUDIO_I2Sx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_in_i2s.hdmarx);
}

/**
  * @brief  This function handles DMA2 Stream0 interrupt request.
  * @param  None
  * @retval None
  */
void SDRAM_DMAx_IRQHandler(void)
{
  /* Check the interrupt and clear flag */
  BSP_SDRAM_DMA_IRQHandler();
}

/**
  * @brief  DMA interrupt handler.
  * @param  None
  * @retval None
  */
void DMA2_Stream1_IRQHandler(void)
{
  BSP_CAMERA_DMA_IRQHandler();
}

/**
  * @brief  This function handles DMA2 Stream4 interrupt request.
  * @param  None
  * @retval None
  */
void SRAM_DMAx_IRQHandler(void)
{
  /* Check the interrupt and clear flag */
  BSP_SRAM_DMA_IRQHandler();
}
/**
  * @brief  This function handles DMA2 Stream 1 interrupt request.
  * @param  None
  * @retval None
  */
void AUDIO_SAIx_DMAx_IRQHandler(void)
{
  HAL_DMA_IRQHandler(haudio_out_sai.hdmatx);
}

/**
  * @brief  DCMI interrupt handler.
  * @param  None
  * @retval None
  */
void DCMI_IRQHandler(void)
{
  BSP_CAMERA_IRQHandler();
}

/**
  * @brief  This function handles DMA2D Handler.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  BSP_LCD_DMA2D_IRQHandler();
}

/**
  * @brief  This function handles LTDC Handler.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  BSP_LCD_LTDC_IRQHandler();
}

/**
  * @brief  This function handles LTDC Error Interrupt Handler.
  * @param  None
  * @retval None
  */

void LTDC_ER_IRQHandler(void)
{
  BSP_LCD_LTDC_ER_IRQHandler();
}

/**
  * @brief  This function handles DSI Handler.
  * @param  None
  * @retval None
  */
void DSI_IRQHandler(void)
{
  BSP_LCD_DSI_IRQHandler();
}


/**
  * @brief  This function handles QUADSPI interrupt request.
  * @param  None
  * @retval None
  */
void QuadSPI_IRQHandler(void)
{
  HAL_QSPI_IRQHandler(&QSPIHandle);
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
