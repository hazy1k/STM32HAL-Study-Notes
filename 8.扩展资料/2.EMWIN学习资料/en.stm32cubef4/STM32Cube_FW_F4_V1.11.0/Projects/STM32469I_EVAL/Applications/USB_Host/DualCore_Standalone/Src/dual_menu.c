/**
  ******************************************************************************
  * @file    USB_Host/DualCore_Standalone/Src/dual_menu.c 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Dual Core Process
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
uint8_t *DEMO_main_menu[] = 
{
  (uint8_t *)"      1 - Mass Storage demo                                                 ",
  (uint8_t *)"      2 - Humain Interface demo                                             ",
  (uint8_t *)"      3 - Re-Enumerate MSC                                                  ",  
  (uint8_t *)"      4 - Re-Enumerate HID                                                  ", 
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t prev_select = 0;
DEMO_StateMachine demo;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Menu Init
  * @param  None
  * @retval None
  */
void DUAL_MenuInit(void)
{
  /* Start HID Interface */
  LCD_UsrLogY("USB Host Full speed initialized.\n");
  /* Start MSC Interface */
  LCD_UsrLogY("USB Host High speed initialized.\n");
  
  BSP_LCD_SetTextColor(LCD_COLOR_GREEN);
  BSP_LCD_DisplayStringAtLine(17, (uint8_t *)"Use [Joystick Left/Right] to scroll up/down");
  BSP_LCD_DisplayStringAtLine(18, (uint8_t *)"Use [Joystick Up/Down] to scroll DUAL menu");
  Demo_SelectItem(DEMO_main_menu, 0); 
}

/**
  * @brief  Manages DUAL Menu Process.
  * @param  None
  * @retval None
  */
void DUAL_MenuProcess(void)
{
  switch(demo.state)
  {
  case DEMO_IDLE:
    Demo_SelectItem(DEMO_main_menu, 0); 
    LCD_UsrLog("Select an operation to continue.\n");
    demo.state = DEMO_WAIT;
    demo.select = 0;
    break;    
  case DEMO_WAIT:
    
    if(demo.select != prev_select)
    {
      prev_select = demo.select;
      Demo_SelectItem(DEMO_main_menu, demo.select & 0x7F);
      
      /* Handle select item */
      if(demo.select & 0x80)
      {
        demo.select &= 0x7F;
        
        switch(demo.select)
        {
        case 0:
          demo.state = DEMO_MSC_START;  
          break;
          
        case 1:
          demo.state = DEMO_HID_START;  
          break;
          
        case 2:
          demo.state = DEMO_MSC_REENUMERATE;
          break;
          
        case 3:
          demo.state = DEMO_HID_REENUMERATE;
          break;
          
        default:
          break;
        }
      }
    }
    break;
    
  case DEMO_MSC_START:
    if(Appli_HS_state == APPLICATION_HS_READY) 
    {
      /* If HS device connected*/
      demo.state = DEMO_MSC; 
      demo.msc_state = APPLI_MSC_IDLE;
    }
    else
    {
      LCD_ErrLog("No Mass storage device connected.\n");
      demo.state = DEMO_WAIT; 
    }
    break;
    
  case DEMO_HID_START:
    if(Appli_FS_state == APPLICATION_FS_READY)
    {
      /*if FS device connected*/
      demo.state = DEMO_HID; 
      demo.hid_state = APPLI_HID_IDLE;
    }
    else
    {
      LCD_ErrLog("No HID device connected.\n");
      demo.state = DEMO_WAIT; 
    }
    break;
    
  case DEMO_MSC: 
    MSC_MenuProcess();
    break;
    
  case DEMO_HID:
    HID_MenuProcess();
    break;
    
  case DEMO_MSC_REENUMERATE: /* Re-Enumeration MSC */
    LCD_DbgLog("Starting Enumeration MSC...."); 
    LCD_DbgLog("\n");
    USBH_ReEnumerate(&hUSBHost_HS); 
    demo.state = DEMO_WAIT;
    break;
    
  case DEMO_HID_REENUMERATE: /* Re-Enumeration HID */
    LCD_DbgLog("Starting Enumeration HID...."); 
    LCD_DbgLog("\n");
    USBH_ReEnumerate(&hUSBHost_FS);
    demo.state = DEMO_WAIT;
    break;
    
  default:
    break;
  }
  
  if(Appli_FS_state == APPLICATION_FS_DISCONNECT)
  {
    Appli_FS_state = APPLICATION_IDLE; 
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("device FS disconnected!\n");
    demo.state = DEMO_IDLE;
    demo.select = 0; 
  }
  
  if(Appli_HS_state == APPLICATION_HS_DISCONNECT)
  {
    Appli_HS_state = APPLICATION_IDLE; 
    LCD_LOG_ClearTextZone();
    LCD_ErrLog("device HS disconnected!\n");
    demo.state = DEMO_IDLE;
    demo.select = 0; 
  }
}

