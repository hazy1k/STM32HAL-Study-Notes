#ifndef __PWMDAC_H
#define __PWMDAC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F407开发板
//PWM DAC驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2017/4/13
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

extern TIM_HandleTypeDef TIM9_Handler;         //定时器9 PWM句柄 

void TIM9_CH2_PWM_Init(u16 arr,u16 psc);	
void TIM_SetTIM9Compare2(u32 compare);
#endif
