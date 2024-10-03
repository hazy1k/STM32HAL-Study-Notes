# 第十一章 定时器输出PWM

## 1. PWM简介

脉冲宽度调制(PWM)，是英文“Pulse Width Modulation” 的缩写，简称脉宽调制，是利用微处理器的数字输出来对模拟电路进行控制的一种非常有效的技术。简单一点，就是对脉冲宽度的控制。

STM32F4 的定时器除了 TIM6 和 7。其他的定时器都可以用来产生 PWM 输出。其中高级定时器 TIM1 和 TIM8 可以同时产生多达 7 路的 PWM 输出。而通用定时器也能同时产生多达 4 路的 PWM 输出！这里我们仅使用 TIM14 的 CH1 产生一路 PWM 输出。

要使 STM32F4 的通用定时器 TIMx 产生 PWM 输出，除了上一章介绍的寄存器外，我们还会用到 3 个寄存器，来控制 PWM 的。这三个寄存器分别是：捕获/比较模式寄存器（TIMx_CCMR1/2）、捕获/比较使能寄存器（TIMx_CCER）、捕获/比较寄存器（TIMx_CCR1~4）。接下来我们简单介绍一下这三个寄存器。

## 2. 相关寄存器介绍

### 2.1 捕获/比较模式寄存器（TIMx_CCMR1/2）

首先是捕获/比较模式寄存器（TIMx_CCMR1/2），该寄存器一般有 2 个： TIMx _CCMR1和 TIMx _CCMR2，不过 TIM14 只有一个。 TIMx_CCMR1 控制 CH1 和 2，而 TIMx_CCMR2控制 CH3 和 4。以下我们将以 TIM14 为例进行介绍。TIM14_CCMR1 寄存器各位描述如图：

