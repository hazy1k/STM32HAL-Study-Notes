/**
  @page CRC_Example Cyclic Redundancy Check Example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    CRC/CRC_Example/readme.txt 
  * @author  MCD Application Team
  * @version V1.1.3
  * @date    29-January-2016
  * @brief   Description of Cyclic Redundancy Check Example.
  ******************************************************************************
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
  @endverbatim

@par Example Description 

This example guides you through the different configuration steps by mean of HAL API 
to ensure the use of the CRC (Cyclic Redundancy Check) calculation unit to get a
CRC code of a given buffer of data words (32-bit long), based on a fixed generator
polynomial (0x4C11DB7).

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The CRC peripheral configuration is ensured by HAL_CRC_Init() function.
The latter is calling HAL_CRC_MspInit() function which core is implementing
the configuration of the needed CRC resources according to the used hardware (CLOCK). 
You can update HAL_CRC_Init() input parameters to change the CRC configuration.

The calculated CRC code is stored in uwCRCValue variable
and compared with the expected one stored in uwExpectedCRCValue variable.

STM32 Eval board LEDs are used to monitor the example status:
  - LED1 (GREEN) is ON when a correct CRC value is calculated
  - LED3 (RED) is ON when an incorrect CRC value is calculated or when there is an initialization error.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@par Directory contents 
  
  - CRC/CRC_Example/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - CRC/CRC_Example/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - CRC/CRC_Example/Inc/main.h                  Header for main.c module
  - CRC/CRC_Example/Src/stm32f4xx_it.c          Interrupt handlers
  - CRC/CRC_Example/Src/main.c                  Main program
  - CRC/CRC_Example/Src/stm32f4xx_hal_msp.c     HAL MSP module 
  - CRC/CRC_Example/Src/system_stm32f4xx.c      STM32F4xx system source file

     
@par Hardware and Software environment

  - This example runs on STM32F446xx devices.
  
  - This example has been tested with an STMicroelectronics STM32446E-EVAL
    board and can be easily tailored to any other supported device 
    and development board.

@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
 