/**
  @page LibJPEG_Decoding LibJPEG decoding application
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    LibJPEG/LibJPEG_Decoding/readme.txt 
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   Description of the LibJPEG decoding application.
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
   @endverbatim

@par Application Description

This application demonstrates how to read jpeg file from USB disk, decode it and display
the final BMP image on the LCD.

The JPEG images should be copied at the USB disk root.

The images must have the following properties:
 - named as "image.jpg"
 - 240*320 size

The image size can be modified by changing IMAGE_WIDTH and IMAGE_HEIGHT defines 
in "main.h" file to decode other resolution than 240x320.

Uncomment "SWAP_RB" define in "main.h" file if the displayed image color 
are inverted

The image transferring to the LCD_BUFFER can be done with two ways; 
with CPU or with DMA2D. Uncomment "USE_DMA2D" define for DMA2D transfer and 
"DONT_USE_DMA2D" for CPU transfer in main.h file.
   
@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents

    - LibJPEG/LibJPEG_Decoding/Inc/main.h                      Main program header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32f4xx_it.h              Interrupt handlers header file
    - LibJPEG/LibJPEG_Decoding/Inc/stm32f4xx_hal_conf.h        HAL Configuration file 
    - LibJPEG/LibJPEG_Decoding/Inc/ffconf.h                    FAT file system module configuration file
    - LibJPEG/LibJPEG_Decoding/Inc/decode.h                    Decoder header file
    - LibJPEG/LibJPEG_Decoding/Inc/jmorecfg.h                  Lib JPEG configuration file (advanced configuration)
    - LibJPEG/LibJPEG_Decoding/Inc/jconfig.h                   Lib JPEG configuration file
    - LibJPEG/LibJPEG_Decoding/Src/main.c                      Main program  
    - LibJPEG/LibJPEG_Decoding/Src/stm32f4xx_it.c              Interrupt handlers
    - LibJPEG/LibJPEG_Decoding/Src/system_stm32f4xx.c          STM32F4xx system clock configuration file
    - LibJPEG/LibJPEG_Decoding/Src/decode.c                    Decoder file


@par Hardware and Software environment

  - This application runs on STM32F429xx Devices.
    
  - This application has been tested with STM32F429I-Discovery RevB board and can be
    easily tailored to any other supported device and development board.  

  - STM32F429I-DISCO RevB Set-up
    - Plug the USB key into the STM32F429I-DISCO board through 'USB micro A-Male 
      to A-Female' cable.


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
                                   
