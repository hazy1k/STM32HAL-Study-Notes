# 第二章 GPIO介绍及应用

## 1. STM32F407 GPIO 简介

PIO 是控制或者采集外部器件的信息的外设， 即负责输入输出。它按组分配存在，每组最多 16 个 IO 口，组数视芯片而定。 比如： STM32F407ZGT6 芯片是 144 脚的芯片， 分为 7 组，分别是： GPIOA、 GPIOB、 GPIOC、 GPIOD、 GPIOE、 GPIOF 和 GPIOG， 其中共有 112 个 IO口可供我们编程使用。这里重点说一下 STM32F407 的 IO 电平兼容性问题， STM32F407 的绝大部分 IO 口，都兼容 5V，至于到底哪些是兼容 5V 的，请看 STM32F407ZG 的数据手册（注意是数据手册，不是中文参考手册），见表 5 大容量 STM32F40xxx 引脚定义，凡是有 FT 标志的，都是兼容 5V 电平的 IO 口，可以直接接 5V 的外设（注意：如果引脚设置的是模拟输入模式，则不能接 5V！ )，凡是不带 FT 标志的，就建议大家不要接 5V 了，可能烧坏 MCU。

![屏幕截图 2025-05-28 164103.png](https://raw.githubusercontent.com/hazy1k/My-drawing-bed/main/2025/05/28-16-42-06-屏幕截图%202025-05-28%20164103.png)

## 2. GPIO使用示例

### 2.1 GPIO输出-LED闪烁

![屏幕截图 2025-05-28 165659.png](https://raw.githubusercontent.com/hazy1k/My-drawing-bed/main/2025/05/28-16-57-06-屏幕截图%202025-05-28%20165659.png)

#### 2.1.1 LED相关参数宏定义

```c
#ifndef __LED_H__
#define __LED_H__

#include "sys.h"

#define LED_GPIO_Port GPIOF
#define LED_GPIO_CLK_ENABLE() __HAL_RCC_GPIOF_CLK_ENABLE()
#define LED0_GPIO_Pin GPIO_PIN_9
#define LED1_GPIO_Pin GPIO_PIN_10

// 低电平点亮
#define LED_ON(x)  HAL_GPIO_WritePin(LED_GPIO_Port, x, GPIO_PIN_RESET)
#define LED_OFF(x) HAL_GPIO_WritePin(LED_GPIO_Port, x, GPIO_PIN_SET)

void bsp_led_init(void);

#endif /* __LED_H__ */

```

#### 2.1.2 LED GPIO初始化

```c
#include "led.h"

void bsp_led_init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    LED_GPIO_CLK_ENABLE(); // 使能GPIO时钟
    GPIO_InitStructure.Pin = LED0_GPIO_Pin;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; // 推挽输出 
    GPIO_InitStructure.Pull = GPIO_PULLUP; // 上拉
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH; // 高速
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);
    GPIO_InitStructure.Pin = LED1_GPIO_Pin;
    HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStructure);
    LED_OFF(LED0_GPIO_Pin|LED1_GPIO_Pin); // 初始关闭LED
}

```

#### 2.1.3 主函数测试

```c
#include "bsp_init.h"

int main(void)
{
    bsp_init();
    while(1)
    {
        LED_ON(LED0_GPIO_Pin);
        delay_ms(500);
        LED_OFF(LED0_GPIO_Pin);
        delay_ms(500);
        LED_ON(LED1_GPIO_Pin);
        delay_ms(500);
        LED_OFF(LED1_GPIO_Pin);
        delay_ms(500);
    }
}

```


