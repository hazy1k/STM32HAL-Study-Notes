/**
******************************************************************************
* @file    BSP/Src/qspi.c 
* @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
* @brief   This example code shows how to use the QSPI Driver
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
/* External variables --------------------------------------------------------*/
extern QSPI_InfoTypeDef QspiInfo;

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define BUFFER_SIZE         ((uint32_t)0x0200)
#define WRITE_READ_ADDR     ((uint32_t)0x0050)

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t qspi_aTxBuffer[BUFFER_SIZE];
uint8_t qspi_aRxBuffer[BUFFER_SIZE];

/* Private function prototypes -----------------------------------------------*/
static void     QSPI_SetHint(void);
static void     Fill_Buffer (uint8_t *pBuffer, uint32_t uwBufferLength, uint32_t uwOffset);
static uint8_t  Buffercmp   (uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength);
static uint8_t  CompareInfo(void);
/* Private functions ---------------------------------------------------------*/

/**
* @brief  QSPI Demo
* @param  None
* @retval None
*/
void QSPI_demo (void)
{ 
  uint8_t status;
  
  QSPI_SetHint();
  
  /*##-1- Configure the QSPI device ##########################################*/
  /* QSPI device configuration */ 
  status = BSP_QSPI_Init();
  
  if (status == QSPI_NOT_SUPPORTED)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"QSPI Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"Check the hardware configuration :", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"  refer to the UM of the board", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 175, (uint8_t*)"  for the hardware modifications", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 190, (uint8_t*)"  to connect the QSPI memory", LEFT_MODE);
  }
  else if (status == QSPI_ERROR)
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"QSPI Initialization : FAILED.", LEFT_MODE);
    BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
  }
  else
  {
    BSP_LCD_DisplayStringAt(20, 100, (uint8_t*)"QSPI Initialization : OK.", LEFT_MODE);
    
    /*##-2- Read & check the QSPI info #######################################*/
    /* Clear the structure */
    QspiInfo.FlashSize = (uint32_t)0x00;
    QspiInfo.EraseSectorSize = (uint32_t)0x00;
    QspiInfo.EraseSectorsNumber = (uint32_t)0x00;
    QspiInfo.ProgPageSize = (uint32_t)0x00;
    QspiInfo.ProgPagesNumber = (uint32_t)0x00;
    QspiInfo.ManufID = (uint8_t)0x00;
    
    /* Read the QSPI memory info by filling QSPIInfo struct  */
    BSP_QSPI_GetInfo(&QspiInfo);
    
    /* Test the correctness */
    if (CompareInfo() !=QSPI_OK)
    {
        BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI GET INFO : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
    }
    else
    {
      if (QspiInfo.ManufID == QSPI_S25FL512S)
      {
        BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI GET INFO : OK. S25FL512S detected  ", LEFT_MODE);
      } else
      {
        BSP_LCD_DisplayStringAt(20, 115, (uint8_t*)"QSPI GET INFO : OK. N25Q512A detected  ", LEFT_MODE);
      }
      
      /*##-3- Erase QSPI memory ################################################*/ 
      if(BSP_QSPI_Erase_Block(WRITE_READ_ADDR) != QSPI_OK)
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"QSPI ERASE : FAILED.", LEFT_MODE);
        BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
      }
      else
      {
        BSP_LCD_DisplayStringAt(20, 130, (uint8_t*)"QSPI ERASE : OK.   ", LEFT_MODE);
        
        /*##-4- QSPI memory read/write access  #################################*/   
        /* Fill the buffer to write */
        Fill_Buffer(qspi_aTxBuffer, BUFFER_SIZE, 0xD20F);   
        
        /* Write data to the QSPI memory */
        if(BSP_QSPI_Write(qspi_aTxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"QSPI WRITE : FAILED.", LEFT_MODE);
          BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
        }
        else
        {
          BSP_LCD_DisplayStringAt(20, 145, (uint8_t*)"QSPI WRITE : OK.     ", LEFT_MODE);
          
          /* Read back data from the QSPI memory */
          if(BSP_QSPI_Read(qspi_aRxBuffer, WRITE_READ_ADDR, BUFFER_SIZE) != QSPI_OK)
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"QSPI READ : FAILED.", LEFT_MODE);
            BSP_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
          }
          else
          {
            BSP_LCD_DisplayStringAt(20, 160, (uint8_t*)"QSPI READ :  OK.    ", LEFT_MODE);
            
            /*##-5- Checking data integrity ############################################*/  
            if(Buffercmp(qspi_aRxBuffer, qspi_aTxBuffer, BUFFER_SIZE) > 0)
            {
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI COMPARE : FAILED.", LEFT_MODE);
              BSP_LCD_DisplayStringAt(20, 190, (uint8_t*)"QSPI Test Aborted.", LEFT_MODE);
            }
            else
            {    
              BSP_LCD_DisplayStringAt(20, 175, (uint8_t*)"QSPI Test : OK.     ", LEFT_MODE);
            }  
          }
        }
      }
    }
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
* @brief  Display QSPI Demo Hint
* @param  None
* @retval None
*/
static void QSPI_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set LCD Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t*)"QSPI", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t*)"This example shows how to write", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t*)"and read data on QSPI memory", CENTER_MODE); 
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t*)"(Hardware modifications needed)", CENTER_MODE); 
  
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
static void Fill_Buffer(uint8_t *pBuffer, uint32_t uwBufferLenght, uint32_t uwOffset)
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
static uint8_t Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint32_t BufferLength)
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

static uint8_t CompareInfo(void)
{
  
  if ((QspiInfo.ManufID != QSPI_S25FL512S) && (QspiInfo.ManufID != QSPI_N25Q512A))
    return QSPI_ERROR;
  if (QspiInfo.FlashSize != 0x4000000)
    return QSPI_ERROR;
  if ((QspiInfo.EraseSectorSize != 0x40000) && (QspiInfo.EraseSectorSize !=  0x10000))
    return QSPI_ERROR;
  if ((QspiInfo.ProgPageSize != 0x200) && (QspiInfo.ProgPageSize != 0x100))
    return QSPI_ERROR;
  if ((QspiInfo.EraseSectorsNumber != 0x100) && (QspiInfo.EraseSectorsNumber !=0x400))
    return QSPI_ERROR;
  if ((QspiInfo.ProgPagesNumber != 131072) && (QspiInfo.ProgPagesNumber != 262144))
    return QSPI_ERROR;
  
  return QSPI_OK;
}

/**
* @}
*/ 

/**
* @}
*/ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
