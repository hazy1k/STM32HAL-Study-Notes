/**
  @page I2C_TwoBoards_AdvComIT I2C Two Boards Advanced Communication IT example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    I2C/I2C_TwoBoards_AdvComIT/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.2
  * @date    29-January-2016
  * @brief   Description of the I2C Two Boards Advanced Communication with 
  *          Interrupt example.
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
using Interrupt. The communication is done with 2 Boards through I2C.

   _________________________                        _________________________
  |       __________________|                      |_________________        |
  |      |     I2C1         |                      |          I2C1   |       |
  |      |                  |                      |                 |       |
  |      |  CN5.D15 SCL(PB8)|______________________|(PB8)SCL CN5.D15 |       |
  |      |                  |                      |                 |       |
  |      |                  |                      |                 |       |
  |      |                  |                      |                 |       |
  |      |  CN5.D14 SDA(PB9)|______________________|(PB9)SDA CN5.D14 |       |
  |      |                  |                      |                 |       |
  |      |__________________|                      |_________________|       |
  |      __                 |                      |             __          |
  |     |__|                |                      |            |__|         |
  |     USER             GND|______________________|GND         USER         |
  |                         |                      |                         |
  |__STM32F469I_DISCOVERY___|                      |__STM32F469I_DISCOVERY___|

At the beginning of the main program the HAL_Init() function is called to reset 
all the peripherals, initialize the Flash interface and the systick.
Then the SystemClock_Config() function is used to configure the system
clock (SYSCLK) to run at 180 MHz.

The I2C peripheral configuration is ensured by the HAL_I2C_Init() function.
This later is calling the HAL_I2C_MspInit()function which core is implementing
the configuration of the needed I2C resources according to the used hardware (CLOCK, 
GPIO and NVIC). You may update this function to change I2C configuration.

The I2C communication is then initiated.
The project is split in two parts: the Master Board and the Slave Board
- Master Board
  The HAL_I2C_Master_Receive_IT() and the HAL_I2C_Master_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Master mode using interrupt.
- Slave Board
  The HAL_I2C_Slave_Receive_IT() and the HAL_I2C_Slave_Transmit_IT() functions 
  allow respectively the reception and the transmission of a predefined data buffer
  in Slave mode using interrupt.
The user can choose between Master and Slave through "#define MASTER_BOARD"
in the "main.c" file:
- Uncomment "#define MASTER_BOARD" to select Master board.
- Comment "#define MASTER_BOARD" to select Slave board.

For this example two buffers are used 
- aTxBuffer buffer contains the data to be transmitted 
- aRxBuffer buffer is used to save the received data
Note that both buffers have same size
                       
On Master board side:
 - Wait User push-button to be pressed (used for only synchronization at startup)
 - Request write operation by sending specific command "MASTER_REQ_WRITE" to Slave
 - Send the number of data to be written
 - Transmit aTxBuffer buffer to slave
 - Request read operation by sending specific command "MASTER_REQ_READ" to Slave
 - Send the number of data to be read
 - Receive data from Slave in aRxBuffer
 - Check the correctness of data and Toggle LED1 when data is received correctly,
   otherwise LED3 is turned ON and communication is stopped (using infinite loop)


On Slave board side:
 - Receive request from Master
 - Receive the request operation from Master and depending on the operation type (write or read):
   - If Master requests write operation:
      - Receive number of data to be written by Master
      - Receive data from master in aRxBuffer
      - Check the correctness of data and Toggle LED1 when data is received correctly,
        otherwise LED3 is turned ON and communication is stopped (using infinite loop)
   - If Master request read operation:
      - Receive number of data to be written to Master
      - Transmit aTxBuffer buffer to master
   
These operations are repeated periodically and the start of communication is triggered 
by pushing the key button of the Master board.  

 @note In Master side, only Acknowledge failure error is handled. When this error
       occurs Master restart the current operation until Slave acknowledges it's
       address.
        
 @note I2Cx instance used and associated resources can be updated in "main.h"
       file depending hardware configuration used.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.

@note The application need to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.

@note The connection of the LCD reset pin to a dedicated GPIO PH7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents 

  - I2C/I2C_TwoBoards_AdvComIT/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - I2C/I2C_TwoBoards_AdvComIT/Inc/stm32f4xx_it.h          I2C interrupt handlers header file
  - I2C/I2C_TwoBoards_AdvComIT/Inc/main.h                  Header for main.c module  
  - I2C/I2C_TwoBoards_AdvComIT/Src/stm32f4xx_it.c          I2C interrupt handlers
  - I2C/I2C_TwoBoards_AdvComIT/Src/main.c                  Main program
  - I2C/I2C_TwoBoards_AdvComIT/Src/system_stm32f4xx.c      STM32F4xx system source file
  - I2C/I2C_TwoBoards_AdvComIT/Src/stm32f4xx_hal_msp.c     HAL MSP file    


@par Hardware and Software environment

  - This example runs on STM32F469xx devices.
    
  - This example has been tested with STM32469I-DISCOVERY board and can be
    easily tailored to any other supported device and development board.    

  -STM32469I-DISCOVERY Set-up
    - Connect Master board PB8 connected to CN5.D15 to Slave Board PB8 connected to CN5.D15
    - Connect Master board PB9 connected to CN5.D14 to Slave Board PB9 connected to CN5.D14
    - Connect Master board GND to Slave Board GND

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
    o Uncomment "#define MASTER_BOARD" and load the project in Master Board
    o Comment "#define MASTER_BOARD" and load the project in Slave Board
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
