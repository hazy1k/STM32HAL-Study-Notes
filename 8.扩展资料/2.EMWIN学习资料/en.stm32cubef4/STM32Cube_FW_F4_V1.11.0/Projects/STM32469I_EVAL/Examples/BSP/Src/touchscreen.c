/**
  ******************************************************************************
  * @file    BSP/Src/touchscreen.c
  * @author  MCD Application Team
  * @version V1.0.3
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
#define  CIRCLE_RADIUS        40
/* Private macro -------------------------------------------------------------*/
#define  CIRCLE_XPOS(i)       ((i * BSP_LCD_GetXSize()) / 5)
#define  CIRCLE_YPOS(i)       (BSP_LCD_GetYSize() - CIRCLE_RADIUS - 60)

/* Private Structures and Enumerations ------------------------------------------------------------*/
/* Possible allowed indexes of touchscreen demo */
typedef enum
{
  TOUCHSCREEN_DEMO_1 = 1,
  TOUCHSCREEN_DEMO_2 = 2,
  TOUCHSCREEN_DEMO_3 = 3,
  TOUCHSCREEN_DEMO_MAX = TOUCHSCREEN_DEMO_3,

} TouchScreenDemoTypeDef;

/* Global variables ---------------------------------------------------------*/
TS_StateTypeDef  TS_State = {0};

/* Private variables ---------------------------------------------------------*/
/* Static variable holding the current touch color index : used to change color at each touch */
static uint32_t touchscreen_color_idx = 0;

/* Private function prototypes -----------------------------------------------*/
static void     Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex);
static void     Touchscreen_DrawBackground_Demo1(uint8_t state);
static uint32_t Touchscreen_Handle_NewTouch(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Touchscreen Demo1 : test touchscreen calibration and single touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo1(void)
{
  uint16_t x1, y1;
  uint8_t state = 0;
  uint8_t exitTsUseCase = 0;
  uint32_t ts_status = TS_OK;

  /* Reset touch data information */
  BSP_TEST_APPLI_ASSERT(BSP_TS_ResetTouchData(&TS_State));

  /* If calibration is not yet done, proceed with calibration */
  if (TouchScreen_IsCalibrationDone() == 0)
  {
    ts_status = Touchscreen_Calibration();
    if (ts_status == TS_OK)
    {
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *) "Touchscreen calibration success.", CENTER_MODE);
    }
  } /* of if (TouchScreen_IsCalibrationDone() == 0) */

  if(ts_status == TS_OK)
  {
    /* Display touch screen demo description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_1);
    Touchscreen_DrawBackground_Demo1(state);

    while (exitTsUseCase == 0)
    {
      if (ts_status == TS_OK)
      {
        /* Check in polling mode in touch screen the touch status and coordinates */
        /* of touches if touch occurred                                           */
        ts_status = BSP_TS_GetState(&TS_State);
        if(TS_State.touchDetected)
        {
          /* One or dual touch have been detected          */
          /* Only take into account the first touch so far */

          /* Get X and Y position of the first touch post calibrated */
          x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
          y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);

          if ((y1 > (CIRCLE_YPOS(1) - CIRCLE_RADIUS)) &&
              (y1 < (CIRCLE_YPOS(1) + CIRCLE_RADIUS)))
          {
            if ((x1 > (CIRCLE_XPOS(1) - CIRCLE_RADIUS)) &&
                (x1 < (CIRCLE_XPOS(1) + CIRCLE_RADIUS)))
            {
              if ((state & 1) == 0)
              {
                Touchscreen_DrawBackground_Demo1(state);
                BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
                BSP_LCD_FillCircle(CIRCLE_XPOS(1), CIRCLE_YPOS(1), CIRCLE_RADIUS);
                state = 1;
              }
            }
            if ((x1 > (CIRCLE_XPOS(2) - CIRCLE_RADIUS)) &&
                (x1 < (CIRCLE_XPOS(2) + CIRCLE_RADIUS)))
            {
              if ((state & 2) == 0)
              {
                Touchscreen_DrawBackground_Demo1(state);
                BSP_LCD_SetTextColor(LCD_COLOR_RED);
                BSP_LCD_FillCircle(CIRCLE_XPOS(2), CIRCLE_YPOS(2), CIRCLE_RADIUS);
                state = 2;
              }
            }

            if ((x1 > (CIRCLE_XPOS(3) - CIRCLE_RADIUS)) &&
                (x1 < (CIRCLE_XPOS(3) + CIRCLE_RADIUS)))
            {
              if ((state & 4) == 0)
              {
                Touchscreen_DrawBackground_Demo1(state);
                BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
                BSP_LCD_FillCircle(CIRCLE_XPOS(3), CIRCLE_YPOS(3), CIRCLE_RADIUS);
                state = 4;
              }
            }

            if ((x1 > (CIRCLE_XPOS(4) - CIRCLE_RADIUS)) &&
                (x1 < (CIRCLE_XPOS(4) + CIRCLE_RADIUS)))
            {
              if ((state & 8) == 0)
              {
                Touchscreen_DrawBackground_Demo1(state);
                BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
                BSP_LCD_FillCircle(CIRCLE_XPOS(4), CIRCLE_YPOS(3), CIRCLE_RADIUS);
                state = 8;
              }
            }
          }

        } /* of if(TS_State.TouchDetected) */

      } /* of if (ts_status == TS_OK) */

      /* Wait for a key button press to switch to next test case of BSP validation application */
      /* Otherwise stay in the test */
      exitTsUseCase = CheckForUserInput();

      HAL_Delay(20);

    } /* of while (exitTsUseCase == 0) */

  } /* of if(status == TS_OK) */
}

