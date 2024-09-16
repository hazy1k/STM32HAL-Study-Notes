#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"

int main(void)
{
    HAL_Init();                  // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); // 设置时钟,168Mhz
	delay_init(168);             // 初始化延时函数
	uart_init(115200);           // 初始化USART
	LED_Init();					 // 初始化LED	
    KEY_Init();                  // 初始化按键
    TIM3_Init(5000-1,8400-1);    // 定时器3初始化，定时器时钟为84M，分频系数为8400-1，
                                 // 所以定时器3的频率为84M/8400=10K，自动重装载为5000-1，那么定时器周期就是500ms
    while(1)
    {
        LED0 = !LED0;  // LED0翻转
        delay_ms(200); // 延时200ms
    }
}