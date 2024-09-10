# 第五章 MDK代码快速组织代码技巧

现 在 我 们 要 初 始 化 某 个 GPIO 端 口 ， 我 们 要 怎 样 快 速 操 作 呢 ？ 在 头 文 件stm32f4xx_hal_gpio.h 头文件中，声明 GPIO 初始化函数为：

```c
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init);
```

现在我们想写初始化函数，那么我们在不参考其他代码的前提下，怎么快速组织代码呢？

首先，我们可以看出，函数的入口参数是 GPIO_TypeDef 类型指针和 GPIO_InitTypeDef 类型指针 ，因为 GPIO_TypeDef 入口参数比较简单 ，所以我们 就通过第二个入口参数GPIO_InitTypeDef 类型指针来讲解。双击 GPIO_InitTypeDef 后右键选择“Go to definition of…”，

![屏幕截图 2024 09 10 154455](https://img.picgo.net/2024/09/10/-2024-09-10-154455ca6788efb4b96418.png)

于是定位到 stm32f4xx_hal_gpio.h 中 GPIO_InitTypeDef 的定义处：

```c
typedef struct
{    
    uint32_t Pin;
    uint32_t Mode;
    uint32_t Pull;
    uint32_t Speed;
    uint32_t Alternate;
}GPIO_InitTypeDef
```

可以看到这个结构体有 5 个成员变量，这也告诉我们一个信息，一个 GPIO 口的状态是由模式（Mode），速度(Speed)以及上下拉（Pull） 来决定的。我们首先要定义一个结构体变量，下面我们定义:

```c
GPIO_InitTypeDef GPIO_InitStructure;
```

接着我们要初始化结构体变量 GPIO_InitStructure。首先我们要初始化成员变量 Pin,这个时候我们就有点迷糊了，这个变量到底可以设置哪些值呢？这些值的范围有什么规定吗？

这里我们就回到 HAL_GPIO_Init 声明处，同样双击 HAL_GPIO_Init，右键点击“Go to definition of …”,这样光标定位到 stm32f4xx_hal_gpio.c 文件中的 HAL_GPIO_Init 函数体开始处，我们可以看到在函数中有如下几行：

```c
void HAL_GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init)
{
…//此处省略部分代码

    assert_param(IS_GPIO_ALL_INSTANCE(GPIOx));
    assert_param(IS_GPIO_PIN(GPIO_Init->Pin));
    assert_param(IS_GPIO_MODE(GPIO_Init->Mode));
    assert_param(IS_GPIO_PULL(GPIO_Init->Pull));
…//此处省略部分代码
    assert_param(IS_GPIO_AF(GPIO_Init->Alternate));
…//此处省略部分代码
}
```

顾名思义， assert_param 是断言语句，是对函数入口参数的有效性进行判断，所以我们可以从这个函数入手，确定入口参数范围。第一行是对第一个参数 GPIOx 进行有效性判断，双击“IS_GPIO_ALL_INSTANCE”右键点击“go to defition of…” 定位到了下面的定义：

```c
#define IS_GPIO_ALL_INSTANCE(INSTANCE) (((INSTANCE) == GPIOA) || \
((INSTANCE) == GPIOB) || \
((INSTANCE) == GPIOC) || \
((INSTANCE) == GPIOD) || \
…//此处省略部分代码
((INSTANCE) == GPIOJ) || \
((INSTANCE) == GPIOK))
```

很明显可以看出， GPIOx 的取值规定只允许是 GPIOA~GPIOK。

同样的办法，我们双击“IS_GPIO_PIN” 右键点击“ go to defition of…” ,定位到下面的定义：

```c
#define IS_GPIO_PIN(PIN) (((PIN) & GPIO_PIN_MASK ) != (uint32_t)0x00)
```

同时，宏定义标识符 GPIO_PIN_MASK 的定义为：

```c
#define GPIO_PIN_MASK ((uint32_t)0x0000FFFF)
```

从上面可以看出， PIN 取值只要低 16 位不为 0 即可。这里需要大家注意，因为一组 IO 口只有16 个 IO，实际上 PIN 的值在这里只有低 16 位有效，所以 PIN 的取值范围为 0x0001~0xFFFF。那么是不是我们写代码初始化就是直接给一个 16 位的数字呢？这也是可以的，但是大多数情况下，我们不会直接在入口参数处设置一个简单的数字， 因为这样代码的可读性太差， HAL库会将这些数字的含义通过宏定义定义出来，这样可读性大大增强。我们可以看到在GPIO_PIN_MASK 宏定义的上面还有数行宏定义：

```c
#define GPIO_PIN_0 ((uint16_t)0x0001)
#define GPIO_PIN_1 ((uint16_t)0x0002)
#define GPIO_PIN_2 ((uint16_t)0x0004)
…//此处省略部分定义
#define GPIO_PIN_14 ((uint16_t)0x4000)
#define GPIO_PIN_15 ((uint16_t)0x8000)
#define GPIO_PIN_All ((uint16_t)0xFFFF)
```

这些宏定义 GPIO_PIN_0 ~ GPIO_PIN_All 就是 HAL 库事先定义好的，我们写代码的时候初始化结构体 成员变量 Pin 的时候入口参数可以是这些宏定义标识符。

同理，对于成员变量 Pull，我们用同样的方法，可以找到其取值范围定义为：

```c
#define IS_GPIO_PULL(PULL) (((PULL) == GPIO_NOPULL)\
|| ((PULL) == GPIO_PULLUP) || \ ((PULL) == GPIO_PULLDOWN))
```

也就是PULL 的取值范围只能是标识符 GPIO_NOPULL ， GPIO_PULLUP 以及GPIO_PULLDOWN。

对于其他成员变量 Mode 以及 Alternate，方法都是一样的，这里基于篇幅考虑我们就不重复讲解。 讲到这里，我们基本对 HAL_GPIO_Init 的入口参数有比较详细的了解了。于是我们可以组织起来下面的代码：

```c
GPIO_InitTypeDef GPIO_Initure;
GPIO_Initure.Pin = GPIO_PIN_9; //PA9
GPIO_Initure.Mode = GPIO_MODE_AF_PP; //复用推挽输出
GPIO_Initure.Pull = GPIO_PULLUP; //上拉
GPIO_Initure.Speed = GPIO_SPEED_FAST; //高速
GPIO_Initure.Alternate = GPIO_AF7_USART1; //复用为 USART1
HAL_GPIO_Init(GPIOA,&GPIO_Initure); //初始化 PA9
```
