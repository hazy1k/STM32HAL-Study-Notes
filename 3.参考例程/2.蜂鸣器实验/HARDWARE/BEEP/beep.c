#include "beep.h"

// 初始化PF8为输出.并使能时钟	    
// LED IO初始化
void BEEP_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOF_CLK_ENABLE();           //开启GPIOF时钟
	
    GPIO_Initure.Pin = GPIO_PIN_8; 			 // PF8
    GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;         // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;    // 高速
    HAL_GPIO_Init(GPIOF, &GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOF, GPIO_PIN_8, GPIO_PIN_RESET);	//蜂鸣器对应引脚GPIOF8拉低，
}
