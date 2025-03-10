/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Decoding/Src/main.c
  * @author  MCD Application Team
  * @version V1.0.3
  * @date    29-January-2016
  * @brief   Main program body
  *          This sample code shows how to decompress JPEG file.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
FATFS SDFatFs;  /* File system object for SD card logical drive */
FIL MyFile;     /* File object */
char SDPath[4]; /* SD card logical drive path */

RGB_typedef *RGB_matrix;
uint8_t _aucLine[2048];
uint32_t line_counter = 0;
uint16_t Xpos = 0;
uint16_t Ypos = 0;

/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength);
static void LCD_Config(void);
static void Error_Handler(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
   uint8_t  sdram_status = SDRAM_OK;

   /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, instruction and Data caches
       - Configure the Systick to generate an interrupt each 1 msec
       - Set NVIC Group Priority to 4
       - Global MSP (MCU Support Package) initialization
     */
  HAL_Init();

  /* Configure the system clock to 180 MHz */
  SystemClock_Config();

  /* Initialize IO expander */
  BSP_IO_Init();

  /* Configure LED3 */
  BSP_LED_Init(LED3);

  /* Initialize the SDRAM */
  sdram_status = BSP_SDRAM_Init();
  if(sdram_status != SDRAM_OK)
  {
	  Error_Handler();
  }

  /*###########################################################################
  When the uSD Card is used; the Camera module must be unplugged, this is due
  to the shared pins between the two devices.

  Otherwise, you have to set camera sensor in Power Down mode, by calling the
  BSP_CAMERA_PwrDown() available under stm32469i_eval_camera.c BSP driver */

  /* Set camera sensor in Power Down mode */
  BSP_CAMERA_PwrDown();

  /*##-1- LCD Configuration ##################################################*/
  LCD_Config();

  /*##-2- Link the micro SD disk I/O driver ##################################*/
  if(FATFS_LinkDriver(&SD_Driver, SDPath) == 0)
  {
    /*##-3- Register the file system object to the FatFs module ##############*/
    if(f_mount(&SDFatFs, (TCHAR const*)SDPath, 0) == FR_OK)
    {
      /*##-4- Open the JPG image with read access ############################*/
       if(f_open(&MyFile, "image.jpg", FA_READ) == FR_OK)
       {
       }
       else
       {
         Error_Handler();
       }
     }
  }
  else
  {
    Error_Handler();
  }

  /*##-5- Decode the jpg image file ##########################################*/
  jpeg_decode(&MyFile, IMAGE_WIDTH, _aucLine, Jpeg_CallbackFunction);

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  LCD Configuration.
  * @retval None
  */

static void LCD_Config(void)
{
  uint8_t lcd_status = LCD_OK;

  //  BSP_LCD_InitEx(LCD_ORIENTATION_PORTRAIT); /* uncomment if Portrait orientation is needed */
  BSP_LCD_Init(); /* Uncomment if default config (landscape orientation) is needed */
  while(lcd_status != LCD_OK);

  BSP_LCD_LayerDefaultInit(0, LCD_FB_START_ADDRESS);   
  BSP_LCD_SelectLayer(0);   

  /* Clear the LCD Background layer */
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  BSP_LCD_SetFont(&LCD_DEFAULT_FONT);

  /* Clear the LCD */
  BSP_LCD_SetBackColor(LCD_COLOR_WHITE);
  BSP_LCD_Clear(LCD_COLOR_WHITE);

  /* Set the LCD Text Color */
  BSP_LCD_SetTextColor(LCD_COLOR_DARKBLUE);

  /* Display LCD messages */
  BSP_LCD_DisplayStringAt(0, 430, (uint8_t *)"Application LibJPEG_Decoding", CENTER_MODE);

   /* Compute centered position to draw on screen the decoded pixels */
   Xpos = (uint16_t)((BSP_LCD_GetXSize() - IMAGE_WIDTH) / 2);
   Ypos = (uint16_t)((BSP_LCD_GetYSize() - IMAGE_HEIGHT) / 2);
}

/**
  * @brief  Copy decompressed data to display buffer.
  * @param  Row: Output row buffer
  * @param  DataLength: Row width in output buffer
  * @retval None
  */
static uint8_t Jpeg_CallbackFunction(uint8_t* Row, uint32_t DataLength)
{
  uint32_t i = 0;
  RGB_matrix =  (RGB_typedef*)Row;
  uint32_t  ARGB8888Buffer[IMAGE_WIDTH];

#ifdef SWAP_RB
  for(i = 0; i < IMAGE_WIDTH; i++)
  {
	  ARGB8888Buffer[i]  = (uint32_t)
      (
        0xFF000000                                       |
       (((uint32_t)(RGB_matrix[i].B) & 0x000000FF) >> 0) |
       (((uint32_t)(RGB_matrix[i].G) & 0x000000FF) << 8) |
       (((uint32_t)(RGB_matrix[i].R) & 0x000000FF) << 16)
      );

    BSP_LCD_DrawPixel((i + Xpos), (line_counter + Ypos), ARGB8888Buffer[i]);
  }
#else
  for(i = 0; i < IMAGE_WIDTH; i++)
  {
	  ARGB8888Buffer[i]  = (uint32_t)
      (
        0xFF000000                                       |
       (((uint32_t)(RGB_matrix[i].R) & 0x000000FF) >> 0) |
       (((uint32_t)(RGB_matrix[i].G) & 0x000000FF) << 8) |
       (((uint32_t)(RGB_matrix[i].B) & 0x000000FF) << 16)
      );

    BSP_LCD_DrawPixel((i + Xpos), (line_counter + Ypos), ARGB8888Buffer[i]);
  }
#endif

  line_counter++;
  return 0;
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 180000000
  *            HCLK(Hz)                       = 180000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 360
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            PLL_R                          = 6
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
  HAL_StatusTypeDef ret = HAL_OK;

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is
     clocked below the maximum system frequency, to update the voltage scaling value
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 360;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  RCC_OscInitStruct.PLL.PLLR = 6;

  ret = HAL_RCC_OscConfig(&RCC_OscInitStruct);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Activate the OverDrive to reach the 180 MHz Frequency */
  ret = HAL_PWREx_EnableOverDrive();
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  ret = HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
  if(ret != HAL_OK)
  {
    while(1) { ; }
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
    /* Turn LED3 on */
    BSP_LED_On(LED3);
    while(1)
    {
    }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
