/**
  @page FreeRTOS_Semaphore FreeRTOS Semaphore application
 
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    FreeRTOS/FreeRTOS_Semaphore/readme.txt
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the FreeRTOS Semaphore application.
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

@par Description

This directory contains a set of source files that implement an application that
uses semaphores with CMSIS RTOS API 

This application creates two threads that toggle LEDs through a shared semaphore.

The first thread, having the higher priority, obtains the semaphore and 
toggles the LED1 each 200 ms. After 5 seconds it releases the semaphore and 
suspends itself. The low priority thread can execute now, it obtains the 
semaphore and resumes execution of the first thread. As it has the higher 
priority, the first thread will try to obtain the semaphore but it fails 
because the semaphore is already taken by the low 
priority thread. The latter will toggle the LED3 for 5 seconds before releasing 
the semaphore and begin a new cycle


@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
	  
@note The FreeRTOS heap size configTOTAL_HEAP_SIZE defined in FreeRTOSConfig.h is set according to the OS resources memory requirements of the application with +10% margin and rounded to the upper Kbyte boundary.

For more details about FreeRTOS implementation on STM32Cube, please refer to UM1722 "Developing Applications 
on STM32Cube with RTOS".


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents

    - FreeRTOS/FreeRTOS_Semaphore/Inc/main.h                Main program header file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32f4xx_hal_conf.h  HAL Library Configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/stm32f4xx_it.h        Interrupt handlers header file
    - FreeRTOS/FreeRTOS_Semaphore/Inc/FreeRTOSConfig.h      FreeRTOS Configuration file
    - FreeRTOS/FreeRTOS_Semaphore/Src/main.c                Main program
    - FreeRTOS/FreeRTOS_Semaphore/Src/stm32f4xx_it.c        Interrupt handlers
    - FreeRTOS/FreeRTOS_Semaphore/Src/system_stm32f4xx.c    STM32F4xx system clock configuration file


@par Hardware and Software environment

  - This application runs on STM32F469xx and STM32F479xx devices.
  
  - This application has been tested with STM32469I-EVAL revB board and can be
    easily tailored to any other supported device and development board.


@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
