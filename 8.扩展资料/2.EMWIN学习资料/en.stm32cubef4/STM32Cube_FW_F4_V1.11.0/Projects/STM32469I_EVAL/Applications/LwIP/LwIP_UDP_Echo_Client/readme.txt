/**
  @page LwIP_UDP_Echo_Client LwIP UDP Echo Client Application
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    LwIP/LwIP_UDP_Echo_Client/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the LwIP UDP Echo Client Application.
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

This application guides STM32Cube HAL API users to run a UDP Echo Client application 
based on Raw API of LwIP TCP/IP stack

To run this application, On the remote PC, open a command prompt window.
(In Windows, select Start > All Programs > Accessories > Command Prompt.)
At the command prompt, enter:
    C:\>echotool /p udp /s
where:
    � /p transport layer protocol used for communication (UDP)
    � /s is the actual mode of connection (Server mode)

Each time the user pushes the TAMPER button of the STM32469I-EVAL board, an echo 
request is sent to the server

STM32 Eval board LEDs are used for the following purpose:
  + LED1: ethernet cable is connected.
  + LED2: ethernet cable is not connected.

Note: In this application the Ethernet Link ISR need the System tick interrupt 
to configure the Ethernet MAC, so the Ethernet Link interrupt priority must be 
set lower (numerically greater) than the Systick interrupt priority to ensure 
that the System tick increments while executing the Ethernet Link ISR.

Note: By default, the Ethernet Half duplex mode is not supported in the 
STM32469I-EVAL board, for more information refer to the HAL_ETH_MspInit() 
function in the ethernetif.c file

Note : on MB1165 ETH_MDIO is connected to PA2 by default (SB40 is closed) 

Note :  on MB1165 ETH pins PC1..5 are connected by default (bridges are closed): 
        PC1 (sb31), PC2 (r233), PC3 (sb54) PC4 (sb53), PC5 (sb73) 


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about this application, refer to UM1713 "STM32Cube interfacing with LwIP and applications"


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents

  - LwIP/LwIP_UDP_Echo_Client/Inc/app_ethernet.h          header of app_ethernet.c file
  - LwIP/LwIP_UDP_Echo_Client/Inc/ethernetif.h            header for ethernetif.c file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - LwIP/LwIP_UDP_Echo_Client/Inc/stm32f4xx_it.h          STM32 interrupt handlers header file
  - LwIP/LwIP_UDP_Echo_Client/Inc/main.h                  Main program header file
  - LwIP/LwIP_UDP_Echo_Client/Inc/lwipopts.h              LwIP stack configuration options
  - LwIP/LwIP_UDP_Echo_Client/Inc/udp_echoclient.h        Header for UDP echoclient application
  - LwIP/LwIP_UDP_Echo_Client/Src/app_ethernet.c          Ethernet specific module
  - LwIP/LwIP_UDP_Echo_Client/Src/stm32f4xx_it.c          STM32 interrupt handlers
  - LwIP/LwIP_UDP_Echo_Client/Src/main.c                  Main program
  - LwIP/LwIP_UDP_Echo_Client/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - LwIP/LwIP_UDP_Echo_Client/Src/ethernetif.c            Interfacing LwIP to ETH driver
  - LwIP/LwIP_UDP_Echo_Client/Src/udp_echoclient.c        UDP echoclient application


@par Hardware and Software environment

  - This application runs on STM32F469xx and STM32F479xx devices.
    
  - This application has been tested with the following environments:
     - STM32469I-EVAL revB board   
     - echotool: (http://bansky.net/echotool/) is used as echo server that sends
       back every incoming data.   
      
  - STM32469I-EVAL Set-up
    - Connect the eval board to remote PC (through a crossover ethernet cable)
      or to your local network (through a straight ethernet cable)
  
  - Remote PC Set-up
    - Configure a static IP address for your remote PC 
      this address must be 192.168.0.11 


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