#if (TS_MULTI_TOUCH_SUPPORTED == 1)
/**
  * @brief  Touchscreen Demo2 : test touchscreen single and dual touch in polling mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo2(void)
{
  uint8_t exitTsUseCase = 0;
  uint32_t ts_status = TS_OK;

  /* Reset touch data information */
  BSP_TEST_APPLI_ASSERT(BSP_TS_ResetTouchData(&TS_State));

  /* If calibration is not yet done, this is an error : touchscreen demo 1 should have been run before anyway  */
  if (TouchScreen_IsCalibrationDone() == 1)
  {
  } /* of if (TouchScreen_IsCalibrationDone() == 1) */
  else
  {
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"ERROR : touchscreen not yet calibrated.", CENTER_MODE);
    ts_status = TS_ERROR;
  }

  if(ts_status == TS_OK)
  {
    /* Display touch screen demo2 description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_2);

    while (exitTsUseCase == 0)
    {
      Touchscreen_Handle_NewTouch();

      /* Wait for a key button press to switch to next test case of BSP validation application */
      /* Otherwise stay in the test */
      exitTsUseCase = CheckForUserInput();

      HAL_Delay(100);

    } /* of while (exitTsUseCase == 0) */

  } /* of if(status == TS_OK) */
}

/**
  * @brief  Touchscreen Demo3 : test touchscreen single and dual touch in interrupt mode
  * @param  None
  * @retval None
  */
void Touchscreen_demo3(void)
{
  uint8_t exitTsUseCase = 0;
  uint32_t ts_status = TS_OK;

  /* Reset touch data information */
  BSP_TEST_APPLI_ASSERT(BSP_TS_ResetTouchData(&TS_State));

  /* If calibration is not yet done, this is an error : touchscreen demo 1 should have been run before anyway  */
  if (TouchScreen_IsCalibrationDone() == 0)
  {
    BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 65, (uint8_t *)"ERROR : touchscreen not yet calibrated.", CENTER_MODE);
    ts_status = TS_ERROR;
  }

  if(ts_status == TS_OK)
  {
    /* Display touch screen demo3 description */
    Touchscreen_SetHint_Demo(TOUCHSCREEN_DEMO_3);

    /* Set touchscreen in Interrupt mode and program MFX accordingly to receipt INT falling edge from MFX */
    /* and generate EXTI when occurs                                                                      */
    ts_status = BSP_TS_ITConfig();
    BSP_TEST_APPLI_ASSERT(ts_status != TS_OK);

    while (exitTsUseCase == 0)
    {
      /* In order to avoid using I2C to ask MFX when not need (e.g.  BSP_TS_ITGetStatus) */
      /* check first if EXTI occurred                                                    */
      /* Note : variable 'ts_exti_received' is set to 1b1' by HAL_GPIO_EXTI_Callback()   */
      if(mfx_exti_received == 1)
      {
        mfx_exti_received = 0;
        if(BSP_TS_ITGetStatus() == TS_IRQ_PENDING)
        {
          /* Clear first BSP TS IRQ */
          BSP_TS_ITClear();

          /* Handle the new touch */
          Touchscreen_Handle_NewTouch();
        }
      } 

      /* Wait for a key button press to switch to next test case of BSP validation application */
      /* Otherwise stay in the test */
      exitTsUseCase = CheckForUserInput();

      HAL_Delay(10);

    } /* of while (exitTsUseCase == 0) */

  } /* of if(status == TS_OK) */
}

