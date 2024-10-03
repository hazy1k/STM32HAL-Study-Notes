# 第九章 定时器PWM输出实验

## 1. 硬件设计

本实验用到的硬件资源有：

- 指示灯DS0

- 定时器TIM14

这两个我们前面都已经介绍了，因为 TIM14_CH1 可以通过 PF9 输出 PWM，而 DS0 就是直接在 PF9 上面的，所以电路上并没有任何变化。

## 2. 软件设计

### 2.1 TIM14 PWM模式初始化

```c
// TIM14 PWM部分初始化 
// arr：自动重装值。
// psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
// Ft=定时器工作频率,单位:Mhz
void TIM14_PWM_Init(u16 arr, u16 psc)
{  
    TIM14_Handler.Instance = TIM14;                 // 定时器14
    TIM14_Handler.Init.Prescaler = psc;            // 定时器分频
    TIM14_Handler.Init.CounterMode = TIM_COUNTERMODE_UP;// 向上计数模式
    TIM14_Handler.Init.Period = arr;               // 自动重装载值
    TIM14_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM14_Handler); // 初始化PWM

    TIM14_CH1Handler.OCMode = TIM_OCMODE_PWM1; // 模式选择PWM1
    TIM14_CH1Handler.Pulse = arr/2;            // 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半,即占空比为50%
    TIM14_CH1Handler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM14_Handler,&TIM14_CH1Handler,TIM_CHANNEL_1); // 配置TIM14通道1
    HAL_TIM_PWM_Start(&TIM14_Handler,TIM_CHANNEL_1); // 开启PWM通道1
}
```

#### 函数定义

```c
void TIM14_PWM_Init(u16 arr, u16 psc)
```

- 这是一个初始化 TIM14 的 PWM 输出的函数。它接收两个参数：
  - `arr`：自动重装载值（Auto Reload Register），决定 PWM 的频率。
  - `psc`：预分频器值（Prescaler），用于调整定时器的计数频率。

#### 定时器配置

```c
TIM14_Handler.Instance = TIM14; // 定时器14
```

- 设置 `TIM14_Handler` 的实例为 `TIM14`，这意味着我们将配置 TIM14 定时器。

```c
TIM14_Handler.Init.Prescaler = psc; // 定时器分频
```

- 将传入的预分频器值 `psc` 赋值给 `TIM14_Handler` 的预分频器配置，决定了定时器的输入频率。

```c
TIM14_Handler.Init.CounterMode = TIM_COUNTERMODE_UP; // 向上计数模式
```

- 设置计数模式为向上计数，表示定时器从 0 开始计数到达设定的周期。

```c
TIM14_Handler.Init.Period = arr; // 自动重装载值
```

- 将自动重装载值 `arr` 设置为定时器的周期。当计数器达到这个值时，会重置为 0，循环计数。

```c
TIM14_Handler.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
```

- 设置时钟分频因子为 1，表示不进行额外的分频。

```c
HAL_TIM_PWM_Init(&TIM14_Handler); // 初始化PWM
```

- 调用 HAL 库的 `HAL_TIM_PWM_Init` 函数，完成 TIM14 的 PWM 初始化。

#### PWM 通道配置

```c
TIM14_CH1Handler.OCMode = TIM_OCMODE_PWM1; // 模式选择PWM1
```

- 设置通道 1 的输出比较模式为 PWM1，这是一种常见的 PWM 输出模式。

```c
TIM14_CH1Handler.Pulse = arr / 2; // 设置比较值，此值用来确定占空比
```

- 设置 PWM 的比较值为 `arr/2`，这样可以使 PWM 的占空比为 50%。占空比是指 PWM 信号高电平时间与其周期的比值。

```c
TIM14_CH1Handler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低 
```

- 设置输出比较的极性为低，即在比较值小于计数器值时，输出低电平。

```c
HAL_TIM_PWM_ConfigChannel(&TIM14_Handler, &TIM14_CH1Handler, TIM_CHANNEL_1); // 配置TIM14通道1
```

- 调用 HAL 库的 `HAL_TIM_PWM_ConfigChannel` 函数，配置 TIM14 的通道 1。

#### 启动 PWM

```c
HAL_TIM_PWM_Start(&TIM14_Handler, TIM_CHANNEL_1); // 开启PWM通道1
```

- 调用 `HAL_TIM_PWM_Start` 函数启动 TIM14 的通道 1，使 PWM 输出开始。

### 2.2 TIM14 GPIO初始化

```c
// 定时器底层驱动，时钟使能，引脚配置
// 此函数会被HAL_TIM_PWM_Init()调用
// htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_TIM14_CLK_ENABLE();            // 使能定时器14
    __HAL_RCC_GPIOF_CLK_ENABLE();            // 开启GPIOF时钟

    GPIO_Initure.Pin = GPIO_PIN_9;          // PF9
    GPIO_Initure.Mode = GPIO_MODE_AF_PP;    // 复用推挽输出
    GPIO_Initure.Pull = GPIO_PULLUP;        // 上拉
    GPIO_Initure.Speed = GPIO_SPEED_HIGH;   // 高速
    GPIO_Initure.Alternate = GPIO_AF9_TIM14;// PF9复用为TIM14_CH1
    HAL_GPIO_Init(GPIOF,&GPIO_Initure); // 初始化PF9
}
```

