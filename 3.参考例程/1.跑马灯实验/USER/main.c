#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"

int main(void)
{ 
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	LED_Init();						//初始化LED	
	
	while(1)
	{
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9, GPIO_PIN_RESET); // LED0对应引脚PF9拉低，亮，等同于LED0(0)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10, GPIO_PIN_SET);  // LED1对应引脚PF10拉高，灭，等同于LED1(1)
        delay_ms(500);									     // 延时500ms
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_9,GPIO_PIN_SET);    // LED0对应引脚PF9拉高，灭，等同于LED0(1)
        HAL_GPIO_WritePin(GPIOF,GPIO_PIN_10,GPIO_PIN_RESET); // LED1对应引脚PF10拉低，亮，等同于LED1(0)
        delay_ms(500);                                       // 延时500ms 
	}
}

/*下面主函数使用位带操作实现：*/

/*int main(void)
{ 
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	LED_Init();						//初始化LED	
	while(1)
	{
         LED0=0;			     	//LED0亮
	     LED1=1;				 	//LED1灭
		 delay_ms(500);
		 LED0=1;					//LED0灭
		 LED1=0;					//LED1亮
		 delay_ms(500);
	 }
}*/




/*
下面主函数使用直接操作结存器方式实现跑马灯
*/

/*int main(void)
{ 
    HAL_Init();                    	//初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);               	//初始化延时函数
	LED_Init();						//初始化LED	

	while(1)
	{
      GPIOF->BSRR=GPIO_PIN_9;     	//LED0亮
	  GPIOF->BSRR=GPIO_PIN_10<<16; 	//LED1灭
	  delay_ms(500);
      GPIOF->BSRR=GPIO_PIN_9<<16; 	//LED0灭
	  GPIOF->BSRR=GPIO_PIN_10;     	//LED1亮
	  delay_ms(500);
	 }
}*/	