![屏幕截图 2024 09 16 131201](https://img.picgo.net/2024/09/16/-2024-09-16-131201e3622c7f28167b12.png)

该寄存器的有些位在不同模式下，功能不一样，所以在图中，我们把寄存器分了 2层，上面一层对应输出而下面的则对应输入。

这里我们需要说明的是模式设置位 OC1M，此部分由 3位组成。总共可以配置成 7 种模式，我们使用的是 PWM 模式，所以这 3 位必须设置为 110/111。这两种 PWM 模式的区别就是输出电平的极性相反。 另外 CC1S 用于设置通道的方向（输入/输出）默认设置为 0，就是设置通道作为输出使用。 注意：这里是因为我们的 TIM14 只有 1 个通道，所以才只有第八位有效，高八位无效，其他有多个通道的定时器，高八位也是有效的。

---

- **输出比较**：生成特定的PWM（脉宽调制）信号或在特定时间触发事件。
- **输入捕获**：记录输入信号的状态变化（如上升沿和下降沿），以测量信号的频率或周期。

#### CCMR1 和 CCMR2 的作用

- **CCMR1** 通常用于配置定时器的前两个通道（例如通道1和通道2）。
- **CCMR2** 用于配置下两个通道（如通道3和通道4）。

这两个寄存器允许你设置每个通道的操作模式。

#### 输出比较模式

输出比较模式主要用于生成PWM信号或其他定时器事件。CCMR寄存器中的相关字段如下：

- **OC1M (Output Compare Mode)**：
  
  - 选择输出比较的模式。
  - 常见的模式包括：
    - 000：禁用（普通模式）
    - 001：输出比较模式1
    - 010：输出比较模式2
    - 011：PWM模式1
    - 100：PWM模式2

- **OC1PE (Output Compare Preload Enable)**：
  
  - 使能预装载寄存器。这意味着在下一次更新事件时，将新的输出比较值加载到比较寄存器中。

#### 输入捕获模式

输入捕获模式允许定时器捕获输入信号的变化。相关字段如下：

- **CC1S (Capture/Compare Selection)**：
  
  - 选择该通道是用于输入捕获还是输出比较。
  - 00：输出比较
  - 01：输入捕获

- **IC1F (Input Capture Filter)**：
  
  - 设置过滤器参数，帮助消除噪声对输入信号的影响。

下面是一个更详细的代码示例，展示如何配置定时器以使用输出比较和输入捕获功能。

```c
#include "stm32f4xx.h"  // 请根据你的MCU型号包含相应的头文件

void TIM2_Config(void) {
    // 1. 使能定时器时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

    // 2. 配置基本参数
    TIM2->PSC = 7999;  // 设置预分频器为7999，定时器时钟频率 = 1 MHz
    TIM2->ARR = 1000;  // 设置自动重载值，定时器溢出时间 = 1秒

    // 3. 配置通道1为PWM输出模式
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;          // 清除CC1S位，选择输出比较
    TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // 设置为PWM模式1
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;          // 使能预装载

    // 设置通道1的比较值
    TIM2->CCR1 = 500;  // 设置占空比为50%

    // 4. 配置通道2为输入捕获模式
    TIM2->CCMR1 &= ~(3 << TIM_CCMR1_CC2S_Pos); // 清除CC2S位，选择输入捕获
    TIM2->CCMR1 |= (1 << TIM_CCMR1_IC2F_Pos);  // 设置输入捕获滤波器

    // 5. 启动定时器
    TIM2->CR1 |= TIM_CR1_CEN;  // 启动定时器

    // 6. 使能通道1和通道2的输出/输入
    TIM2->CCER |= TIM_CCER_CC1E; // 使通道1输出使能
    TIM2->CCER |= TIM_CCER_CC2E; // 使通道2输入使能
}
```

### 2.2 捕获/比较使能寄存器（TIM14_CCER）

接下来，我们介绍 TIM14 的捕获/比较使能寄存器（TIM14_CCER），该寄存器控制着各个输入输出通道的开关。

![屏幕截图 2024 09 16 131422](https://img.picgo.net/2024/09/16/-2024-09-16-1314229a5ef43994a79c2a.png)

该寄存器比较简单， 我们这里只用到了 CC1E 位，该位是输入/捕获 1 输出使能位，要想PWM 从 IO 口输出，这个位必须设置为 1，所以我们需要设置该位为 1

### 2.3 捕获/比较寄存器（TIMx_CCR1~4）

最后，我们介绍一下捕获/比较寄存器（TIMx_CCR1~4），该寄存器总共有 4 个，对应 4 个通道 CH1~4。 不过 TIM14 只有一个，即： TIM14_CCR1

![屏幕截图 2024 09 16 131515](https://img.picgo.net/2024/09/16/-2024-09-16-13151583845dc8071c1c74.png)

在输出模式下，该寄存器的值与 CNT 的值比较，根据比较结果产生相应动作。利用这点，我们通过修改这个寄存器的值，就可以控制 PWM 的输出脉宽了。

- **TIMx_CCR1**: 通道 1 的捕获/比较寄存器
- **TIMx_CCR2**: 通道 2 的捕获/比较寄存器
- **TIMx_CCR3**: 通道 3 的捕获/比较寄存器
- **TIMx_CCR4**: 通道 4 的捕获/比较寄存器

- **输出比较模式**：
  
  - 设置比较值，当定时器计数器达到该值时，触发相应的事件（如翻转输出状态、产生中断等）。

- **输入捕获模式**：
  
  - 存储在输入信号上检测到的时间戳（即计数器的值），可用于后续的时间测量。

以下是配置 TIM2 的 CCR 寄存器以实现 PWM 输出和输入捕获的代码示例：

```c
#include "stm32f4xx.h"  // 根据你的 MCU 型号包含相应的头文件

void TIM2_Config(void) {
    // 1. 使能定时器时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // 2. 配置基本参数
    TIM2->PSC = 7999;  // 预分频器设置
    TIM2->ARR = 1000;  // 自动重载值设置
    // 3. 配置通道1为PWM输出
    TIM2->CCMR1 &= ~TIM_CCMR1_CC1S;          // 清除 CC1S 位，选择输出比较
    TIM2->CCMR1 |= (6 << TIM_CCMR1_OC1M_Pos); // 设置为 PWM 模式 1
    TIM2->CCMR1 |= TIM_CCMR1_OC1PE;          // 使能预加载
    // 设置比较值
    TIM2->CCR1 = 500;  // 设置占空比为 50%
    // 4. 配置通道2为输入捕获模式
    TIM2->CCMR1 &= ~(3 << TIM_CCMR1_CC2S_Pos); // 清除 CC2S 位，选择输入捕获
    TIM2->CCMR1 |= (1 << TIM_CCMR1_IC2F_Pos);  // 设置输入捕获滤波器
    // 5. 启动定时器
    TIM2->CR1 |= TIM_CR1_CEN;  // 启动定时器
    // 6. 使能通道1和通道2
    TIM2->CCER |= TIM_CCER_CC1E; // 使能通道1输出
    TIM2->CCER |= TIM_CCER_CC2E; // 使能通道2输入
}
```

### 2.4 刹车和死区寄存器（TIMx_BDTR）

如果是通用定时器，则配置以上三个寄存器就够了，但是如果是高级定时器，则还需要配置：刹车和死区寄存器（TIMx_BDTR）

![屏幕截图 2024 09 16 131548](https://img.picgo.net/2024/09/16/-2024-09-16-131548067da6531488513c.png)

该寄存器，我们只需要关注最高位： MOE 位，要想高级定时器的 PWM 正常输出，则必须设置 MOE 位为 1，否则不会有输出。

---

#### 1. **用途**

- **刹车功能**：用于在特定条件下立即停止 PWM 输出，以保护设备。
- **死区时间**：在输出信号之间插入一个小的延迟，以避免短路或其他电气问题。

#### 2. **寄存器描述**

- **TIMx_BDTR**: 包含多个位，用于配置刹车和死区时间的特性。

#### 3. **主要字段**

- **MOE (Main Output Enable)**: 启用主输出。如果设置为 1，PWM 输出将被激活。
- **AOE (Auto-Output Enable)**: 自动输出使能。当设置为 1 时，定时器会自动生成 PWM 输出。
- **BKINE (Break Input Enable)**: 使能刹车输入信号。
- **BKDFN (Break Filter)**: 配置刹车输入滤波器的数量。
- **OSS (Off-State Selection)**: 当启用刹车时，输出状态选择。可以选择将输出设置为高阻态或低状态。
- **DBL (Dead-Time Length)**: 配置死区时间的长度。

#### 4. **基本使用示例**

以下是配置 TIM2 的 BDTR 寄存器的代码示例：

```c
#include "stm32f4xx.h"  // 根据你的 MCU 型号包含相应的头文件

void TIM2_BDTR_Config(void) {
    // 1. 使能定时器时钟
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
    // 2. 配置定时器基础参数
    TIM2->PSC = 7999;  // 预分频
    TIM2->ARR = 1000;  // 自动重载值
    // 3. 配置比较模式 (假设已经完成)
    // ...
    // 4. 配置 BDTR 寄存器
    TIM2->BDTR = 0;                  // 清零
    TIM2->BDTR |= TIM_BDTR_MOE;     // 使能主输出
    TIM2->BDTR |= (5 << TIM_BDTR_DBL_Pos); // 设置死区时间长度
    TIM2->BDTR |= TIM_BDTR_BKINE;   // 使能刹车输入
    // 5. 启动定时器
    TIM2->CR1 |= TIM_CR1_CEN;       // 启动定时器
}
```

## 3. 基本操作步骤

### 3.1 开启TIM14和GPIO时钟，配置 PF9 选择复用功能 AF9（TIM14） 输出

要使用 TIM14，我们必须先开启 TIM14 的时钟，这点相信大家看了这么多代码，应该明白了。这里我们还要配置 PF9 为复用（AF9） 输出， 才可以实现 TIM14_CH1 的 PWM 经过 PF9输出。 HAL 库使能 TIM14 时钟和 GPIO 时钟方法是：

```c
__HAL_RCC_TIM14_CLK_ENABLE(); // 使能定时器 14
__HAL_RCC_GPIOF_CLK_ENABLE(); // 开启 GPIOF 时钟
```

接下来便是要配置 PF9 复用映射为 TIM3 的 PWM 输出引脚。关于 IO 口复用映射，在串口通信实验中有详细讲解，主要是通过函数 HAL_GPIO_Init 来实现的：

```c
GPIO_InitTypeDef GPIO_Initure;
__HAL_RCC_TIM14_CLK_ENABLE();  //使能定时器 14
__HAL_RCC_GPIOF_CLK_ENABLE();  // 开启 GPIOF 时钟
GPIO_Initure.Pin = GPIO_PIN_9; // PF9
GPIO_Initure.Mode = GPIO_MODE_AF_PP; // 复用推挽输出
GPIO_Initure.Pull  =GPIO_PULLUP; //上拉
GPIO_Initure.Speed = GPIO_SPEED_HIGH; // 高速
GPIO_Initure.Alternate = GPIO_AF9_TIM14; // PF9 复用为 TIM14_CH1
HAL_GPIO_Init(GPIOF,&GPIO_Initure);
```

这里还需要说明一下，对于定时器通道的引脚关系，大家可以查看 STM32F4 对应的数据手册，比如我们 PWM 实验，我们使用的是定时器 14 的通道 1，对应的引脚 PF9 可以从数据手册表中查看：

![屏幕截图 2024 09 16 132058](https://img.picgo.net/2024/09/16/-2024-09-16-13205870c4165023f66f95.png)

### 3.2 初始化TIM14，设置TIM14的ARR和PSC等参数

根据前面的讲解，初始化定时器的 ARR 和 PSC 等参数是通过函数 HAL_TIM_Base_Init 来实现的，但是这里大家要注意，对于我们使用定时器的 PWM 输出功能时， HAL 库为我们提供了一个独立的定时器初始化函数 HAL_TIM_PWM_Init，该函数声明为：

```c
HAL_StatusTypeDef HAL_TIM_PWM_Init(TIM_HandleTypeDef *htim);
```

该函数实现的功能以及使用方法和 HAL_TIM_Base_Init 都是类似的，作用都是初始化定时器 的 ARR 和 PSC 等参 数 。 为什么 HAL 库要提供这个函数而不直接让我们使用HAL_TIM_Base_Init 函数呢？

这是因为 HAL 库为定时器的 PWM 输出定义了单独的 MSP 回调函数HAL_TIM_PWM_MspInit，也就是说，当我们调用HAL_TIM_PWM_Init进行 PWM 初始化之后，该函数内部会调用 MSP 回调函数 HAL_TIM_PWM_MspInit。而当我们使用 HAL_TIM_Base_Init 初始化定时器参数的时候，它内部调用的回调函数为 HAL_TIM_Base_MspInit，这里大家注意区分。

所以大家一定要注意，使用 HAL_TIM_PWM_Init 初始化定时器时，回调函数为： HAL_TIM_PWM_MspInit，该函数声明为：

```c
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
```

### 3.3 设置TIM14_CH1的PWM模式，使能TIM14的CH1输出

接下来，我们要设置 TIM14_CH1 为 PWM 模式（默认是冻结的），因为我们的 DS0 是低电平亮，而我们希望当 CCR1 的值小的时候， DS0 就暗， CCR1 值大的时候， DS0 就亮，所以我们要通过配置 TIM14_CCMR1 的相关位来控制 TIM14_CH1 的模式。

在 HAL 库中， PWM 通道设置是通过函数 HAL_TIM_PWM_ConfigChannel 来设置的：

```c
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim, TIM_OC_InitTypeDef* sConfig, uint32_t Channel);
```

第一个参数 htim 是定时器初始化句柄，也就是 TIM_HandleTypeDef 结构体指针类型，这和 HAL_TIM_PWM_Init 函数调用时候参数保存一致即可。

第二个参数 sConfig 是 TIM_OC_InitTypeDef 结构体指针类型，这也是该函数最重要的参数。该参数用来设置 PWM 输出模式，极性，比较值等重要参数。首先我们来看看结构体定义：

```c
typedef struct
{
    uint32_t OCMode;     // PWM 模式
    uint32_t Pulse;      // 捕获比较值
    uint32_t OCPolarity; // 极性
    uint32_t OCNPolarity;
    uint32_t OCFastMode; //快速模式
    uint32_t OCIdleState;
    uint32_t OCNIdleState;
} TIM_OC_InitTypeDef
```

该结构体成员我们重点关注前三个。成员变量 OCMode 用来设置模式，也就是我们前面讲解的7 种模式，这里我们设置为 PWM 模式 1。成员变量 Pulse 用来设置捕获比较值。成员变量TIM_OCPolarity 用来设置输出极性是高还是低。其他的参数 TIM_OutputNState ， TIM_OCNPolarity， TIM_OCIdleState 和 TIM_OCNIdleState 是高级定时器才用到的。

第三个参数 Channel 用来选择定时器的通道,取值范围为 TIM_CHANNEL_1~ TIM_CHANNEL_4。这里我们使用的是定时器 14 的通道 1，所以取值为 TIM_CHANNEL_1 即可

例如我们要初始化定时器TIM3的通道4为 PWM 模式1，输出极性为低，那么实例代码为：

```c
TIM_OC_InitTypeDef TIM14_CH1Handler; // 定时器 14 通道 1 句柄
TIM3_CH4Handler.OCMode = TIM_OCMODE_PWM1; // 模式选择 PWM1
TIM3_CH4Handler.Pulse = arr/2; // 设置比较值,此值用来确定占空比
TIM3_CH4Handler.OCPolarity = TIM_OCPOLARITY_LOW; // 输出比较极性为低
HAL_TIM_PWM_ConfigChannel(&TIM3_Handler,&TIM3_CH4Handler,TIM_CHANNEL_4);
```

### 3.4 使能TIM14

在完成上面的设置之后，我们需要使能TIM14：

```c
HAL_StatusTypeDef HAL_TIM_PWM_Start(TIM_HandleTypeDef *htim, uint32_t Channel);
```

函数第二个入口参数 Channel 是用来设置要使能输出的通道号。

对于单独使能定时器的方法，在上一章定时器实验我们已经讲解。实际上， HAL 库也同样提供了单独使能定时器的输出通道函数，函数为：

```c
void TIM_CCxChannelCmd(TIM_TypeDef* TIMx, uint32_t Channel, uint32_t ChannelState);
```

### 3.5 修订TIM14_CCR1来控制占空比

最后，在经过以上设置之后， PWM 其实已经开始输出了，只是其占空比和频率都是固定的，而我们通过修改比较值 TIM14_CCR1 则可以控制 CH1 的输出占空比。继而控制 DS0 的亮度。 HAL 库中并没有提供独立的修改占空比函数，这里我们可以编写这样一个函数如下：

```c
// 设置 TIM 通道 4 的占空比
// compare:比较值
void TIM_SetTIM14Compare1(u32 compare)
{
    TIM14->CCR1 = compare;
}
```

实际上，因为调用函数 HAL_TIM_PWM_ConfigChanne 进行 PWM 配置的时候可以设置比较值，所以我们也可以直接使用该函数来达到修改占空比的目的:

```c
void TIM_SetCompare1(TIM_TypeDef *TIMx, u32 compare)
{
    TIM14_CH1Handler.Pulse = compare;
    HAL_TIM_PWM_ConfigChannel(&TIM14_Handler,&TIM14_CH1Handler,TIM_CHANNEL_1);
}
```

这种方法因为要调用 HAL_TIM_PWM_ConfigChannel 函数对各种初始化参数进行重新设置，所以大家在使用中一定要注意，例如在实时系统中如果多个线程同时修改初始化结构体相关参数，可能导致结果混乱。

---

2024.10.3 第一次修订，后期不再维护