### 2.3 设置占空比函数

```c
// 设置TIM通道4的占空比
// compare:比较值
void TIM_SetTIM14Compare1(u32 compare)
{
    TIM14->CCR1 = compare; // 设置占空比 
}
```

### 2.4 主函数

```c
int main(void)
{
    u8 dir = 1; // 1:递增 0:递减
    u16 led0pwmval = 0; // LED0 PWM输出占空比值

    HAL_Init();                  // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7); // 设置时钟,168Mhz
    delay_init(168);             // 初始化延时函数
    uart_init(115200);           // 初始化USART
    LED_Init();                  // 初始化LED    
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
            dir=0;          // led0pwmval到达300后,方向为递减
        if(led0pwmval == 0)
            dir=1;          // led0pwmval递减到0后，方向改为递增
        TIM_SetTIM14Compare1(led0pwmval); // 修改比较值，修改占空比
    }
}
```

第一次接触PWM可能对这个主函数不熟悉，下面我们来解释一下：

```c
u8 dir = 1; // 1:递增 0:递减
u16 led0pwmval = 0; // LED0 PWM输出占空比值
```

- `dir`：一个8位无符号整数，用于控制LED的亮度变化方向。1表示亮度逐渐增加，0表示亮度逐渐减少。
- `led0pwmval`：一个16位无符号整数，表示PWM（脉宽调制）信号的占空比，也就是LED的亮度值，初始为0。

```c
while(1)
{
    delay_ms(10);         
    if(dir)
        led0pwmval++; // dir==1 led0pwmval递增
    else 
        led0pwmval--; // dir==0 led0pwmval递减 
    if(led0pwmval > 300)
        dir=0;          // led0pwmval到达300后,方向改为递减
    if(led0pwmval == 0)
        dir=1;          // led0pwmval递减到0后，方向改为递增
    TIM_SetTIM14Compare1(led0pwmval); // 修改比较值，修改占空比
}
```

- **`while(1)`**：进入无限循环，不断执行内部的代码。
- **`delay_ms(10)`**：每次循环延时10毫秒，控制LED亮度更新的频率。
- **`if(dir)`**：检查`dir`的值。如果为1，则表示当前是递增状态，`led0pwmval`增加1；如果为0，则表示当前是递减状态，`led0pwmval`减少1。
- **`if(led0pwmval > 300)`**：当`led0pwmval`大于300时，切换方向为递减（将`dir`设为0）。
- **`if(led0pwmval == 0)`**：当`led0pwmval`减小到0时，切换方向为递增（将`dir`设为1）。
- **`TIM_SetTIM14Compare1(led0pwmval)`**：根据当前的`led0pwmval`值更新PWM信号的占空比，从而调整LED的亮度。

## 3. 小结

实际上我们就是用定时器输出PWM来做了一个呼吸灯嘛，没什么难度，简单回顾一下吧：

```c
#include "stm32f4xx_hal.h" // 根据你的STM32型号选择合适的头文件

TIM_HandleTypeDef htim3; // 定时器句柄

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM3_Init(void);

int main(void)
{
    HAL_Init(); // 初始化HAL库
    SystemClock_Config(); // 配置系统时钟
    MX_GPIO_Init(); // 初始化GPIO
    MX_TIM3_Init(); // 初始化定时器

    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // 启动PWM输出

    uint16_t pwmValue = 0; // PWM占空比
    uint8_t direction = 1; // 方向：1递增，0递减

    while (1)
    {
        // 调整PWM占空比
        __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwmValue); 
        // 更新占空比
        if (direction)
        {
            pwmValue += 5; // 增加亮度
            if (pwmValue >= 1000) // 假设最大值为1000
            {
                direction = 0; // 转向减小
            }
        }
        else
        {
            pwmValue -= 5; // 减少亮度
            if (pwmValue == 0)
            {
                direction = 1; // 转向增加
            }
        }
        HAL_Delay(50); // 延时以控制呼吸速度
    }
}

static void MX_TIM3_Init(void)
{
    TIM_OC_InitTypeDef sConfigOC = {0};

    htim3.Instance = TIM3;
    htim3.Init.Prescaler = 8399; // 预分频，从84MHz变为10kHz
    htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
    htim3.Init.Period = 1000; // 最大计数值
    htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&htim3);

    sConfigOC.OCMode = TIM_OCMODE_PWM1;
    sConfigOC.Pulse = 0; // 初始占空比为0
    sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
    HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1);
}

static void MX_GPIO_Init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE(); // 根据实际连接的引脚使能时钟
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_0; // 假设PWM输出连接在PB0
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP; // 复用推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
    // 这里配置系统时钟，省略具体细节
}
```

1. **定时器初始化**：
   
   - `MX_TIM3_Init()`：配置TIM3为PWM模式，设置预分频和计数周期。

2. **PWM输出**：
   
   - `HAL_TIM_PWM_Start()`：启动PWM输出。
   - `__HAL_TIM_SET_COMPARE()`：根据当前的`pwmValue`设置PWM的占空比。

3. **循环控制**：
   
   - 使用`direction`变量控制`pwmValue`的递增和递减，以实现呼吸灯的效果。

---

2024.10.3 第一次修订，后期不再维护
