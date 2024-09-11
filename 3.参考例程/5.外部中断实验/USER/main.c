#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "exti.h"

int main(void)
{
    HAL_Init();                  // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); // 设置时钟,168Mhz
	delay_init(168);             // 初始化延时函数
	uart_init(115200);           // 初始化USART
	LED_Init();				     // 初始化LED	
    KEY_Init();                  // 初始化按键
	EXTI_Init();                 // 外部中断初始化
	
    while(1)
    {	
		printf("OK\r\n"); // 打印OK提示程序运行
        delay_ms(1000);   // 每隔1s打印一次 
    }
}