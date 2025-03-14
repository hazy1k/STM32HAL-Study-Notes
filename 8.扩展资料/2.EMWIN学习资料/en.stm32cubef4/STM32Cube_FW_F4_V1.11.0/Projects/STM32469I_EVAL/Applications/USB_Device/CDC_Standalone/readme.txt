/**
  @page CDC_Standalone USB Device Communication (CDC) application
  
  @verbatim
  ******************** (C) COPYRIGHT 2016 STMicroelectronics *******************
  * @file    USB_Device/CDC_Standalone/readme.txt 
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Description of the USB Device CDC application.
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

This application is a part of the USB Device Library package using STM32Cube firmware. It describes how to 
use USB device application based on the Device Communication Class (CDC) following the PSTN subprotocol
in the STM32F469xx devices using the OTG-USB and UART peripherals.

This is a typical application on how to use the STM32F469xx USB OTG Device peripheral where the STM32 MCU
behaves as a USB-to-RS232 bridge following the Virtual COM Port (VCP) implementation.
 - On one side, the STM32 exchanges data with a PC host through USB interface in Device mode.
 - On the other side, the STM32 exchanges data with other devices (same host, other host,
   other devices�) through the UART interface (RS232).

At the beginning of the main program the HAL_Init() function is called to reset all the peripherals,
initialize the Flash interface and the systick. The user is provided with the SystemClock_Config()
function to configure the system clock (SYSCLK) to run at 180 MHz. The Full Speed (FS) USB module uses
internally a 48-MHz clock which is coming from a specific output of two PLLs PLL or PLL SAI.
In the High Speed (HS) mode the USB clock (60 MHz) is driven by the ULPI.

The 48 MHz clock for the USB FS can be derived from one of the two following sources:
  � PLL clock (clocked by the HSE): If the USB uses the PLL as clock source, the PLL clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLVCO/PLLQ).
  � PLLSAI clock (clocked by the HSE): If the USB uses the PLLSAI as clock source, the PLLSAI clock must be programmed
    to output 48 MHz frequency (USBCLK = PLLSAIVCO/PLLSAIP).

When the VCP application is started, the STM32 MCU is enumerated as serial communication port and is
configured in the same way (baudrate, data format, parity, stop bit) as it would configure a standard 
COM port. The 7-bit data length with no parity control is NOT supported.

During enumeration phase, three communication pipes "endpoints" are declared in the CDC class
implementation (PSTN sub-class):
 - 1 x Bulk IN endpoint for receiving data from STM32 device to PC host:
   When data are received over UART they are saved in the buffer "UserTxBuffer". Periodically, in a 
   timer callback the state of the buffer "UserTxBuffer" is checked. If there are available data, they
   are transmitted in response to IN token otherwise it is NAKed.
   The polling period depends on "CDC_POLLING_INTERVAL" value.
    
 - 1 x Bulk OUT endpoint for transmitting data from PC host to STM32 device:
   When data are received through this endpoint they are saved in the buffer "UserRxBuffer" then they
   are transmitted over UART using interrupt mode and in meanwhile the OUT endpoint is NAKed.
   Once the transmission is over, the OUT endpoint is prepared to receive next packet in
   HAL_UART_TxCpltCallback().
    
 - 1 x Interrupt IN endpoint for setting and getting serial-port parameters:
   When control setup is received, the corresponding request is executed in CDC_Itf_Control().
   In this application, two requests are implemented:
    - Set line: Set the bit rate, number of Stop bits, parity, and number of data bits 
    - Get line: Get the bit rate, number of Stop bits, parity, and number of data bits
   The other requests (send break, control line state) are not implemented.

@note Receiving data over UART is handled by interrupt while transmitting is handled by DMA allowing
      hence the application to receive data at the same time it is transmitting another data (full- 
      duplex feature).

The support of the VCP interface is managed through the ST Virtual COM Port driver available for 
download from www.st.com.

@note The user has to check the list of the COM ports in Device Manager to find out the number of the
      COM ports that have been assigned (by OS) to the VCP interface.

This application uses UART as a communication interface. The UART instance and associated resources
(GPIO, NVIC) can be tailored in "usbd_cdc_interface.h" header file according to your hardware 
configuration. Moreover, this application can be customized to communicate with interfaces other than UART.
For that purpose a template CDC interface is provided in: 
Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Src directory.

In High Speed (HS) mode, enabling USB-DMA will result in data being sent only by multiple of 4 packet 
sizes.  This is due to the fact that USB DMA does not allow sending data from non word-aligned addresses.
For this specific application, it is advised to not enable the DMA capability unless required.

To run this application, the user can use one of the following configurations:

 - Configuration 1: 
   Connect USB cable to host and UART (RS232) to a different host (PC or other device) or to same host.
   In this case, you can open two hyperterminals to send/receive data to/from host to/from device.
   
 - Configuration 2: 
   Connect USB cable to Host and connect UART TX pin to UART RX pin on the STM32469I-EVAL board
   (Loopback mode). In this case, you can open one terminal (relative to USB com port or UART com port)
   and all data sent from this terminal will be received by the same terminal in loopback mode.
   This mode is useful for test and performance measurements.

@note If using this example in loopback mode (ie. USART Tx IO connected to USART Rx IO on STM32 side) 
and with baudrates higher than 9600, there might be communication errors.

@note Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
      based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
      a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
      than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
      To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
      
@note The application needs to ensure that the SysTick time base is always set to 1 millisecond
      to have correct HAL operation.
      
For more details about the STM32Cube USB Device library, please refer to UM1734 
"STM32Cube USB Device library".
      

@par USB Library Configuration

To select the appropriate USB Core to work with, user must add the following macro defines within the
compiler preprocessor (already done in the preconfigured projects provided with this application):
      - "USE_USB_HS" when using USB High Speed (HS) Core
      - "USE_USB_FS" when using USB Full Speed (FS) Core 
      - "USE_USB_HS" and "USE_USB_HS_IN_FS" when using USB High Speed (HS) Core in FS mode
      

@note The connection of the LCD reset pin to a dedicated GPIO PK7 instead of the STM32F469 NRST pin may cause residual display on LCD with applications/examples that do not require display.
	  The LCD clear can be ensured by hardware through the board's power off/power on or by software calling the BSP_LCD_Reset() function.

@par Directory contents 

  - USB_Device/CDC_Standalone/Src/main.c                  Main program
  - USB_Device/CDC_Standalone/Src/system_stm32f4xx.c      STM32F4xx system clock configuration file
  - USB_Device/CDC_Standalone/Src/stm32f4xx_it.c          Interrupt handlers
  - USB_Device/CDC_Standalone/Src/stm32f4xx_hal_msp.c     HAL MSP module
  - USB_Device/CDC_Standalone/Src/usbd_cdc_interface.c    USBD CDC interface
  - USB_Device/CDC_Standalone/Src/usbd_conf.c             General low level driver configuration
  - USB_Device/CDC_Standalone/Src/usbd_desc.c             USB device CDC descriptor
  - USB_Device/CDC_Standalone/Inc/main.h                  Main program header file
  - USB_Device/CDC_Standalone/Inc/stm32f4xx_it.h          Interrupt handlers header file
  - USB_Device/CDC_Standalone/Inc/stm32f4xx_hal_conf.h    HAL configuration file
  - USB_Device/CDC_Standalone/Inc/usbd_conf.h             USB device driver Configuration file
  - USB_Device/CDC_Standalone/Inc/usbd_desc.h             USB device MSC descriptor header file
  - USB_Device/CDC_Standalone/Inc/usbd_cdc_interface.h    USBD CDC interface header file  


@par Hardware and Software environment

  - This application runs on STM32F469xx and STM32F479xx devices.
    
  - This application has been tested with STMicroelectronics STM32469I-EVAL RevB
    evaluation boards and can be easily tailored to any other supported device 
    and development board.

  - STM32469I-EVAL RevB Set-up
    - Connect the STM32469I_EVAL board to the PC through 'USB micro A-Male 
      to A-Male' cable to the connector:
      - CN8 : to use USB High Speed (HS) 
      - CN13: to use USB Full Speed (FS)
      - CN14: to use USB HS-IN-FS.
              Note that some FS signals are shared with the HS ULPI bus, so some PCB rework is needed.
              For more details, refer to section "USB OTG2 HS & FS" in STM32469I_EVAL Evaluation Board 
              User Manual.
        @note Make sure that :
         - jumper JP9 must be removed when using USB OTG FS
    - Connect the STM32469I-EVAL board to the PC (or to another evaluation board) through RS232 (USART)
      serial cable CN7 (USART1) connector.
@note Make sure that :
    - jumper JP8  is on RS232_RX position (1-2)  and 
    - jumper JP15 is on USART1_RX position (1-2) and 
    - jumper JP19 is on USART1_TX position (1-2).
    - For loopback mode test: remove RS232 cable on CN7 and connect directly USART TX and RX pins:
      PA9 and PA10 (with a cable or a jumper)
      
    - Hyperterminal configuration:
      - Word Length = 8 Bits
      - Stop Bit    = One Stop bit
      - Parity      = No parity
      - BaudRate    = 115200 baud
      - Hardware flow control disabled (RTS and CTS signals) */


@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain 
 - Rebuild all files and load your image into target memory
 - In the workspace toolbar select the project configuration:
   - STM32469I-EVAL_USBH-HS: to configure the project for STM32F469xx devices using USB OTG HS peripheral
   - STM32469I-EVAL_USBH-FS: to configure the project for STM32F469xx devices using USB OTG FS peripheral
   - STM32469I-EVAL_USBH-HS-IN-FS: to configure the project for STM32F469xx devices and use USB OTG HS 
                                   peripheral In FS (using embedded PHY).
 - Run the application
 - Install the USB virtual COM port driver
 - Find out the number of the COM port assigned to the STM32 CDC device
 - Open a serial terminal application and start the communication

 * <h3><center>&copy; COPYRIGHT STMicroelectronics</center></h3>
 */
