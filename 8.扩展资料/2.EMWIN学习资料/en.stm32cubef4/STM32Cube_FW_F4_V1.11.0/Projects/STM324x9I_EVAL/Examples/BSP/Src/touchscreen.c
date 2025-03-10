/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c 
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   This example code shows how to use the touchscreen driver.
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
#define  CIRCLE_RADIUS        30
    
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)       (BSP_LCD_GetYSize() - CIRCLE_RADIUS - 60)
    
/* Private variables ---------------------------------------------------------*/
static TS_StateTypeDef  TS_State;

/* Private function prototypes -----------------------------------------------*/
static void Touchscreen_SetHint(void);
static void Touchscreen_DrawBackground (uint8_t state);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo
  * @param  None
  * @retval None
  */
void Touchscreen_demo(void)
{ 
  uint16_t x,y;
  uint8_t status = 0;
  uint8_t state = 0;
  
  if(IsCalibrationDone() == 0)
  {
    Touchscreen_Calibration();
  }
  
  Touchscreen_SetHint();
  
  status = BSP_TS_Init(BSP_LCD_GetXSize(), BSP_LCD_GetYSize());
  
  if (status != TS_OK)
  {
    BSP_LCD_SetBackColor(LCD_COLOR_WHITE); 
    BSP_LCD_SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 95, (uint8_t *)"ERROR", CENTER_MODE);
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize()- 80, (uint8_t *)"Touchscreen cannot be initialized", CENTER_MODE);
  }
  else
  {
    Touchscreen_DrawBackground(state);
  }
  
  while (1)
  {
    if (status == TS_OK)
    {
      BSP_TS_GetState(&TS_State);
      
      x = Calibration_GetX(TS_State.x);
      y = Calibration_GetX(TS_State.y);
      
      if((TS_State.TouchDetected) && 
         (y > (CIRCLE_YPOS(1) - CIRCLE_RADIUS))&&
           (y < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
      {
        
        if((x > (CIRCLE_XPOS(1) - CIRCLE_RADIUS))&&
           (x < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
        {
          if((state & 1) == 0)
          {
            Touchscreen_DrawBackground(state);
            BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
            BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
            state = 1;
          }
        }
        if((x > (CIRCLE_XPOS(2) - CIRCLE_RADIUS))&&
           (x < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
        {
          if((state & 2) == 0)
          {          
            Touchscreen_DrawBackground(state);
            BSP_LCD_SetTextColor(LCD_COLOR_RED); 
            BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
            state = 2;
          }          
        }
        
        if((x > (CIRCLE_XPOS(3) - CIRCLE_RADIUS))&&
           (x < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
        {
          if((state & 4) == 0)
          {           
            Touchscreen_DrawBackground(state);
            BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
            BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
            state = 4;
          }            
        }
        
        if((x > (CIRCLE_XPOS(4) - CIRCLE_RADIUS))&&
           (x < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
        {
          if((state & 8) == 0)
          {           
            Touchscreen_DrawBackground(state);
            BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
            BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
            state = 8;
          }           
        }        
      }   
    }
    
    if(CheckForUserInput() > 0)
    {
      return;
    }
    
    HAL_Delay(10);
  }
}

/**
  * @brief  Display TS Demo Hint
  * @param  None
  * @retval None
  */
static void Touchscreen_SetHint(void)
{
  /* Clear the LCD */ 
  BSP_LCD_Clear(LCD_COLOR_WHITE);
  
  /* Set Touchscreen Demo description */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
  BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_SetBackColor(LCD_COLOR_BLUE); 
  BSP_LCD_SetFont(&Font24);
  BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen", CENTER_MODE);
  BSP_LCD_SetFont(&Font12);
  BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate the colored circle", CENTER_MODE);
  BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"inside the rectangle", CENTER_MODE);
  
  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_BLUE);  
  BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize()- 100);
  BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize()- 102);
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
static void Touchscreen_DrawBackground (uint8_t state)
{
  switch(state)
  {
  case 0:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);        
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2);
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2); 
    break;
    
  case 1:
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS - 2);        
    break;
    
  case 2:
    BSP_LCD_SetTextColor(LCD_COLOR_RED); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS - 2); 
    break;
    
  case 4:
    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS - 2); 
    break;
    
  case 8:
    BSP_LCD_SetTextColor(LCD_COLOR_GREEN); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE); 
    BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(4), CIRCLE_RADIUS - 2); 
    break;
  }
}

/**
  * @}
  */ 
   
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