/**
  * @brief  Probes the joystick state.
  * @param  state: Joystick state
  * @retval None
  */
void Demo_ProbeKey(JOYState_TypeDef state)
{
  if((state == JOY_UP) && (demo.select > 0))
  {
    demo.select--;
  }
  else if(((demo.state == DEMO_WAIT) && (state == JOY_DOWN) && (demo.select < 3)) ||
          ((demo.state == DEMO_MSC)  && (state == JOY_DOWN) && (demo.select < 2)) ||
          ((demo.state == DEMO_HID)  && (state == JOY_DOWN) && (demo.select < 1)))           
  {
    demo.select++;
  }
  else if(state == JOY_SEL)
  {
    demo.select |= 0x80;
  }  
}

/**
  * @brief  Manages the menu on the screen.
  * @param  menu: Menu table
  * @param  item: Selected item to be highlighted
  * @retval None
  */
void Demo_SelectItem(uint8_t **menu, uint8_t item)
{
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  
  switch(item)
  {
  case 0: 
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);
    BSP_LCD_DisplayStringAtLine(19, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);    
    BSP_LCD_DisplayStringAtLine(20, menu[1]);
    BSP_LCD_DisplayStringAtLine(21, menu[2]);
    break;
    
  case 1: 
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(19, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);    
    BSP_LCD_DisplayStringAtLine(20, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);  
    BSP_LCD_DisplayStringAtLine(21, menu[2]); 
    break;
    
  case 2: 
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(19, menu[0]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);    
    BSP_LCD_DisplayStringAtLine(20, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);  
    BSP_LCD_DisplayStringAtLine(21, menu[2]); 
    break;
    
  case 3: 
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);
    BSP_LCD_DisplayStringAtLine(19, menu[1]);
    BSP_LCD_SetBackColor(LCD_COLOR_BLUE);    
    BSP_LCD_DisplayStringAtLine(20, menu[2]);
    BSP_LCD_SetBackColor(LCD_COLOR_MAGENTA);  
    BSP_LCD_DisplayStringAtLine(21, menu[3]); 
    break;
    
  default:
    break;
  }
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK); 
}

/**
  * @brief  EXTI line detection callbacks.
  * @param  GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{  
  static JOYState_TypeDef JoyState = JOY_NONE;
  
  if(GPIO_Pin == MFX_IRQOUT_PIN)
  {    
    /* The different functionalities of MFX (TS, Joystick, SD detection, etc. )  
    can be configured in exti mode to generate an IRQ on given events.
    The MFX IRQ_OUT pin is unique and common to all functionalities, so if several 
    functionalities are configured in exit mode, the MCU has to enquire MFX about  
    the IRQ source (see BSP_IO_ITGetStatus). Communication with Mfx is done by I2C. 
    Often the sw requires ISRs (irq service routines) to be quick while communication 
    with I2C can be considered relatively long (hundreds of usec depending on I2C clk). 
    In order to avoid to use "blocking I2C communication" on interrupt service routines 
    it's suggested (as alternative to this implementation) to use dedicated semaphore*/

    /* Get the Joystick State */
    JoyState = BSP_JOY_GetState();
    HAL_Delay(200);
    
    Demo_ProbeKey(JoyState); 
    
    switch(JoyState)
    {
    case JOY_LEFT:
      LCD_LOG_ScrollBack();
      break;
      
    case JOY_RIGHT:
      LCD_LOG_ScrollForward();
      break;          
      
    default:
      break;           
    }
    
    /* Clear joystick interrupt pending bits */
    BSP_IO_ITClear();
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
