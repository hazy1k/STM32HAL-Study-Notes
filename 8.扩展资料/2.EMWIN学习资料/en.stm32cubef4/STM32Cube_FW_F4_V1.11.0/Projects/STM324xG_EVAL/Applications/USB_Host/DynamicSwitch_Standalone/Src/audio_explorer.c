/**
  ******************************************************************************
  * @file    USB_Host/DynamicSwitch_Standalone/Src/audio_explorer.c 
  * @author  MCD Application Team
  * @version V1.3.3
  * @date    29-January-2016
  * @brief   This file provides uSD Card drive configuration
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
FATFS SD_FatFs;
char SD_Path[4]; 
FILELIST_FileTypeDef FileList;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the SD Storage.
  * @param  None
  * @retval Status
  */
uint8_t SD_StorageInit(void)
{
  /*Initializes the SD card device*/
  BSP_SD_Init();
  
  /* Check if the SD card is plugged in the slot */
  if(BSP_SD_IsDetected() == SD_PRESENT )
  {
    /* Link the SD Card disk I/O driver */
    if(FATFS_LinkDriver(&SD_Driver, SD_Path) == 0)
    {
      if((f_mount(&SD_FatFs, (TCHAR const*)SD_Path, 0) != FR_OK))
      {
        /* FatFs Initialization Error */
        LCD_ErrLog("Cannot Initialize FatFs! \n");
        return 1;
      }
      else
      {
        LCD_DbgLog ("INFO : FatFs Initialized! \n");
      }
    }  
  }
  else
  {
    LCD_ErrLog("SD card NOT plugged \n");
    return 1;
  }
return 0;
}

/**
  * @brief  Copies disk content in the explorer list.
  * @param  None
  * @retval Operation result
  */
FRESULT SD_StorageParse(void)
{
  FRESULT res;
  FILINFO fno;
  DIR dir;
  char *fn;
  
#if _USE_LFN
  static char lfn[_MAX_LFN];
  fno.lfname = lfn;
  fno.lfsize = sizeof(lfn);
#endif
  
  res = f_opendir(&dir, SD_Path);
  FileList.ptr = 0;
  
  if(res == FR_OK)
  {
    while (1)
    {
      res = f_readdir(&dir, &fno);
      
      if(res != FR_OK || fno.fname[0] == 0)
      {
        break;
      }
      if(fno.fname[0] == '.')
      {
        continue;
      }
#if _USE_LFN
      fn = *fno.lfname ? fno.lfname : fno.fname;
#else
      fn = fno.fname;
#endif
      
      if(FileList.ptr < FILEMGR_LIST_DEPDTH)
      {
        if((fno.fattrib & AM_DIR) == 0)
        {
          if((strstr(fn, "wav")) || (strstr(fn, "WAV")))
          {
            strncpy((char *)FileList.file[FileList.ptr].name, (char *)fn, FILEMGR_FILE_NAME_SIZE);
            FileList.file[FileList.ptr].type = FILETYPE_FILE;
            FileList.ptr++;
          }
        }
      }   
    }
  }
  f_closedir(&dir);
  return res;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
