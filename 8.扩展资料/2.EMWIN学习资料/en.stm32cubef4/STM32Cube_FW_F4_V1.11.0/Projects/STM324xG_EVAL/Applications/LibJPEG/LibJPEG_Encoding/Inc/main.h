/**
  ******************************************************************************
  * @file    LibJPEG/LibJPEG_Encoding/Inc/main.h
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   Header for main.c module
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* EVAL includes component */
#include "stm324xg_eval.h"
#include "stm324xg_eval_lcd.h"
#include "stm324xg_eval_sram.h"

/* FatFs includes component */
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* TBC */
#include <stdint.h>
#include <string.h>
#include "jpeglib.h"

#include "decode.h"
#include "encode.h"

/* Exported types ------------------------------------------------------------*/
typedef struct RGB
{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;

/* Exported constants --------------------------------------------------------*/
#define IMAGE_HEIGHT 240
#define IMAGE_WIDTH  320
#define IMAGE_QUALITY 90


#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