/**
  * @brief  Touchscreen_Handle_NewTouch: factorization of touch management
  * @param  None
  * @retval TS_OK or TS_ERROR
  */
static uint32_t Touchscreen_Handle_NewTouch(void)
{
#define TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS 15
#define TOUCH_INFO_STRING_SIZE                70
  uint16_t x1 = 0;
  uint16_t y1 = 0;
  uint16_t x2 = 0;
  uint16_t y2 = 0;
  uint32_t drawTouch1 = 0; /* activate/deactivate draw of footprint of touch 1 */
  uint32_t drawTouch2 = 0; /* activate/deactivate draw of footprint of touch 2 */
  uint32_t colors[24] = {LCD_COLOR_BLUE, LCD_COLOR_GREEN, LCD_COLOR_RED, LCD_COLOR_CYAN, LCD_COLOR_MAGENTA, LCD_COLOR_YELLOW,
                         LCD_COLOR_LIGHTBLUE, LCD_COLOR_LIGHTGREEN, LCD_COLOR_LIGHTRED, LCD_COLOR_LIGHTCYAN, LCD_COLOR_LIGHTMAGENTA,
                         LCD_COLOR_LIGHTYELLOW, LCD_COLOR_DARKBLUE, LCD_COLOR_DARKGREEN, LCD_COLOR_DARKRED, LCD_COLOR_DARKCYAN,
                         LCD_COLOR_DARKMAGENTA, LCD_COLOR_DARKYELLOW, LCD_COLOR_LIGHTGRAY, LCD_COLOR_GRAY, LCD_COLOR_DARKGRAY,
                         LCD_COLOR_BLACK, LCD_COLOR_BROWN, LCD_COLOR_ORANGE };
  uint32_t ts_status = TS_OK;
  uint8_t lcd_string[TOUCH_INFO_STRING_SIZE] = "";

  /* Check in polling mode in touch screen the touch status and coordinates */
  /* of touches if touch occurred                                           */
  ts_status = BSP_TS_GetState(&TS_State);
  if(TS_State.touchDetected)
  {
    /* One or dual touch have been detected  */

    /* Erase previous information on touchscreen play pad area */
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_FillRect(0, 80, BSP_LCD_GetXSize(), BSP_LCD_GetYSize() - 160);

    /* Re-Draw touch screen play area on LCD */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 180);
    BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 182);

    /* Erase previous information on bottom text bar */
    BSP_LCD_FillRect(0, BSP_LCD_GetYSize() - 80, BSP_LCD_GetXSize(), 80);

    /* Desactivate drawing footprint of touch 1 and touch 2 until validated against boundaries of touch pad values */
    drawTouch1 = drawTouch2 = 0;

    /* Get X and Y position of the first touch post calibrated */
    x1 = TouchScreen_Get_Calibrated_X(TS_State.touchX[0]);
    y1 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[0]);

    if((y1 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
       (y1 < (BSP_LCD_GetYSize() - 90 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
    {
      drawTouch1 = 1;
    }

    /* If valid touch 1 position : inside the reserved area for the use case : draw the touch */
    if(drawTouch1 == 1)
    {
      /* Draw circle of first touch : turn on colors[] table */
      BSP_LCD_SetTextColor(colors[(touchscreen_color_idx++ % 24)]);
      BSP_LCD_FillCircle(x1, y1, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS);

      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_SetFont(&Font16);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 70, (uint8_t *)"TOUCH INFO : ", CENTER_MODE);

      BSP_LCD_SetFont(&Font12);
      sprintf((char*)lcd_string, "x1 = %d, y1 = %d, Event = %s, Weight = %d",
              x1,
              y1,
              ts_event_string_tab[TS_State.touchEventId[0]],
              TS_State.touchWeight[0]);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 45, lcd_string, CENTER_MODE);
    } /* of if(drawTouch1 == 1) */

    if(TS_State.touchDetected > 1)
    {
      /* Get X and Y position of the second touch post calibrated */
      x2 = TouchScreen_Get_Calibrated_X(TS_State.touchX[1]);
      y2 = TouchScreen_Get_Calibrated_Y(TS_State.touchY[1]);

      if((y2 > (90 + TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)) &&
         (y2 < (BSP_LCD_GetYSize() - 90 - TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS)))
      {
        drawTouch2 = 1;
      }

      /* If valid touch 2 position : inside the reserved area for the use case : draw the touch */
      if(drawTouch2 == 1)
      {
        sprintf((char*)lcd_string, "x2 = %d, y2 = %d, Event = %s, Weight = %d",
                x2,
                y2,
                ts_event_string_tab[TS_State.touchEventId[1]],
                TS_State.touchWeight[1]);
        BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 35, lcd_string, CENTER_MODE);

        /* Draw circle of second touch : turn on color[] table */
        BSP_LCD_SetTextColor(colors[(touchscreen_color_idx++ % 24)]);
        BSP_LCD_FillCircle(x2, y2, TS_MULTITOUCH_FOOTPRINT_CIRCLE_RADIUS);
      } /* of if(drawTouch2 == 1) */

    } /* of if(TS_State.touchDetected > 1) */

    if((drawTouch1 == 1) || (drawTouch2 == 1))
    {
      /* Get updated gesture Id in global variable 'TS_State' */
      ts_status = BSP_TS_Get_GestureId(&TS_State);

      sprintf((char*)lcd_string, "Gesture Id = %s", ts_gesture_id_string_tab[TS_State.gestureId]);
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 15, lcd_string, CENTER_MODE);
    }
    else
    {
      /* Invalid touch position */
      BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
      BSP_LCD_DisplayStringAt(0, BSP_LCD_GetYSize() - 70, (uint8_t *)"Invalid touch position : use drawn touch area : ", CENTER_MODE);
    }
  } /* of if(TS_State.TouchDetected) */

  return(ts_status);
}

