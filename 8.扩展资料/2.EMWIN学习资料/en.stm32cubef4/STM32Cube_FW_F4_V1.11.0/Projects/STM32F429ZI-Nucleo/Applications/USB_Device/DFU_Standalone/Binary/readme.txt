/**
  @page Binary Description of the binary template
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/DFU_Standalone/Binary/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    29-January-2016
  * @brief   Description of the binary template application.
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

This directory contains a binary template (in DFU format) to be loaded into Flash memory using Device 
Firmware Upgrade application. This file was converted to the DFU format using the "DFU File Manager Tool"
included in the "DfuSe" PC software install.
For more details on how to convert a .bin file to DFU format please refer to the UM0412 user manual 
"Getting started with DfuSe USB device firmware upgrade STMicroelectronics extension" available from the
STMicroelectronics microcontroller website www.st.com.
   
This binary is a simple LEDs chaser. The LED1 and LED2 toggle each 100 ms.
The system Timer (Systick) is used for to generate the delay.
The offset address of this binary is 0x08008000 which matches the definition in DFU application
"USBD_DFU_APP_DEFAULT_ADD".


@par Hardware and Software environment

  - This application runs on STM32F429ZI devices.
    
  - This application has been tested with STMicroelectronics NUCLEO-F429ZI board
     and can be easily tailored to any other supported device and boards.  

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
