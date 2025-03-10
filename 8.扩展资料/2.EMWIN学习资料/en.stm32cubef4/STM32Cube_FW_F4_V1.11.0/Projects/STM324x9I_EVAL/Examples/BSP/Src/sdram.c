/**
  ******************************************************************************
  * @file    BSP/Src/sdram.c 
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   This example code shows how to use the SDRAM Driver
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

/** @addtogroup BSP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE            ((uint32_t)0x0100)
#define WRITE_READ_ADDR        ((uint32_t)0x0800)
#define SDRAM_WRITE_READ_ADDR  ((uint32_t)0xC0130000)
    
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t sdram_aTxBuffer[BUFFER_SIZE];
uint32_t sdram_aRxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void SDRAM_SetHint(void);
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset);
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  SDRAM Demo
  * @param  None
  * @retval None
  */
void SDRAM_demo(void)
{ 
  
  SDRAM_SetHint();
  
  /* SDRAM device configuration */ 
  if(BSP_SDRAM_Init() != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t *)"SDRAM Initialization : OK.", LEFT_MODE);
  }
  /* Fill the buffer to write */
  Fill_Buffer(sdram_aTxBuffer, BUFFER_SIZE, 0xA244250F);   
  
  /* Write data to the SDRAM memory */
  if(BSP_SDRAM_WriteData(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR, sdram_aTxBuffer, BUFFER_SIZE) != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  { 
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t *)"SDRAM WRITE : OK.", LEFT_MODE);
  }
  
  /* Read back data from the SDRAM memory */
  if(BSP_SDRAM_ReadData(SDRAM_WRITE_READ_ADDR + WRITE_READ_ADDR, sdram_aRxBuffer, BUFFER_SIZE) != SDRAM_OK)
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 130, (uint8_t *)"SDRAM READ : OK.", LEFT_MODE);
  }
  
  if(Buffercmp(sdram_aTxBuffer, sdram_aRxBuffer, BUFFER_SIZE) > 0)
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM COMPARE : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t *)"SDRAM Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t *)"SDRAM Test : OK.", LEFT_MODE);
  }
  
  while (1)
  {    
    if(CheckForUserInput() > 0)
    {
      return;
    }
  }
}

/**
  * @brief  Display SDRAM Demo Hint
  * @param  None
  * @retval None
  */
static void SDRAM_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"SDRAM", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"and read data on SDRAM", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
  
  BSP_LCD_SetTextColor(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
}

/**
  * @brief  Fills buffer with user predefined data.
  * @param  pBuffer: pointer on the buffer to fill
  * @param  uwBufferLenght: size of the buffer to fill
  * @param  uwOffset: first value to fill on the buffer
  * @retval None
  */
static void Fill_Buffer(uint32_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
{
  uint32_t tmpIndex = 0;
  
  /* Put in global buffer different values */
  for (tmpIndex = 0; tmpIndex < uwBufferLenght; tmpIndex++ )
  {
    pBuffer[tmpIndex] = tmpIndex + uwOffset;
  }
}

/**
  * @brief  Compares two buffers.
  * @param  pBuffer1, pBuffer2: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval 1: pBuffer identical to pBuffer1
  *         0: pBuffer differs from pBuffer1
  */
static uint8_t Buffercmp(uint32_t* pBuffer1, uint32_t* pBuffer2, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer1 != *pBuffer2)
    {
      return 1;
    }
    
    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}

/**
  * @}
  */ 

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
