/**
  @page Audio_playback_and_record  Audio Playback and Record application

  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    Audio/Audio_playback_and_record/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the Audio Playback and Record application
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

@par Application Description 

This application shows how to use the different functionalities of Audio device 
and ST MEMS microphones (MP34DT01TR).

After having calibrated the touch screen, the application explores the wave files 
stored in the USB Key and it displays them on the LCD.
Via the touchscreen the user can choose between playing (triangle) and recording (circle).
 
1) Audio Player
In Audio Play mode any wave file stored under the USB Key can be opened using the FatFs 
file system and transferred to the internal SRAM using the file system. All the wave
files properties are read from the Wave File Header.
Plug a headphone to ear the sound  /!\ Take care of yours ears. Default volume is 70%.

To play the audio file, a buffer is used between USB and audio codec. This buffer 
is used like a ping-pong buffer to be able to read regularly data from USB to be 
sent to audio codec: 
- Using FatFs driver, a 1st read of AUDIO_BUFFER_SIZE bytes is done
- This buffer is sent to audio BSP which uses DMA mode to transfer data from MCU
to audio codec (16 bits)
- At DMA half transfer ISR, an indication is sent to application to indicate that
half of the buffer has been sent
- At DMA half transfer ISR exit, application can read (AUDIO_BUFFER_SIZE/2) bytes from USB to fill
the 1st part of the buffer
- At DMA transfer complete ISR, DMA transfer is requested to send 1st part of the
buffer
- At DMA transfer complete ISR exit, application can read (AUDIO_BUFFER_SIZE/2) bytes from USB to fill
the 2nd part of the buffer
- When file size is reached, audio codec is stopped and a new play is requested.

A control interface through touchscreen is proposed with the following options;
In order from the left the the right: 
     - Volume down
     - Pause
     - Stop
     - Previous wave file
     - Next wave file
     - Volume up

Note: The audio files provided under "/Utilities/Media/Audio" are based on a free 
music download from www.DanoSongs.com website and user can load his own audio 
(*.wav) files in the USB Key root.
 
2) Audio Recorder 
Two MEMS microphone MP34DT01TR mounted on STM32469I-Discovery are connected
to the Inter-IC Sound (I2S) peripheral. The I2S is configured in master
receiver mode. In this mode, the I2S peripheral provides the clock to an embedded
timer (TIM3 in this application) which divides the I2S clock. This clock is delivered
to the MEMS microphone through CLK. Data acquired (Audio samples) from the MEMS 
microphone through DOUT.

Data acquisition is performed in 16KHz and 16-bit PDM format and DMA is used to
transfer data from I2S peripheral to internal SRAM. In order to avoid data-loss, one buffer
split into two halves is used:

When the first half of the buffer is filled (half transfer complete), the PDM
samples are converted to PCM format while the second half of the buffer is being
used to receive data from Audio samples.

When the second half of the buffer is filled (transfer complete), the PDM
samples are converted to PCM format while the first half of the buffer is being
used to receive data from Audio samples. This process is repeated as long as the 
application is running.

To avoid data corruption:
- DMA ISR priority must be set at a Lower priority than USB interrupt sources,
  this priority order must be respected when managing other interrupts;
- The processing time of converting/filtering samples from PDM to PCM 
  PDM_Filter_64_LSB()) should be lower than the time required to fill a
  single buffer.

In main loop of the application, when a buffer is full with PCM format it is written
in USB key using the free file system FatFS. Recorded file is named WAVE.WAV.

A control interface through touchscreen is proposed with the following options;
In order from the left the the right: 
     - Volume down
     - Pause
     - Stop recording
     - Record
     - Volume up

Note that a PDM Audio software decoding library provided in binary is used in 
this application. For IAR EWARM toolchain, the library is labeled 
"libPDMFilter_IAR.a".

@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.


@par Directory contents 

  - Audio_playback_and_record/Src/main.c               Main program
  - Audio_playback_and_record/Src/stm32f4xx_it.c       Interrupt handlers
  - Audio_playback_and_record/Src/system_stm32f4xx.c   STM32F4xx system source file
  - Audio_playback_and_record/Src/usbh_conf.c          Board support package for the USB host library
  - Audio_playback_and_record/Src/usbh_diskio.c        USB disk interface
  - Audio_playback_and_record/Src/ts_calibration.c     Calibration phase for the touchscreen
  - Audio_playback_and_record/Src/menu.c               Start page menu
  - Audio_playback_and_record/Src/explorer.c           Search and display the wave file on the mass storage
  - Audio_playback_and_record/Src/waveplayer.c         Audio play file
  - Audio_playback_and_record/Src/waverecorder.c       Audio record file
  - Audio_playback_and_record/Inc/ffconf.h             FAT file system module configuration file   
  - Audio_playback_and_record/Inc/main.h               Main program header file
  - Audio_playback_and_record/Inc/stm32f4xx_hal_conf.h HAL configuration file
  - Audio_playback_and_record/Inc/stm32f4xx_it.h       Interrupt handlers header file
  - Audio_playback_and_record/Inc/usbh_conf.h          USB driver Configuration file
  - Audio_playback_and_record/Inc/waveplayer.h         Audio play header file
  - Audio_playback_and_record/Inc/waverecorder.h       Audio record header file
      
      
@par Hardware and Software environment

  - This application runs on STM32F469xx devices.
    
  - This application has been tested with STM32469I-Discovery board and can be
    easily tailored to any other supported device and development board.   

  - STM32469I-Discovery Set-up
    - Please ensure that jumpers JP4 is in the position "AUDIO"
    - Plug the USB key into the STM32469I-EVAL board through 'USB micro A-Male 
      to A-Female' cable (FS mode: connector CN13).
    - Load .wav audio file (audio_sample.wav) available under "/Utilities/Media/Audio" to the root of USB mass storage


@par How to use it ? 

In order to make the program work, you must do the following:
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - Run the application
 
 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
