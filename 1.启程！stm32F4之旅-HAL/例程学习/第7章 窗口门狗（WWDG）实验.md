# 第七章 窗口门狗（WWDG）实验

## 1. 硬件设计

本实验用到的硬件资源有：

- 指示灯DS0和DS1

- 窗口看门狗

其中指示灯前面介绍过了，窗口看门狗属于 STM32F4 的内部资源，只需要软件设置好即可正常工作。我们通过 DS0 和 DS1 来指示 STM32F4 的复位情况和窗口看门狗的喂狗情况。

## 2. 软件设计

### 2.1 看门狗初始函数

```c
void WWDG_Init(u8 tr,u8 wr,u32 fprer)
{
    WWDG_Handler.Instance = WWDG;               // 选择WWDG外设
    WWDG_Handler.Init.Prescaler = fprer; 	    // 设置分频系数
    WWDG_Handler.Init.Window = wr;       	    // 设置窗口值
    WWDG_Handler.Init.Counter = tr;     	    // 设置计数器值
	WWDG_Handler.Init.EWIMode = WWDG_EWI_ENABLE;// 使能提前唤醒中断
    HAL_WWDG_Init(&WWDG_Handler);      			// 初始化WWDG
}
```

1. **`u8 tr`**: 计数器初值。WWDG计数器的起始值，值越大，看门狗重置的时间越长。
2. **`u8 wr`**: 窗口值。WWDG的窗口值，必须大于计数器值，用于监控计数器是否在规定范围内，以避免重置。
3. **`u32 fprer`**: 分频系数。决定WWDG计数器的时钟频率。通常是一个预分频值，用于调整计数器的计数频率。

---

**代码功能**:

- `WWDG_Handler.Instance = WWDG;` 选择使用WWDG外设。
- `WWDG_Handler.Init.Prescaler = fprer;` 设置分频系数。
- `WWDG_Handler.Init.Window = wr;` 设置窗口值。
- `WWDG_Handler.Init.Counter = tr;` 设置计数器初值。
- `WWDG_Handler.Init.EWIMode = WWDG_EWI_ENABLE;` 使能提前唤醒中断。
- `HAL_WWDG_Init(&WWDG_Handler);` 调用初始化函数配置WWDG。

### 2.2 看门狗时钟及中断配置

```c
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{   
    __HAL_RCC_WWDG_CLK_ENABLE();         // 使能窗口看门狗时钟
    // NVIC中断设置
    HAL_NVIC_SetPriority(WWDG_IRQn,2,3); // 抢占优先级2，子优先级为3
    HAL_NVIC_EnableIRQ(WWDG_IRQn);       // 使能窗口看门狗中断
}
```

1. **`__HAL_RCC_WWDG_CLK_ENABLE();`**: 使能WWDG外设的时钟。这是必须的步骤，以确保WWDG模块的正常工作。

2. **`HAL_NVIC_SetPriority(WWDG_IRQn,2,3);`**: 设置WWDG中断的优先级。`2`是抢占优先级，`3`是子优先级，这决定了中断处理的优先顺序。

3. **`HAL_NVIC_EnableIRQ(WWDG_IRQn);`**: 使能WWDG中断请求，确保中断能够被处理。

### 2.3 窗口看门狗中断服务函数

```c
// 窗口看门狗中断服务函数
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&WWDG_Handler); // 调用WWDG共用中断处理函数
}
```

```c
// 中断服务函数处理过程
// 此函数会被HAL_WWDG_IRQHandler()调用
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef* hwwdg)
{
    HAL_WWDG_Refresh(&WWDG_Handler); // 更新窗口看门狗值
    LED1 = !LED1; 
}
```

## 2.4 主函数

```c
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
```

该函数通过 LED0(DS0)来指示是否正在初始化。而 LED1(DS1)用来指示是否发生了中断。我们先让 LED0 亮 300ms，然后关闭以用于判断是否有复位发生了。在初始化 WWDG 之后，我们回到死循环，关闭 LED1，并等待看门狗中断的触发/复位。
