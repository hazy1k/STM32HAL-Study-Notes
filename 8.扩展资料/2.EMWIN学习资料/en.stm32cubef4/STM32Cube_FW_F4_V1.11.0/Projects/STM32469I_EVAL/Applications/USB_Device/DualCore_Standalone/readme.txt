/**
  @page DualCore_Standalone USB Device Dual Core application
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/DualCore_Standalone/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the USB Device Dual Core application.
  ******************************************************************************
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
  @endverbatim

@par Application Description 

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to use
USB device application based on the STM32F469xx multi core support feature integrating the Device Communication 
Class (CDC) and Human Interface (HID) in the same project.

This is a typical application on how to use the STM32F469xx USB OTG Device peripheral, where STM32 is 
enumerated as a CDC device in the High Speed mode, and also as a HID device in the Full Speed mode,
using the native PC Host HID/CDC drivers to which the STM32469I-EVAL board is connected. 

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 180 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  � PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLVCO/PLLQ).
  � PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).

When the application is started, the user has just to plug the two USB cables into a PC host and two
USB devices (CDC/HID) are automatically detected. A new VCP drive appears in the system window
for the CDC device and serial communication operations can be performed as with any other serial
communication drive. A new HID Mouse device is detected, and the Joystick buttons mounted on 
the STM32469I-EVAL board, allow to user to emulate the Mouse directions.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".


@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" and "USE_USB_FS" when using USB Dual Core Mode


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents

  - USB_Device/DualCore_Standalone/Src/main.c                  Main program
  - USB_Device/DualCore_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Device/DualCore_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Device/DualCore_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/DualCore_Standalone/Src/usbd_cdc_desc.c         USB device CDC descriptor
  - USB_Device/DualCore_Standalone/Src/usbd_hid_desc.c         USB device HID descriptor
  - USB_Device/DualCore_Standalone/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - USB_Device/DualCore_Standalone/Src/usbd_cdc_interface.c    USBD CDC interface 
  - USB_Device/DualCore_Standalone/Inc/main.h                  Main program header file
  - USB_Device/DualCore_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Device/DualCore_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Device/DualCore_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/DualCore_Standalone/Inc/usbd_desc.h             USB device descriptor header file  
  - USB_Device/DualCore_Standalone/Inc/usbd_cdc_desc.h         USB device CDC descriptor header file
  - USB_Device/DualCore_Standalone/Inc/usbd_hid_desc.h         USB device HID descriptor header file
  - USB_Device/DualCore_Standalone/Inc/usbd_cdc_interface.h    USBD CDC interface header file

@par Hardware and Software environment

  - This application runs on STM32F469xx and STM32F479xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevB
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32469I-EVAL RevB Set-up
    - Connect the STM32469I-EVAL board to the PC through two 'USB micro A-Male 
      to A-Male' cables to the connectors:
      - CN8 : to use USB High Speed (HS) 
      - CN13: to use USB Full Speed (FS)
        @note Make sure that :
         - jumper JP9 must be removed when using USB OTG FS

    - Connect the STM32469I-EVAL board to the PC (or to another evaluation board) through RS232 (USART)
      serial cable CN7 (USART1) connector.
  @note Make sure that :
    - jumper JP8  is on RS232_RX position (1-2)  and 
    - jumper JP15 is on USART1_RX position (1-2) and 
    - jumper JP19 is on USART1_TX position (1-2).
    - For loopback mode test: remove RS232 cable on CN7 and connect directly USART TX and RX pins:
      PA9 and PA10 (with a cable or a jumper)
      
    - Hyperterminal configuration:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = No parity
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */            

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
