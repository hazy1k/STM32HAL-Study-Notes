#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"

int main(void)
{
	u8 key; // 按键扫描值
	
    HAL_Init();                   // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  // 设置时钟,168Mhz
	delay_init(168);              // 初始化延时函数
	LED_Init();					  // 初始化LED	
    KEY_Init();                   // 初始化按键
	
    while(1)
    {
        key = KEY_Scan(0); // 按键扫描
		switch(key)
		{				 
			case WKUP_PRES:	// 控制LED0,LED1互斥点亮
				LED1 = !LED1;
				LED0 = !LED1;
				break;
			case KEY2_PRES: // 控制LED0翻转
				LED0 = !LED0;
				break;
			case KEY1_PRES: // 控制LED1翻转	 
				LED1 = !LED1;
				break;
			case KEY0_PRES: // 同时控制LED0,LED1翻转 
				LED0 = !LED0;
				LED1 = !LED1;
				break;
		}
        delay_ms(10);
	}
}