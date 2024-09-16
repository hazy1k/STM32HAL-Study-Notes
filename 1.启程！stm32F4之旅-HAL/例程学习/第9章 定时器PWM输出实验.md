# 第九章 定时器PWM输出实验

## 1. 硬件设计

本实验用到的硬件资源有：

- 指示灯DS0

- 定时器TIM14

这两个我们前面都已经介绍了，因为 TIM14_CH1 可以通过 PF9 输出 PWM，而 DS0 就是直接节在 PF9 上面的，所以电路上并没有任何变化。

## 2. 软件设计

- TIM14 PWM模式初始化

```c
// TIM14 PWM部分初始化 
// arr：自动重装值。
// psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM14_PWM_Init(u16 arr, u16 psc)
{  
    TIM14_Handler.Instance = TIM14;          	   // 定时器14
    TIM14_Handler.Init.Prescaler = psc;            // 定时器分频
    TIM14_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;// 向上计数模式
    TIM14_Handler.Init.Period = arr;               // 自动重装载值
    TIM14_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM14_Handler); // 初始化PWM
    
    TIM14_CH1Handler.OCMode=TIM_OCMODE_PWM1; // 模式选择PWM1
    TIM14_CH1Handler.Pulse = arr/2;            // 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM14_CH1Handler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM14_Handler,&TIM14_CH1Handler,TIM_CHANNEL_1);//配置TIM14通道1
	
    HAL_TIM_PWM_Start(&TIM14_Handler,TIM_CHANNEL_1); // 开启PWM通道1
}
```

- TIM14 GPIO初始化

```c
// 定时器底层驱动，时钟使能，引脚配置
// 此函数会被HAL_TIM_PWM_Init()调用
// htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM14_CLK_ENABLE();			// 使能定时器14
	__HAL_RCC_GPIOF_CLK_ENABLE();			// 开启GPIOF时钟
	
	GPIO_Initure.Pin = GPIO_PIN_9;          // PF9
	GPIO_Initure.Mode = GPIO_MODE_AF_PP;  	// 复用推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;        // 上拉
	GPIO_Initure.Speed = GPIO_SPEED_HIGH;   // 高速
	GPIO_Initure.Alternate = GPIO_AF9_TIM14;// PF9复用为TIM14_CH1
	HAL_GPIO_Init(GPIOF,&GPIO_Initure); // 初始化PF9
}
```

- 设置占空比函数

```c
// 设置TIM通道4的占空比
// compare:比较值
void TIM_SetTIM14Compare1(u32 compare)
{
	TIM14->CCR1 = compare; // 设置占空比 
}
```

- 主函数

```c
int main(void)
{
	u8 dir = 1; // 1:递增 0:递减
    u16 led0pwmval = 0; // LED0 PWM输出占空比值
	
    HAL_Init();                  // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); // 设置时钟,168Mhz
	delay_init(168);             // 初始化延时函数
	uart_init(115200);           // 初始化USART
	LED_Init();				     // 初始化LED	
	TIM3_Init(5000-1,8400-1);    // 定时器3初始化，周期为500ms
	TIM14_PWM_Init(500-1,84-1);  // 84M/84=1M的计数频率，自动重装载为500，那么PWM频率为1M/500=2kHZ
	
    while(1)
    {
		delay_ms(10);	 	
		if(dir)
			led0pwmval++; // dir==1 led0pwmval递增
		else 
			led0pwmval--; // dir==0 led0pwmval递减 
		if(led0pwmval > 300)
			dir=0;		  // led0pwmval到达300后,方向为递减
		if(led0pwmval == 0)
			dir=1;		  // led0pwmval递减到0后，方向改为递增

		TIM_SetTIM14Compare1(led0pwmval); // 修改比较值，修改占空比
    }
}
```


