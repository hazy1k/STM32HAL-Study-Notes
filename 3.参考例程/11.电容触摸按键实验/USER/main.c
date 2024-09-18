#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "tpad.h"

int main(void)
{
	u8 t=0; 
    HAL_Init();                   	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	uart_init(115200);             	//初始化USART
	LED_Init();						//初始化LED	
	TPAD_Init(8);                   //初始化触摸按键
	
   	while(1)
	{					  						  		 
 		if(TPAD_Scan(0)) // 成功捕获到了一次上升沿(此函数执行时间至少15ms)
		{
			LED1=!LED1;	// LED1取反
		}
		t++;
		if(t == 15)		 
		{
			t = 0;
			LED0 = !LED0; // LED0取反,提示程序正在运行
		}
		delay_ms(10);
	}
}