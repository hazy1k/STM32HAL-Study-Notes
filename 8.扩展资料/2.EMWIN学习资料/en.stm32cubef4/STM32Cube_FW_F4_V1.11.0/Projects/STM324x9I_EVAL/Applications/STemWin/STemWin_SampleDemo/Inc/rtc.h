/**
  ******************************************************************************
  * @file    RTC.h
  * @author  MCD Application Team
  * @version V1.4.3
  * @date    29-January-2016
  * @brief   Header for RTC.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT YYYY STMicroelectronics</center></h2>
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
#ifndef __RTC_H
#define __RTC_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dialog.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
   
extern  RTC_HandleTypeDef RtcHandle;
/* Exported functions ------------------------------------------------------- */ 
void     RTC_Init(void);
void     BACKUP_SaveParameter(uint32_t address, uint32_t data);
uint32_t BACKUP_RestoreParameter(uint32_t address);
#ifdef __cplusplus
}
#endif

#endif /*__RTC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
