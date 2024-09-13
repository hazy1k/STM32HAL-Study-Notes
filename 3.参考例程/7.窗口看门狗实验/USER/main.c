#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "wwdg.h"

int main(void)
{
  HAL_Init();                   // 初始化HAL库    
  Stm32_Clock_Init(336,8,2,7);  // 设置时钟,168Mhz
	delay_init(168);              // 初始化延时函数
	uart_init(115200);            // 初始化USART
	LED_Init();						        // 初始化LED	
  KEY_Init();                   // 初始化按键
  LED0 = 0;                     // 点亮LED0
  delay_ms(300);                // 延时300ms再初始化看门狗,LED0的变化"可见"
  WWDG_Init(0X7F,0X5F,WWDG_PRESCALER_8); // 计数器值为7F，窗口寄存器为5F，分频数为8
  while(1)
  {	
	  LED0=1; // 熄灭LED灯 
  }
}