#endif /* TS_MULTI_TOUCH_SUPPORTED == 1 */

/**
  * @brief  Display TS Demo Hint for all touchscreen demos depending on passed
  *         demoIndex in parameter.
  * @param  demoIndex : parameter of type @ref TouchScreenDemoTypeDef
  * @retval None
  */
static void Touchscreen_SetHint_Demo(TouchScreenDemoTypeDef demoIndex)
{
  if(demoIndex <= TOUCHSCREEN_DEMO_MAX)
  {
    /* Clear the LCD */
    BSP_LCD_Clear(LCD_COLOR_WHITE);

    /* Set Touchscreen Demo1 description */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_FillRect(0, 0, BSP_LCD_GetXSize(), 80);
    BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_SetFont(&Font24);

    if(demoIndex == TOUCHSCREEN_DEMO_1)
    {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen basic polling", CENTER_MODE);
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please use the Touchscreen to", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate the colored circle", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"inside the rectangle. Then press TAMPER button", CENTER_MODE);
    }
    else if (demoIndex == TOUCHSCREEN_DEMO_2)
    {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch polling", CENTER_MODE);
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);

    }
    else /* demoIndex == TOUCHSCREEN_DEMO_3 */
    {
      BSP_LCD_DisplayStringAt(0, 0, (uint8_t *)"Touchscreen dual touch interrupt", CENTER_MODE);
      BSP_LCD_SetFont(&Font12);
      BSP_LCD_DisplayStringAt(0, 30, (uint8_t *)"Please press the Touchscreen to", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 45, (uint8_t *)"activate single and", CENTER_MODE);
      BSP_LCD_DisplayStringAt(0, 60, (uint8_t *)"dual touch", CENTER_MODE);
    }

    /* Set the LCD Text Color */
    BSP_LCD_SetTextColor(LCD_COLOR_BLUE);
    BSP_LCD_DrawRect(10, 90, BSP_LCD_GetXSize() - 20, BSP_LCD_GetYSize() - 100);
    BSP_LCD_DrawRect(11, 91, BSP_LCD_GetXSize() - 22, BSP_LCD_GetYSize() - 102);

  } /* of if(demoIndex <= TOUCHSCREEN_DEMO_MAX) */
}

/**
  * @brief  Draw Touchscreen Background
  * @param  state : touch zone state
  * @retval None
  */
static void Touchscreen_DrawBackground_Demo1 (uint8_t state)
{
  switch (state)
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
