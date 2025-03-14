/*********************************************************************
*          Portions COPYRIGHT 2016 STMicroelectronics                *
*          Portions SEGGER Microcontroller GmbH & Co. KG             *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2015  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.28 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The  software has  been licensed  to STMicroelectronics International
N.V. a Dutch company with a Swiss branch and its headquarters in Plan-
les-Ouates, Geneva, 39 Chemin du Champ des Filles, Switzerland for the
purposes of creating libraries for ARM Cortex-M-based 32-bit microcon_
troller products commercialized by Licensee only, sublicensed and dis_
tributed under the terms and conditions of the End User License Agree_
ment supplied by STMicroelectronics International N.V.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUIDEMO_Intro.c
Purpose     : Introduction for emWin generic demo
              (This is also a good file to demo and explain basic
              emWin features by setting breakpoints)
----------------------------------------------------------------------
*/

/**
  ******************************************************************************
  * @file    GUIDEMO_Intro.c
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   Introduction for emWin generic demo
  ******************************************************************************
  * @attention
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


#include <string.h>

#include "GUIDEMO.h"

/*********************************************************************
*
*       Defines
*/
#define SCREEN_DIV          6 // 2^6 = 64

#define FACTOR_EMWIN        4
#define FACTOR_DESC        11
#define FACTOR_ANY_COMP    22
#define FACTOR_VERSION     31
#define FACTOR_LOGO        38
#define FACTOR_WWW         56

#define DIST_ANY_COMP      18

/*********************************************************************
*
*       GUIDEMO_Intro
*
**********************************************************************
*/
void GUIDEMO_Intro(void) {
  char acVersion[30] = "Version of STemWin: ";
  int  xCenter, xSize, ySize;

  xSize   = LCD_GetXSize();
  ySize   = LCD_GetYSize();
  xCenter = xSize >> 1;

  GUIDEMO_DrawBk(0);
  GUI_SetTextMode(GUI_TM_TRANS);
  //
  // STemWin
  //
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontRounded22);
  GUI_DispStringHCenterAt("STemWin", xCenter, (FACTOR_EMWIN * ySize) >> SCREEN_DIV);
  //
  // STemWin description
  //
  GUI_SetFont(&GUI_Font16_ASCII);
  GUI_DispStringHCenterAt("Universal graphic software\nfor embedded applications", xCenter, (FACTOR_DESC * ySize) >> SCREEN_DIV);
  //
  // Any text
  //
  GUI_SetColor(0x2288ff);
  GUI_DispStringHCenterAt("For STM32 MCU Products - CM3 - CM4", xCenter, (FACTOR_ANY_COMP * ySize) >> SCREEN_DIV);
  //
  // Compiled
  //
  GUI_SetFont(&GUI_Font10S_ASCII);
  GUI_DispStringHCenterAt("Compiled " __DATE__ " "__TIME__,          xCenter, ((FACTOR_ANY_COMP * ySize) >> SCREEN_DIV) + DIST_ANY_COMP);
  //
  // Version
  //
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_Font16_ASCII);
  strcat(acVersion, GUI_GetVersionString());
  GUI_DispStringHCenterAt(acVersion, xCenter,  (FACTOR_VERSION * ySize) >> SCREEN_DIV);
  //
  // Logo
  //
  GUI_DrawBitmap(&bmSTLogo, (xSize - bmSeggerLogo.XSize) >> 1, (FACTOR_LOGO * ySize) >> SCREEN_DIV);
  //
  // www.st.com
  //
  GUI_SetColor(GUI_WHITE);
  GUI_SetFont(&GUI_FontRounded16);
  GUI_DispStringHCenterAt("www.st.com/stm32", xCenter*2/3, (FACTOR_WWW * ySize) >> SCREEN_DIV);
  GUIDEMO_Delay(5000);
}

/*************************** End of file ****************************/
