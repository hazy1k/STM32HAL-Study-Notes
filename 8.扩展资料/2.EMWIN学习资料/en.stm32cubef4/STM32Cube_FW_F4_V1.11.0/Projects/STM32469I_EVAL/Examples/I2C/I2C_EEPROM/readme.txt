/**
  @page I2C_EEPROM I2C EEPROM DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2C/I2C_EEPROM/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the I2C EEPROM DMA example.
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

This example describes how to ensure I2C Data buffer transmission and reception 
with DMA. The communication is done with an I2C EEPROM memory.

SCL Pin: PB.8
SDA Pin: PB.9

   __________________________________________________________________________                        
  |           ______________                        ______________           |
  |          | I2C1         |                      |    I2C_EEPROM|          |
  |          |              |                      |              |          |
  |          |           SCL|______________________|CLOCK         |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |              |                      |              |          |
  |          |           SDA|______________________|DATA          |          |
  |          |              |                      |              |          |
  |          |______________|                      |______________|          |
  |      __                                                                  |
  |     |__|                                                                 |
  |    TAMPER                                                                |
  |                                                                          |
  |__________________________________________________________________________|
      STM32469I-EVAL

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO, DMA and NVIC). You may update this function to change I2C configuration.

The I2C/EEPROM communication is then initiated.
The HAL_I2C_Mem_Read_DMA() and the HAL_I2C_Mem_Write_DMA() functions allow respectively 
the reception of Data from EEPROM and the transmission of a predefined data 
buffer.

For this example the TxBuffer is predefined and the aRxBuffer size is same as aTxBuffer.

In a first step the I2C writes the aTxBuffer by group of 4 bytes (RF EEPROM 
Page size) using HAL_I2C_Mem_Write_DMA() then read back the data through aRxBuffer
using HAL_I2C_Mem_Read_DMA(). 
The end of this two steps are monitored through the HAL_I2C_GetState() function
result.
Finally, aRxBuffer and aRxBuffer are compared through Buffercmp() in order to 
check buffers correctness.  

STM32 Eval board's LEDs can be used to monitor the transfer status:
 - LED1 is ON when the transmission process is complete.
 - LED2 is ON when the reception process is complete.
 - LED3 is ON when there is an error in transmission/reception process.  

 @note I2Cx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.


@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents 

  - I2C/I2C_EEPROM/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2C/I2C_EEPROM/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - I2C/I2C_EEPROM/Inc/main.h                  Header for main.c module  
  - I2C/I2C_EEPROM/Src/stm32f4xx_it.c          Interrupt handlers
  - I2C/I2C_EEPROM/Src/main.c                  Main program
  - I2C/I2C_EEPROM/Src/system_stm32f4xx.c      STM32F4xx system source file
  - I2C/I2C_EEPROM/Src/stm32f4xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL revB board and can be
    easily tailored to any other supported device and development board.      

  - STM32469I-EVAL Set-up
    Make sure the ANT7-M24LR-A eeprom is connected to CN2 of the STM32469I-EVAL revB board
  

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain
 - Rebuild all files and load your image into target memory
 - Run the example


 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
