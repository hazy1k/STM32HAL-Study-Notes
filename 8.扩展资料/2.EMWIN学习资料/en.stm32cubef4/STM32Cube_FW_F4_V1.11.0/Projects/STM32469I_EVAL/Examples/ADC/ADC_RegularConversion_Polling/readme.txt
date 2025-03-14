/**
  @page ADC_RegularConversion_Polling conversion using Polling

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    ADC/ADC_RegularConversion_Polling/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the ADC RegularConversion Polling example.
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

This example describes how to use the ADC3 with channel ADC_CHANNEL_10 in Polling mode to convert data.

When the end of conversion occurs, the converted data of ADC3 DR register is 
affected to the uhADCxConvertedValue variable.

Conversion time (reference manual, "Reset and clock control" and "ADC clocks" sections):
  1) the system clock is 180 MHz. APB2 = 90MHz and ADC clock = APB2/4
  2) __HAL_RCC_ADC_CONFIG() macro in HAL_ADC_MspInit() API sets the system clock as ADC asynchronous clock source 
  2) ClockPrescaler field of initialization structure is set to ADC_CLOCKPRESCALER_PCLK_DIV2
     => the input ADC clock is set in asynchronous clock mode   
     => the input ADC clock is the undivided system clock 


  Sampling time is set to ADC_SAMPLETIME_6CYCLES_5 (6.5 cycles).
  ConvTime = Sampling time + 12.5 ADC clock cycles.
           = 19 clock cycles
           = (19 / 22.5 MHz) = 844.4 ns 
                    

Please select PC0 option on JP3 jumper called Potentiometer
User can vary the ADC_CHANNEL_10 voltage using the Eval Board potentiometer (RV1) connected to PC.0.
The converted value is monitored through debugger: uhADCxConvertedValue variable.

STM32 Eval board's LEDs can be used to monitor the transfer status:
  - LED3 is ON when there is an error in initialization.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents 

  - ADC/ADC_RegularConversion_Polling/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - ADC/ADC_RegularConversion_Polling/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - ADC/ADC_RegularConversion_Polling/Inc/main.h                  Header for main.c module  
  - ADC/ADC_RegularConversion_Polling/Src/stm32f4xx_it.c          Interrupt handlers
  - ADC/ADC_RegularConversion_Polling/Src/main.c                  Main program
  - ADC/ADC_RegularConversion_Polling/Src/stm32f4xx_hal_msp.c     HAL MSP file 
  - ADC/ADC_RegularConversion_Polling/Src/system_stm32f4xx.c      STM32F4xx system source file

@par Hardware and Software environment 

  - This example runs on STM32F469xx/STM32F479xx devices.
  
  - This example has been tested with STM32469I-EVAL revB board and can be
    easily tailored to any other supported device and development board.

  - STM32469I-EVAL revB Set-up
    - Use the Potentiometer (RV1) of the Eval board (connected to  PC.0).
    - Make sure that JP3 is fitted in 2-3 positions to use potentiometer.
    
@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
