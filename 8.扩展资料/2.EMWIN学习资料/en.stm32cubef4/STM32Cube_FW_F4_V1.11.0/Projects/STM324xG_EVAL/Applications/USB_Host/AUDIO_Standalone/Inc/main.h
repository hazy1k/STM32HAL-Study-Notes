/**
  ******************************************************************************
  * @file    USB_Host/AUDIO_Standalone/Inc/main.h 
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
#include "stdio.h"
#include "usbh_core.h"
#include "stm324xg_eval_io.h"
#include "lcd_log.h"
#include "usbh_audio.h" 
#include "stm324xg_eval_sd.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/* Exported constants --------------------------------------------------------*/
#define FILEMGR_LIST_DEPDTH                     24
#define FILEMGR_FILE_NAME_SIZE                  40
#define FILEMGR_FULL_PATH_SIZE                 256
#define FILEMGR_MAX_LEVEL                        4    
#define FILETYPE_DIR                             0
#define FILETYPE_FILE                            1

/* Exported types ------------------------------------------------------------*/
typedef struct AUDIO_Info_t {
  uint32_t ChunkID;       /* 0  */ 
  uint32_t FileSize;      /* 4  */
  uint32_t FileFormat;    /* 8  */
  uint32_t SubChunk1ID;   /* 12 */
  uint32_t SubChunk1Size; /* 16 */  
  uint16_t AudioFormat;   /* 20 */ 
  uint16_t NbrChannels;   /* 22 */   
  uint32_t SampleRate;    /* 24 */
  uint32_t ByteRate;      /* 28 */
  uint16_t BlockAlign;    /* 32 */  
  uint16_t BitPerSample;  /* 34 */  
  uint32_t SubChunk2ID;   /* 36 */   
  uint32_t SubChunk2Size; /* 40 */    
}WAV_InfoTypedef;

typedef struct _FILELIST_LineTypeDef {
  uint8_t type;
  uint8_t name[FILEMGR_FILE_NAME_SIZE];
}FILELIST_LineTypeDef;

typedef struct _FILELIST_FileTypeDef {
  FILELIST_LineTypeDef  file[FILEMGR_LIST_DEPDTH] ;
  uint16_t              ptr; 
}FILELIST_FileTypeDef;

typedef enum {
  AUDIO_DEMO_IDLE = 0,
  AUDIO_DEMO_WAIT,  
  AUDIO_DEMO_EXPLORE,
  AUDIO_DEMO_PLAYBACK,
  AUDIO_REENUMERATE  
}AUDIO_Demo_State;

typedef struct _DemoStateMachine {
  __IO AUDIO_Demo_State state;
  __IO uint8_t          select;
  __IO uint8_t          lock;
}AUDIO_DEMO_StateMachine;

typedef enum {
  AUDIO_SELECT_MENU = 0,
  AUDIO_PLAYBACK_CONTROL ,  
}AUDIO_DEMO_SelectMode;

typedef enum {
  APPLICATION_IDLE = 0,  
  APPLICATION_START,   
  APPLICATION_READY,
  APPLICATION_DISCONNECT,
}AUDIO_ApplicationTypeDef;

typedef enum {
  AUDIO_STATE_IDLE = 0,
  AUDIO_STATE_WAIT,    
  AUDIO_STATE_INIT,    
  AUDIO_STATE_CONFIG,    
  AUDIO_STATE_PLAY,  
  AUDIO_STATE_NEXT,  
  AUDIO_STATE_PREVIOUS,
  AUDIO_STATE_FORWARD,   
  AUDIO_STATE_BACKWARD, 
  AUDIO_STATE_PAUSE,
  AUDIO_STATE_RESUME,
  AUDIO_STATE_VOLUME_UP,
  AUDIO_STATE_VOLUME_DOWN,
  AUDIO_STATE_ERROR, 
}AUDIO_PLAYBACK_StateTypeDef;

typedef enum {
  AUDIO_ERROR_NONE = 0,  
  AUDIO_ERROR_IO,   
  AUDIO_ERROR_EOF,
}AUDIO_ErrorTypeDef;

extern AUDIO_ApplicationTypeDef Appli_state;
extern USBH_HandleTypeDef hUSBHost;
extern AUDIO_DEMO_StateMachine audio_demo;
extern FILELIST_FileTypeDef FileList;
extern AUDIO_PLAYBACK_StateTypeDef audio_state;

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void AUDIO_MenuInit(void);
void AUDIO_MenuSelectItem(uint8_t **menu, uint8_t item);
void AUDIO_MenuProcess(void);
void AUDIO_MenuProbeKey(JOYState_TypeDef state);
void AUDIO_PlaybackProbeKey(JOYState_TypeDef state);
uint8_t SD_StorageInit(void);
FRESULT SD_StorageParse(void);
void Toggle_Leds(void);
AUDIO_ErrorTypeDef AUDIO_Start(uint8_t idx);
AUDIO_ErrorTypeDef AUDIO_Process(void);
AUDIO_ErrorTypeDef AUDIO_Stop(void);
#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
