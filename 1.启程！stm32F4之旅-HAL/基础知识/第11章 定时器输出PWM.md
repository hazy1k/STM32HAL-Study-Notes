# 第十一章 定时器输出PWM

## 1. PWM简介

脉冲宽度调制(PWM)，是英文“Pulse Width Modulation” 的缩写，简称脉宽调制，是利用微处理器的数字输出来对模拟电路进行控制的一种非常有效的技术。简单一点，就是对脉冲宽度的控制

STM32F4 的定时器除了 TIM6 和 7。其他的定时器都可以用来产生 PWM 输出。其中高级定时器 TIM1 和 TIM8 可以同时产生多达 7 路的 PWM 输出。而通用定时器也能同时产生多达 4路的 PWM 输出！这里我们仅使用 TIM14 的 CH1 产生一路 PWM 输出。

要使 STM32F4 的通用定时器 TIMx 产生 PWM 输出，除了上一章介绍的寄存器外，我们还会用到 3 个寄存器，来控制 PWM 的。这三个寄存器分别是：捕获/比较模式寄存器（TIMx_CCMR1/2）、捕获/比较使能寄存器（TIMx_CCER）、捕获/比较寄存器（TIMx_CCR1~4）。接下来我们简单介绍一下这三个寄存器。

## 2. 相关寄存器介绍

首先是捕获/比较模式寄存器（TIMx_CCMR1/2），该寄存器一般有 2 个： TIMx _CCMR1和 TIMx _CCMR2，不过 TIM14 只有一个。 TIMx_CCMR1 控制 CH1 和 2，而 TIMx_CCMR2控制 CH3 和 4。以下我们将以 TIM14 为例进行介绍。TIM14_CCMR1 寄存器各位描述如图：

![屏幕截图 2024 09 16 131201](https://img.picgo.net/2024/09/16/-2024-09-16-131201e3622c7f28167b12.png)

该寄存器的有些位在不同模式下，功能不一样，所以在图中，我们把寄存器分了 2层，上面一层对应输出而下面的则对应输入。

这里我们需要说明的是模式设置位 OC1M，此部分由 3位组成。总共可以配置成 7 种模式，我们使用的是 PWM 模式，所以这 3 位必须设置为 110/111。这两种 PWM 模式的区别就是输出电平的极性相反。 另外 CC1S 用于设置通道的方向（输入/输出）默认设置为 0，就是设置通道作为输出使用。 注意：这里是因为我们的 TIM14 只有 1 个通道，所以才只有第八位有效，高八位无效，其他有多个通道的定时器，高八位也是有效的

接下来，我们介绍 TIM14 的捕获/比较使能寄存器（TIM14_CCER），该寄存器控制着各个输入输出通道的开关。

![屏幕截图 2024 09 16 131422](https://img.picgo.net/2024/09/16/-2024-09-16-1314229a5ef43994a79c2a.png)

该寄存器比较简单， 我们这里只用到了 CC1E 位，该位是输入/捕获 1 输出使能位，要想PWM 从 IO 口输出，这个位必须设置为 1，所以我们需要设置该位为 1

最后，我们介绍一下捕获/比较寄存器（TIMx_CCR1~4），该寄存器总共有 4 个，对应 4 个通道 CH1~4。 不过 TIM14 只有一个，即： TIM14_CCR1

![屏幕截图 2024 09 16 131515](https://img.picgo.net/2024/09/16/-2024-09-16-13151583845dc8071c1c74.png)

在输出模式下，该寄存器的值与 CNT 的值比较，根据比较结果产生相应动作。利用这点，我们通过修改这个寄存器的值，就可以控制 PWM 的输出脉宽了。

如果是通用定时器，则配置以上三个寄存器就够了，但是如果是高级定时器，则还需要配置：刹车和死区寄存器（TIMx_BDTR）

![屏幕截图 2024 09 16 131548](https://img.picgo.net/2024/09/16/-2024-09-16-131548067da6531488513c.png)

该寄存器，我们只需要关注最高位： MOE 位，要想高级定时器的 PWM 正常输出，则必须设置 MOE 位为 1，否则不会有输出。

本章，我们使用的是 TIM14 的通道 1，所以我们需要修改 TIM14_CCR1 以实现脉宽控制DS0 的亮度。至此，我们把本章要用的几个相关寄存器都介绍完了， 本章要实现通过 TIM14_CH1输出 PWM 来控制 DS0 的亮度。 下面我们介绍通过库函数来配置该功能的步骤。

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
__HAL_RCC_TIM14_CLK_ENABLE(); //使能定时器 14
__HAL_RCC_GPIOF_CLK_ENABLE(); // 开启 GPIOF 时钟
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

这是因为 HAL 库为定时器的 PWM 输出定义了单独的 MSP 回调函数HAL_TIM_PWM_MspInit，也就是说，当我们调用HAL_TIM_PWM_Init进行PWM初始化之后，该函数内部会调用 MSP 回调函数 HAL_TIM_PWM_MspInit。而当我们使用 HAL_TIM_Base_Init初始化定时器参数的时候，它内部调用的回调函数为 HAL_TIM_Base_MspInit，这里大家注意区分。

所以大家一定要注意，使用 HAL_TIM_PWM_Init 初始化定时器时，回调函数为： HAL_TIM_PWM_MspInit，该函数声明为：

```c
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim);
```

### 3.3 设置TIM14_CH1的PWM模式，使能TIM14的CH1输出

接下来，我们要设置 TIM14_CH1 为 PWM 模式（默认是冻结的），因为我们的 DS0 是低电平亮，而我们希望当 CCR1 的值小的时候， DS0 就暗， CCR1 值大的时候， DS0 就亮，所以我们要通过配置 TIM14_CCMR1 的相关位来控制 TIM14_CH1 的模式。

在 HAL 库中， PWM 通道设置是通过函数 HAL_TIM_PWM_ConfigChannel 来设置的：

```c
HAL_StatusTypeDef HAL_TIM_PWM_ConfigChannel(TIM_HandleTypeDef *htim,
TIM_OC_InitTypeDef* sConfig, uint32_t Channel);
```

第一个参数 htim 是定时器初始化句柄，也就是 TIM_HandleTypeDef 结构体指针类型，这和 HAL_TIM_PWM_Init 函数调用时候参数保存一致即可。

第二个参数 sConfig 是 TIM_OC_InitTypeDef 结构体指针类型，这也是该函数最重要的参数。该参数用来设置 PWM 输出模式，极性，比较值等重要参数。首先我们来看看结构体定义：

```c
typedef struct
{
    uint32_t OCMode; // PWM 模式
    uint32_t Pulse; // 捕获比较值
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
