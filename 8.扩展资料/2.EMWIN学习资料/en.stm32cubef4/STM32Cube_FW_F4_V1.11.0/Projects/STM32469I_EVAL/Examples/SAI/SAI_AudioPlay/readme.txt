/**
  @page SAI_AudioPlay  Description of the SAI audio play example
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    SAI/SAI_AudioPlay/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the SAI audio play example.
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

This example show how to play an audio file using the DMA circular mode and 
how to handle the buffer update.

      Plug a headphone to ear the sound  /!\ Take care of yours ears.
      Default volume is 20%.
      The audio file is played in loop
      @Note: Copy file 'audio.bin' (available in AudioFile) directly in the flash 
      at @0x08080000 using ST-Link utility
      The sound is output on Audio_out1 connector CN27 of  STM32469I-EVAL.
      
In case of error, LED3 is on.
Once started, LED1 is toggling on STM32469I-EVAL.

@note Note the DMA runs in circular buffer mode and never stops. If you break with 
      the debugger, the DMA hw will keep running and a noise will be heard.

@note This example does not use BSP_AUDIO so the MspInit is coded in the main.c.

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents  

  - SAI/SAI_AudioPlay/Src/main.c                  Main program
  - SAI/SAI_AudioPlay/Src/system_stm32f4xx.c      STM32F4xx system source file
  - SAI/SAI_AudioPlay/Src/stm32f4xx_it.c          Interrupt handlers
  - SAI/SAI_AudioPlay/Inc/main.h                  Main program header file
  - SAI/SAI_AudioPlay/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - SAI/SAI_AudioPlay/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - SAI/SAI_AudioPlay/AudioFile/audio.bin         Audio wave extract.

@par Hardware and Software environment

  - This example runs on STM32F469xx/STM32F479xx devices.

  - This example has been tested and validated with STMicroelectronics STM32469I-EVAL revB 
    board and can be easily tailored to any other supported device
    and development board.      

  - STM32469I-EVAL Set-up :
    There are no special switches for this example 

@par How to use it ? 

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the example 

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
