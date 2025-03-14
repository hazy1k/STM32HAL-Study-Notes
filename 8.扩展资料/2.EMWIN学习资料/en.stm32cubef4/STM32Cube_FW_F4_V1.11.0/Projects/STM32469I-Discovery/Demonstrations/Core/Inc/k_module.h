/**
  ******************************************************************************
  * @file    k_module.h
  * @author  MCD Application Team
  * @version V1.1.2
  * @date    29-January-2016
  * @brief   Header for k_module.c file
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
#ifndef __K_MODULE_H
#define __K_MODULE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"
   
/* Exported types ------------------------------------------------------------*/
typedef struct
{
 uint8_t     id;
 const char  *name;
 GUI_CONST_STORAGE GUI_BITMAP  ** open_icon;
 GUI_CONST_STORAGE GUI_BITMAP  ** close_icon;
 void        (*startup) (WM_HWIN , uint16_t, uint16_t );
 void        (*DirectOpen) (char * ); 
}
K_ModuleItem_Typedef;

typedef struct
{
  const K_ModuleItem_Typedef   *module;  
  uint8_t  in_use;  
  uint8_t  win_state;    
}
K_ModulePropertyTypedef;

typedef struct
{
  char   ext[4];
  const K_ModuleItem_Typedef   *module;
}
K_ModuleOpenTypedef;

typedef void K_GET_DIRECT_OPEN_FUNC(char *);

/* Structure for menu items */
typedef struct {
  char * sText;
  U16 Id;
  U16 Flags;
} MENU_ITEM;

extern K_ModulePropertyTypedef    module_prop[];
/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void    k_InitMenu(void);
void    k_StartUp(void);
void    k_SetGuiProfile(void);
void    k_ModuleInit(void);
void    k_UpdateLog(char *Msg);
uint8_t k_ModuleAdd(K_ModuleItem_Typedef *module);
uint8_t k_ModuleGetIndex(K_ModuleItem_Typedef *module);
void    k_ModuleRemove(K_ModuleItem_Typedef *module); 
uint8_t k_ModuleGetNumber(void);
uint8_t k_ModuleOpenLink(K_ModuleItem_Typedef *module, char *ext);
K_GET_DIRECT_OPEN_FUNC *k_ModuleCheckLink(char *ext);

#ifdef __cplusplus
}
#endif

#endif /*__K_MODULE_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
