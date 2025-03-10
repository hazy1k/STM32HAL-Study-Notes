/**
  ******************************************************************************
  * @file    audioplayer_app.h
  * @author  MCD Application Team
  * @version V1.4.3
  * @date    29-January-2016   
  * @brief   header of audio player application file
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
#ifndef __AUDIOPLAYER_APP_H
#define __AUDIOPLAYER_APP_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Exported constants --------------------------------------------------------*/
#define MUTE_OFF                      0x00
#define MUTE_ON                       0x01
   
#define PLAY_ACTIVE                   0x00
#define PAUSE_ACTIVE                  0x01
   
#define AUDIO_BUFFER_SIZE             (1024 * 8)
   
/* Exported types ------------------------------------------------------------*/
typedef enum
{
  AUDIOPLAYER_STOP = 0, 
  AUDIOPLAYER_START,   
  AUDIOPLAYER_PLAY,
  AUDIOPLAYER_PAUSE, 
  AUDIOPLAYER_EOF,   
  AUDIOPLAYER_ERROR,  
  
}AUDIOPLAYER_StateTypdef;

typedef enum
{
  BUFFER_OFFSET_NONE = 0,  
  BUFFER_OFFSET_HALF,  
  BUFFER_OFFSET_FULL,     
}
BUFFER_StateTypeDef;

typedef enum
{
  AUDIOPLAYER_ERROR_NONE = 0, 
  AUDIOPLAYER_ERROR_IO,
  AUDIOPLAYER_ERROR_HW, 
  AUDIOPLAYER_ERROR_MEM, 
  AUDIOPLAYER_ERROR_FORMAT_NOTSUPPORTED,   
  
}AUDIOPLAYER_ErrorTypdef;

typedef struct 
{
  uint8_t                   buffer[AUDIO_BUFFER_SIZE];  
  uint32_t                  volume;
  uint32_t                  mute;   
  AUDIOPLAYER_StateTypdef   state;

}AUDIOPLAYER_ProcessTypdef ;

typedef struct 
{
  uint32_t   ChunkID;       /* 0 */ 
  uint32_t   FileSize;      /* 4 */
  uint32_t   FileFormat;    /* 8 */
  uint32_t   SubChunk1ID;   /* 12 */
  uint32_t   SubChunk1Size; /* 16*/  
  uint16_t   AudioFormat;   /* 20 */ 
  uint16_t   NbrChannels;   /* 22 */   
  uint32_t   SampleRate;    /* 24 */
  
  uint32_t   ByteRate;      /* 28 */
  uint16_t   BlockAlign;    /* 32 */  
  uint16_t   BitPerSample;  /* 34 */  
  uint32_t   SubChunk2ID;   /* 36 */   
  uint32_t   SubChunk2Size; /* 40 */    

}WAV_InfoTypedef ;

/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Init(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Play(uint32_t frequency);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Stop(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Pause(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Resume(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Mute(uint8_t state);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_GetFileInfo(char* file, WAV_InfoTypedef* info);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SelectFile(char* file);
AUDIOPLAYER_StateTypdef  AUDIOPLAYER_GetState(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_Process(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_DeInit(void);
uint32_t                 AUDIOPLAYER_GetProgress (void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetVolume(uint32_t volume);
uint32_t                 AUDIOPLAYER_GetVolume(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_NotifyEndOfFile(void);
AUDIOPLAYER_ErrorTypdef  AUDIOPLAYER_SetPosition(uint32_t position);

#ifdef __cplusplus
}
#endif

#endif /*__AUDIOPLAYER_APP_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
