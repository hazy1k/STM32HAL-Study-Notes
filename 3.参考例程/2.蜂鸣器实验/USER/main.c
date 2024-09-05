#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "beep.h"

int main(void)
{ 
    HAL_Init();                  // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); // 设置时钟,168Mhz
	delay_init(168);             // 初始化延时函数
	LED_Init();				     // 初始化LED	
	BEEP_Init();			     // 初始化蜂鸣器
	
	while(1)
	{
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_9, GPIO_PIN_RESET); // DS0拉低，亮   等同LED0=0;
		HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET); // BEEP引脚拉低，等同BEEP=0;
		delay_ms(300);										  // 延时300ms
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);	  // DS0拉高，灭   等同LED0=1;
		HAL_GPIO_WritePin(GPIOF,GPIO_PIN_8,GPIO_PIN_SET);	  // BEEP引脚拉高，等同BEEP=1;
		delay_ms(300);										//延时300ms
	 }
}