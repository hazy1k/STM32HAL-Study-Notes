/**
  @page IAP AN3965 STM32F4xx In-Application Programming using the USART Readme file

  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    IAP/IAP_Main/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of implementation of the AN3965 (in-application programming
  *          using the USART (IAP)) on STM32F4xx devices.
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

@par Application Description

This directory contains a set of sources files and pre-configured projects that 
describes how to build an application to be loaded into Flash memory using
In-Application Programming (IAP, through USART).

@par Directory contents

 - "IAP/IAP_Main/inc": contains the IAP firmware header files 
    - IAP/IAP_Main/inc/main.h              The main include file of the project.
    - IAP/IAP_Main/inc/common.h            This file provides all the headers of the common functions.
    - IAP/IAP_Main/inc/flash_if.h          This file provides all the firmware 
                                                     function headers of the flash_if.c file.
    - IAP/IAP_Main/inc/menu.h              This file provides all the firmware
                                                     function headers of the menu.c file.
    - IAP/IAP_Main/inc/ymodem.h            This file provides all the firmware
                                                     function headers of the ymodem.c file.
    - IAP/IAP_Main/inc/stm32f4xx_hal_conf.h  Library Configuration file
    - IAP/IAP_Main/inc/stm32f4xx_it.h      Header for stm32f4xx_it.c

 - "IAP/IAP_Main/MDK-ARM": contains pre-configured project for MDK-ARM toolchain

 - "IAP/IAP_Main/EWARM": contains pre-configured project for EWARM toolchain
 
 - "IAP/IAP_Main/SW4STM32": contains preconfigured project for SW4STM32 toolchain (main program)

 - "IAP/IAP_Main/src": contains the IAP firmware source files
    - IAP/IAP_Main/src/main.c              Main program
    - IAP/IAP_Main/src/stm32f4xx_it.c      Interrupt handlers
    - IAP/IAP_Main/src/stm32f4xx_hal_msp.c Microcontroller specific packages
                                                     initialization file.
    - IAP/IAP_Main/src/flash_if.c          The file contains write, erase and disable
                                                     write protection of the internal Flash
                                                     memory.
    - IAP/IAP_Main/src/menu.c              This file contains the menu to select
                                                     downloading a new binary file, uploading
                                                     internal Flash memory, executing the binary
                                                     and disabling the write protection of
                                                     write-protected pages
    - IAP/IAP_Main/src/common.c            This file provides functions related to
                                                     read/write from/to USART peripheral
    - IAP/IAP_Main/src/ymodem.c            This file provides all the firmware functions
                                                     related to the ymodem protocol.
    - IAP/IAP_Main/src/system_stm32f4xx.c  STM32F4xx system source file

@par Hardware and Software environment 

  - This application runs on STM32F469xx and STM32F479xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevB
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

Table 1. IAP implementation on STM32469I-EVAL RevB
/*** Platform ***|************* Implementation **************************|***** Configuration *****\
****************************************************************************************************
|    Firmware    | The IAP program is located at 0x08000000. The Flash   |                         |
|                | routines (program/erase) are executed from the Flash  |                         |
|                | memory.                                               |                         |
|                | The size of this program is about 16 Kbytes and       |                         |
|                | programmed on:                                        | Sector 0                |  
|                | ------------------------------------------------------|-------------------------|
|                | The user application (image to be downloaded with the |                         | 
|                | IAP) will be programmed starting from address         |                         |
|                | (uint32_t)0x08004000(1).                              | (Sector 1 - Sector 23)  | 
|                | The maximum size of the image to be loaded is:        | 3742 bytes             | 
|                | ------------------------------------------------------|-------------------------|
|                | The image is uploaded with the IAP from the STM32F4xx | 11 Kbytes                | 
|                | internal Flash.                                       | (Sector 1)              |
|                | The size of the image to be uploaded is:              |                         |
|----------------|-------------------------------------------------------|-------------------------|
|    Hardware    | Push-button (active level: high)                      | Tamper push-button      |                                                                     
|                |                                                       | connected to pin PC13   |
|                | ------------------------------------------------------|-------------------------| 
|                | USART used                                            |  USART   (CN7)          |
\**************************************************************************************************/
(1) User application location address is defined in the flash_if.h file as: 
#define APPLICATION_ADDRESS           ((uint32_t)0x08004000)
To modify it, change the default value to the desired one. Note that the application must be linked
relatively to the new address too.

Following picture illustrates the situation in program memory:
Figure 2. Flash memory usage

 Top Flash Memory address /-------------------------------------------\  0x081FFFFF
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          Sector 1 - Sector 23             |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |                                           |
                          |          User code                        |
                          |                                           |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          |-------------------------------------------|  0x08004000
                          |          IAP code                         |
                          |- - - - - - - - - - - - - - - - - - - - - -|
                          |          Vector table                     |
                          \-------------------------------------------/	 0x08000000					  
   
  - STM32469I-EVAL RevB Set-up
    - Connect a null-modem female/female RS232 cable between the boards DB9 connector 
      CN7 (USART) and PC serial port.
      (make sure that jumper JP8 is RS232_RX position pos 1-2).
    - Hold the Tamper push-button during reset to enter the IAP.    

  - Terminal configuration: 
    - Word Length = 8 Bits
    - One Stop Bit
    - No parity
    - BaudRate = 115200 baud
    - flow control: None 
    - Ymodem protocol is using CRC16 by default. To switch to checksum, comment #define CRC16_F
      in ymodem.c

@par How to use it? 

In order to make the program work, you must do the following:

  1. Generate a binary image for the program provided in the 
     "IAP/IAP_Binary_Template" project directory. 
  2. Program the internal Flash with the IAP (see below) 
  3. Open HyperTerminal window using the settings already defined in section
     "Hardware and Software environment" 
  4. To run the IAP driver, keep the Tamper push-button pressed at Reset. 
     The IAP main menu is then displayed on the HyperTerminal window.
  5. To download an application, press 1 and use the Ymodem protocol

In order to load the IAP code, you have do the following:
   - EWARM:
      - Open the Project.eww workspace
      - Rebuild all files: Project->Rebuild all
      - Load project image: Project->Debug
      - Run program: Debug->Go(F5)

   - MDK-ARM:
      - Open the Project.uvproj project
      - Rebuild all files: Project->Rebuild all target files
      - Load project image: Debug->Start/Stop Debug Session
      - Run program: Debug->Run (F5)
      
   - System Workbench for STM32: 
      - Open System Workbench for STM32 toolchain
      - Browse to the SW4STM32 workspace directory, select the project (.project file in \IAP_Main\SW4STM32 \STM32469I-EVAL directory).
      - Rebuild all project files: Select the project in the "Project explorer" window then click on Project->build project menu.
      - Run program: Run->Debug


   - TrueSTUDO 
      - Open the TrueSTUDIO toolchain. 
      - Browse to the TrueSTUDIO workspace directory, select the project (.project file in \IAP_Main\TrueSTUDIO\STM32469I-EVAL directory). 
      - Rebuild all project files: Select the project in the "Project explorer" window then click on Project->build project menu. 
      - Run program: Run->Debug (F11)

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
