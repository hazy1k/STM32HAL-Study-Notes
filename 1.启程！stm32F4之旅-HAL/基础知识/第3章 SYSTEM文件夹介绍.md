# 第三章 SYSTEM文件夹介绍

我们介绍了如何在 MDK5 下建立 STM32F4 工程。 在这个新建的工程之中，我们用到了一个 SYSTEM 文件夹里面的代码，此文件夹里面的代码由 ALIENTEK 提供，是STM32F4xx 系列的底层核心驱动函数，可以用在 STM32F4xx 系列的各个型号上面，方便大家快速构建自己的工程。

SYSTEM 文件夹下包含了 delay、 sys、 usart 等三个文件夹。分别包含了 delay.c、 sys.c、 usart.c及其头文件。通过这 3 个 c 文件，可以快速的给任何一款 STM32F4 构建最基本的框架。使用起来是很方便的。

本章， 我们将向大家介绍这些代码，通过这章的学习，大家将了解到这些代码的由来，也希望大家可以灵活使用 SYSTEM 文件夹提供的函数，来快速构建工程，并实际应用到自己的项目中去。

## 1. delay文件夹代码介绍

delay 文件夹内包含了 delay.c 和 delay.h 两个文件，这两个文件用来实现系统的延时功能，其中包含 7 个函数：

- void delay_osschedlock(void);

- void delay_osschedunlock(void); 

- void delay_ostimedly(u32 ticks); 

- void SysTick_Handler(void);

- void delay_init(u8 SYSCLK); 

- void delay_ms(u16 nms); 

- void delay_us(u32 nus);

前面 4 个函数，仅在支持操作系统（OS）的时候，需要用到，而后面 3 个函数，则不论是否支持 OS 都需要用到。

在介绍这些函数之前，我们先了解一下编程思想： CM4 内核的处理和 CM3 一样，内部都包含了一个 SysTick 定时器， SysTick 是一个 24 位的倒计数定时器，当计到 0 时，将从 RELOAD寄存器中自动重装载定时初值。只要不把它在 SysTick 控制及状态寄存器中的使能位清除，就永不停息。

这里我们将介绍的是 ALIENTEK 提供的最新版本的延时函数，该版本的延时函数支持在任意操作系统（OS）下面使用，它可以和操作系统共用 SysTick 定时器。

这里，我们以 UCOSII 为例，介绍如何实现操作系统和我们的 delay 函数共用 SysTick 定时器。首先，我们简单介绍下 UCOSII 的时钟： ucos 运行需要一个系统时钟节拍（类似 “心跳”），而这个节拍是固定的（由 OS_TICKS_PER_SEC 宏定义设置），比如要求 5ms 一次（即可设置： OS_TICKS_PER_SEC=200），在 STM32 上面，一般是由 SysTick 来提供这个节拍，也就是 SysTick要设置为 5ms 中断一次，为 ucos 提供时钟节拍，而且这个时钟一般是不能被打断的（否则就不准了）。

因为在 ucos 下 systick 不能再被随意更改，如果我们还想利用 systick 来做 delay_us 或者delay_ms 的延时，就必须想点办法了，这里我们利用的是时钟摘取法。以 delay_us 为例，比如delay_us（50），在刚进入 delay_us 的时候先计算好这段延时需要等待的 systick 计数次数，这里为 50*21（假设系统时钟为 168Mhz，因为 systick 的频率为系统时钟频率的 1/8，那么 systick每增加 1，就是 1/21us），然后我们就一直统计 systick 的计数变化，直到这个值变化了 50*21，一旦检测到变化达到或者超过这个值，就说明延时 50us 时间到了。这样，我们只是抓取 SysTick计数器的变化，并不需要修改 SysTick 的任何状态，完全不影响 SysTick 作为 UCOS 时钟节拍的功能，这就是实现 delay 和操作系统共用 SysTick 定时器的原理。
