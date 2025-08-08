# 第十六章 ADC介绍及应用

## 1. ADC简介

ADC 即模拟数字转换器，英文详称 Analog-to-digital converter，可以将外部的模拟信号转换为数字信号。

STM32F4xx 系列芯片拥有 3 个 ADC，这些 ADC 可以独立使用，其中 ADC1 和 ADC2 还可以组成双重模式（提高采样率）。 STM32 的 ADC 是 12 位逐次逼近型的模拟数字转换器。它有 19 个通道，可测量 16 个外部和 2 个内部信号源和 Vbat 通道的信号 ADC 中的各个通道的A/D 转换可以单次、连续、扫描或间断模式执行。 ADC 的结果可以以左对齐或者右对齐存储在16 位数据寄存器中。 ADC 具有模拟看门狗的特性，允许应用检测输入电压是否超过了用户自定义的阈值上限或下限。

STM32F407 的 ADC 主要特性我们可以总结为以下几条：

1. 可配置 12 位、 10 位、 8 位或 6 位分辨率；

2. 转换结束、注入转换结束和发生模拟看门狗事件时产生中断

3. 单次和连续转换模式

4. 自校准

5. 带内嵌数据一致性的数据对齐

6. 采样间隔可以按通道分别编程

7. 规则转换和注入转换均有外部触发选项

8. 间断模式

9. 双重模式（带 2 个或以上 ADC 的器件）

10. ADC 转换时间： 最大转换速率为 2.4MHz，转换时间为 0.41us

11. ADC 供电要求： 2.4V 到 3.6V

12. ADC 输入范围： VREF–≤VIN≤VREF+

13. 规则通道转换期间有 DMA 请求产生

下面来介绍 ADC 的框图：

![屏幕截图 2025-08-08 153320.png](https://raw.githubusercontent.com/hazy1k/My-drawing-bed/main/2025/08/08-15-33-29-屏幕截图%202025-08-08%20153320.png)

### 1.1 输入电压

在前面 ADC 的主要特性也对输入电压有所提及， ADC 输入范围 VREF–≤VIN≤VREF+，最终还是由 VREF–、 VREF+、 VDDA和 VSSA决定的。下面看一下这几个参数的关系

![屏幕截图 2025-08-08 153402.png](https://raw.githubusercontent.com/hazy1k/My-drawing-bed/main/2025/08/08-15-34-07-屏幕截图%202025-08-08%20153402.png)

从上图可以知道， VDDA 和 VREF+接 VCC3.3，而 VSSA和 VREF-是接地，所以 ADC 的输入范围即 0~3.3V。 R55 默认焊接， R54 默认不焊接。

### 1.2 输入通道

在确定好了 ADC 输入电压后，如何把外部输入的电压输送到 ADC 转换器中呢，在这里引入了 ADC 的输入通道，在前面也提及到了 ADC1 有 16 个外部通道和 3 个内部通道，而 ADC2和 ADC3 只有有 16 个外部通道。 ADC1 的外部通道是通道 17、通道 18 和通道 19，分别连接到内部温度传感器、内部Vrefint和Vbat。

![屏幕截图 2025-08-08 153826.png](https://raw.githubusercontent.com/hazy1k/My-drawing-bed/main/2025/08/08-15-38-31-屏幕截图%202025-08-08%20153826.png)

### 1.3 转换顺序

 当任意 ADCx 多个通道以任意顺序进行一系列转换就诞生了成组转换，这里就有两种成组转换类型：规则组和注入组。规则组就是图上的规则通道，注入组也就是图上的注入通道。为了避免大家对输入通道加上规则通道和注入通道理解有所模糊，后面规则通道以规则组来代称，注入通道以注入组来代称。

规则组允许最多 16 个输入通道进行转换，而注入组允许最多 4 个输入通道进行转换。这里讲解一下规则组和注入组。

规则组（规则通道）

规则组，按字面理解，“规则”就是按照一定的顺序，相当于正常运行的程序，平常用到最多也是规则组。

注入组（注入通道）

注入组，按字面理解，“注入”就是打破原来的状态，相当于中断。当程序执行的时候，中断是可以打断程序的执行。同这个类似，注入组转换可以打断规则组的转换。假如在规则组转换过程中，注入组启动，那么注入组被转换完成之后，规则组才得以继续转换。

![屏幕截图 2025-08-08 153921.png](https://raw.githubusercontent.com/hazy1k/My-drawing-bed/main/2025/08/08-15-39-25-屏幕截图%202025-08-08%20153921.png)

### 1.4 触发源

在配置好输入通道以及转换顺序后，就可以进行触发转换了。 ADC 的触发转换有两种方法：分别是通过软件或外部事件(也就是硬件)触发转换。

我们先来看看通过写软件触发转换的方法。方法是：通过写 ADC_CR2 寄存器的 ADON 这个位来控制，写 1 就开始转换，写 0 就停止转换，这个控制 ADC 转换的方式非常简单。

另一种就是通过外部事件触发转换的方法，有定时器和输入引脚触发等等。这里区分规则组和注入组。方法是：通过 ADC_CR2 寄存器的 EXTSET[2:0]选择规则组的触发源， JEXTSET[2:0]选择注入组的触发源。通过 ADC_CR2 的 EXTTRIG 和 JEXTTRIG 这两位去激活触发源。

### 1.5 转换时间

STM32F407 的 ADC 总转换时间的计算公式如下：

```c
TCONV = 采样时间 + 12 个周期
```

采样时间可通过 ADC_SMPR1 和 ADC_SMPR2 寄存器中的 SMP[2:0]位编程， ADC_SMPR2控制的是通道 0~9， ADC_SMPR1 控制的是通道 10~18。所有通道都可以通过编程来控制使用不同的采样时间，可选采样时间值如下：

- SMP = 000： 3 个 ADC 时钟周期

- SMP = 001： 15 个 ADC 时钟周期

- SMP = 010： 28 个 ADC 时钟周期

- SMP = 011： 56 个 ADC 时钟周期

- SMP = 100： 84 个 ADC 时钟周期

- SMP = 101： 112 个 ADC 时钟周期

- SMP = 110： 144 个 ADC 时钟周期

- SMP = 111： 480 个 ADC 时钟周期

12 个周期是 ADC 输入时钟 ADC_CLK 决定的。 ADC_CLK 是由 APB2 经过分频产生，分频系数是由 RCC_CFGR 寄存器中的 PPRE2[2:0]进行设置，有 2/4/6/8/16 分频选项。

采样时间最小是 3 个时钟周期，这个采样时间下，我们可以得到最快的采样速度。 举个例子，我们采用最高的采样速率，使用采样时间为 3 个 ADC 时钟周期，那么得到：

```c
TCONV = 3 个 ADC 时钟周期 + 12 个 ADC 时钟周期 = 15 个 ADC 时钟周期
```

一般 APB2 的时钟是 84MHz，经过 ADC 分频器的 4 分频后， ADC 时钟频率就为 21MHz。通过换算可得到：

```c
TCONV = 15 个 ADC 时钟周期 = ( 1/21000000) ∗ 15 s = 0.71us
```

### 1.6 数据寄存器

ADC 转换完成后的数据输出寄存器。根据转换组的不同，规则组的完成转换的数据输出到ADC_DR 寄存器，注入组的完成转换的数据输出到 ADC_JDRx 寄存器。假如是使用双重模式，规则组的数据也是存放在 ADC_DR 寄存器。

### 1.7 中断

规则和注入组转换结束时能产生中断，当模拟看门狗状态位被设置时也能产生中断。它们在 ADC_SR 中都有独立的中断使能位，后面讲解 ADC_SR 寄存器时再进行展开。这里讲解一下，模拟看门狗中断以及 DMA 请求。

模拟看门狗中断

模拟看门狗中断发生条件：首先通过ADC_LTR和ADC_HTR寄存器设置低阈值和高阈值，然后开启了模拟看门狗中断后，当被 ADC 转换的模拟电压低于低阈值或者高于高阈值时，就会产生中断。例如我们设置高阈值是 3.0V，那么模拟电压超过 3.0V 的时候，就会产生模拟看门狗中断，低阈值的情况类似。

DMA 请求

规则组和注入组的转换结束后，除了产生中断外，还可以产生 DMA 请求，把转换好的数据存储在内存里面，防止读取不及时数据被覆盖。

## 2. ADC使用示例

### 2.1 单通道ADC采集

#### 2.1.1 ADC初始化

```c
#include "adc.h"
#include "delay.h"

ADC_HandleTypeDef adc_handle;

void adc_init(void)
{
    adc_handle.Instance = ADC1;
    adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // 4分频
    adc_handle.Init.Resolution = ADC_RESOLUTION_12B; // 12位精度
    adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_handle.Init.ScanConvMode = DISABLE;
    adc_handle.Init.ContinuousConvMode = DISABLE;
    adc_handle.Init.NbrOfConversion = 1; // 单次转换
    adc_handle.Init.DiscontinuousConvMode = DISABLE;
    adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_handle.Init.DMAContinuousRequests = DISABLE;
    HAL_ADC_Init(&adc_handle);
}
```

#### 2.1.2 ADC通道配置

```c
// ADC通道配置
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance==ADC1)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();
        /* PA5 */
        GPIO_Initure.Pin = GPIO_PIN_5; 
        GPIO_Initure.Mode = GPIO_MODE_ANALOG; // 输入模式
        GPIO_Initure.Pull = GPIO_NOPULL; // 上拉
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);
    }
}
```

#### 2.1.3 设置ADC通道采样时间

```c
/**
 * @brief       设置ADC通道采样时间
 * @param       adcx : adc句柄指针,ADC_HandleTypeDef
 * @param       ch   : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: 采样时间  0~7, 对应关系为:
 *   @arg       ADC_SAMPLETIME_3CYCLES,  3个ADC时钟周期        ADC_SAMPLETIME_15CYCLES, 15个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_28CYCLES, 28个ADC时钟周期       ADC_SAMPLETIME_56CYCLES, 56个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_84CYCLES, 84个ADC时钟周期       ADC_SAMPLETIME_112CYCLES,112个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_144CYCLES,144个ADC时钟周期      ADC_SAMPLETIME_480CYCLES,480个ADC时钟周期
 * @param       rank: 多通道采集时需要设置的采集编号,
                假设你定义channel1的rank=1，channel2的rank=2，
                那么对应你在DMA缓存空间的变量数组AdcDMA[0] 就i是channel1的转换结果，AdcDMA[1]就是通道2的转换结果。 
                单通道DMA设置为 ADC_REGULAR_RANK_1
 *   @arg       编号1~16：ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      无
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;         // 设置ADCX对通道ch
    adc_channel.Rank = rank;          // 设置采样序列
    adc_channel.SamplingTime = stime; // 设置采样时间
    HAL_ADC_ConfigChannel( adc_handle, &adc_channel);   
}
```

#### 2.1.4 获取采样值

```c
// 获取ADC通道ch的转换值 
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&adc_handle, ch, 1, ADC_SAMPLETIME_480CYCLES); // 设置通道，序列和采样时间
    HAL_ADC_Start(&adc_handle); // 开启ADC
    HAL_ADC_PollForConversion(&adc_handle, 10); // 轮询转换

    return (uint16_t)HAL_ADC_GetValue(&adc_handle); // 返回最近一次ADC1规则组的转换结果
}

// 通道ch的times次转换结果平均值
uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)  
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }
    return temp_val / times;     
}
```

#### 2.1.5 主函数测试

```c
#include "bsp_init.h"
#include "stdio.h"
#include "adc.h"

int main(void)
{
  uint16_t adc_value;
  float temp;
  bsp_init();
  adc_init();
  LCD_ShowString(30,50,200,16,16,"STM32F4 ADC Test");
  LCD_ShowString(30,110,200,16,16, "ADC1_CH5_VAL:");
  LCD_ShowString(30,130,200,16,16, "ADC1_CH5_VOL:0.000V");
    while(1)
  {
    adc_value = adc_get_result_average(ADC_CHANNEL_5, 10); // 10次采样取平均值
    LCD_ShowxNum(134,110,adc_value,5,16,0);
    temp = (float)adc_value*(3.3/4096); // 计算电压值
    adc_value = temp;
    LCD_ShowxNum(134,130,adc_value,1,16,0); // 显示电压整数部分
    temp -= adc_value;
    temp *= 1000; // 计算电压小数部分
    LCD_ShowxNum(150,130,temp,3,16,0x80);
    LED_TOGGLE(LED0_GPIO_Pin);
    delay_ms(100);
  }
}
```

### 2.2 单通道ADC采集（DMA读取）

#### 2.2.1 ADC初始化

```c
#include "adc.h"
#include "delay.h"

ADC_HandleTypeDef adc_handle;
DMA_HandleTypeDef dma_handle;
uint8_t adc_dma_sta = 0; // 0:未完成，1:完成

void adc_init(void)
{
    adc_handle.Instance = ADC1;
    adc_handle.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4; // 4分频
    adc_handle.Init.Resolution = ADC_RESOLUTION_12B; // 12位精度
    adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    adc_handle.Init.ScanConvMode = DISABLE; // 单通道扫描模式禁用
    adc_handle.Init.ContinuousConvMode = DISABLE; // 连续转换模式禁用 (在DMA模式下会通过CR2_CONT使能)
    adc_handle.Init.NbrOfConversion = 1; // 单次转换
    adc_handle.Init.DiscontinuousConvMode = DISABLE;
    adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    adc_handle.Init.DMAContinuousRequests = ENABLE; // 使能DMA连续请求 (ADC生成DMA请求后，DMA会持续请求直到传输完成)
    if (HAL_ADC_Init(&adc_handle) != HAL_OK)
    {
        // 初始化失败处理，例如错误打印或死循环
        while(1);
    }
}
```

#### 2.2.2 ADC通道初始化

```c
// ADC通道配置
void HAL_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
    if(hadc->Instance == ADC1)
    {
        GPIO_InitTypeDef GPIO_Initure;
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_ADC1_CLK_ENABLE();
        /* PA5 作为ADC输入 */
        GPIO_Initure.Pin = GPIO_PIN_5;
        GPIO_Initure.Mode = GPIO_MODE_ANALOG; // 模拟输入模式
        GPIO_Initure.Pull = GPIO_NOPULL; // 无上拉/下拉
        HAL_GPIO_Init(GPIOA, &GPIO_Initure);
        // 关联ADC和DMA中断
        HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 2, 2); // DMA中断优先级
        HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
    }
}
```

#### 2.2.3 设置ADC通道采样时间

```c
/**
 * @brief       设置ADC通道采样时间
 * @param       adc_handle : adc句柄指针,ADC_HandleTypeDef
 * @param       ch   : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       rank: 多通道采集时需要设置的采集编号, 单通道DMA设置为 ADC_REGULAR_RANK_1
 * @param       stime: 采样时间
 * @retval      无
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;         // 设置ADCX对通道ch
    adc_channel.Rank = rank;          // 设置采样序列
    adc_channel.SamplingTime = stime; // 设置采样时间
    if (HAL_ADC_ConfigChannel(adc_handle, &adc_channel) != HAL_OK)
    {
        // 通道配置失败处理
        while(1);
    }
}
```

#### 2.2.4 DMA配置

```c
// DMA配置
void adc_dma_init(uint32_t address)
{
    // 确保ADC先初始化，因为DMA需要关联到ADC句柄
    adc_init();

    // 确定DMA时钟使能
    __HAL_RCC_DMA2_CLK_ENABLE(); // 对于DMA2_Stream4，直接使能DMA2时钟即可

    dma_handle.Instance = DMA2_Stream4;
    dma_handle.Init.Channel = DMA_CHANNEL_0; // 对应ADC1的DMA请求，通常是通道0
    dma_handle.Init.Direction = DMA_PERIPH_TO_MEMORY; // 外设到存储器
    dma_handle.Init.PeriphInc = DMA_PINC_DISABLE; // 外设地址不递增
    dma_handle.Init.MemInc = DMA_MINC_ENABLE; // 存储器地址递增
    dma_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; // 外设数据宽度半字 (16位)
    dma_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD; // 存储器数据宽度半字 (16位)
    dma_handle.Init.Mode = DMA_CIRCULAR; 
    dma_handle.Init.Priority = DMA_PRIORITY_MEDIUM; // DMA优先级中等
    dma_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE; // 禁用FIFO
    dma_handle.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_FULL; // FIFO阈值
    dma_handle.Init.MemBurst = DMA_MBURST_SINGLE; // 存储器突发模式单次传输
    dma_handle.Init.PeriphBurst = DMA_PBURST_SINGLE; // 外设突发模式单次传输

    if (HAL_DMA_Init(&dma_handle) != HAL_OK)
    {
        // DMA初始化失败处理
        while(1);
    }

    // 关联DMA句柄到ADC句柄
    __HAL_LINKDMA(&adc_handle, DMA_Handle, dma_handle);

    // 设置ADC通道用于DMA传输 (这里我们只用一个通道，PA5/ADC_CHANNEL_5)
    adc_channel_set(&adc_handle, ADC_CHANNEL_5, 1, ADC_SAMPLETIME_56CYCLES); // 设置通道，序列和采样时间

    // 开启ADC连续转换模式（对于DMA通常需要，以便ADC自动开始下一次转换）
    SET_BIT(adc_handle.Instance->CR2, ADC_CR2_CONT); // 使能连续转换

    // 启动DMA ADC转换
    // 第三个参数是数据传输的次数。在DMA_CIRCULAR模式下，DMA会自动循环。
    if (HAL_ADC_Start_DMA(&adc_handle, (uint32_t*)address, ADC_DMA_BUFFSIZE) != HAL_OK)
    {
        // 启动DMA转换失败处理
        while(1);
    }

    // 开启DMA传输完成中断 (在DMA_CIRCULAR模式下，每次缓冲区填充满时会触发)
    __HAL_DMA_ENABLE_IT(&dma_handle, DMA_IT_TC);
}

// 在DMA_CIRCULAR模式下，此函数通常只用于首次启动DMA，或从停止状态重新启动。
// 一旦DMA以CIRCULAR模式启动，它会自行循环，无需重复调用。
void adc_dma_enable(uint16_t times)
{
    // 检查DMA是否已经处于活跃状态，如果已经活跃，则不执行任何操作。
    // 如果DMA处于HAL_DMA_STATE_READY或HAL_DMA_STATE_BUSY_TC，则不应再次启动。
    // 这里简单地禁用和重新使能，以确保状态被重置。
    // 在DMA_CIRCULAR模式下，HAL_ADC_Start_DMA 已经启动了，所以这个函数可能变得多余。
    // 如果您想使用它来停止和重新启动，可以保留逻辑。
    __HAL_ADC_DISABLE(&adc_handle); // 禁用ADC
    __HAL_DMA_DISABLE(&dma_handle); // 禁用DMA

    // 在DMA_CIRCULAR模式下，NDTR会被HAL_ADC_Start_DMA设置，并自动重置。
    // 重新设置NDTR在这里可能没有必要，甚至可能导致问题，因为HAL_ADC_Start_DMA会处理它。
    // dma_handle.Instance->NDTR = times; // 考虑注释掉或删除此行

    __HAL_DMA_ENABLE(&dma_handle); // 重新使能DMA
    __HAL_ADC_ENABLE(&adc_handle); // 重新使能ADC

    // 启动ADC规则组转换（软件触发），这将启动DMA传输
    SET_BIT(adc_handle.Instance->CR2, ADC_CR2_SWSTART); // 启动规则转换通道
}


// DMA中断函数
void DMA2_Stream4_IRQHandler(void)
{
    // HAL库中断处理函数会自动处理DMA中断标志和调用回调函数
    HAL_DMA_IRQHandler(&dma_handle);
}

// DMA传输完成回调函数
// 在DMA_CIRCULAR模式下，每次整个缓冲区被填充完毕后，此回调函数都会被调用。
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc_dma_sta = 1; // 完成标志位置1
        // 在DMA_CIRCULAR模式下，DMA会自动重新开始传输，无需在此处手动重新启动。
    }
}
```

#### 2.2.5 主函数测试

```c
#include "bsp_init.h" // 假设包含了LCD初始化和显示函数，以及LED控制
#include "stdio.h"
#include "adc.h"

uint16_t adc_dma_buf[ADC_DMA_BUFFSIZE]; // DMA缓冲区
extern uint8_t adc_dma_sta; // DMA传输完成标志

int main(void)
{
  uint16_t i;
  uint32_t sum;
  float voltage_val;

  bsp_init(); 

  // 初始化ADC DMA采集。
  // 在此函数内部，DMA已经被设置为CIRCULAR模式并启动。
  adc_dma_init((uint32_t)adc_dma_buf);

  LCD_ShowString(30, 50, 200, 16, 16, "STM32F4 ADC DMA Test");
  LCD_ShowString(30, 110, 200, 16, 16, "ADC1_CH5_VAL:");
  LCD_ShowString(30, 130, 200, 16, 16, "ADC1_CH5_VOL:0.000V");

  // 由于 adc_dma_init 已经通过 HAL_ADC_Start_DMA 启动了DMA和ADC
  // 并且DMA是CIRCULAR模式，这里不需要再调用 adc_dma_enable。
  // 如果您在 adc_dma_init 中没有调用 HAL_ADC_Start_DMA，那么在这里调用一次是必需的。
  // 目前的 adc_dma_init 已经包含了 HAL_ADC_Start_DMA，所以这行可以注释掉或移除。
  // adc_dma_enable(ADC_DMA_BUFFSIZE); // 此行在此方案中不再需要

  while(1)
  {
      // 检查DMA传输是否完成一个缓冲区的数据
      if(adc_dma_sta)
      {
          sum = 0;
          // 遍历DMA缓冲区，计算平均值
          for(i = 0; i < ADC_DMA_BUFFSIZE; i++)
          {
              sum += adc_dma_buf[i];
          }
          // 计算平均ADC值
          uint16_t avg_adc_value = sum / ADC_DMA_BUFFSIZE;
          LCD_ShowxNum(134, 110, avg_adc_value, 4, 16, 0); // 显示平均ADC值

          // 计算电压值 (3.3V 参考电压，12位ADC，最大值为4095)
          voltage_val = (float)avg_adc_value * (3.3f / 4095.0f); // 12位ADC最大值为4095

          // 显示电压整数部分
          uint16_t voltage_int = (uint16_t)voltage_val;
          LCD_ShowxNum(134, 130, voltage_int, 1, 16, 0);

          // 显示电压小数部分 (取小数点后三位)
          uint16_t voltage_frac = (uint16_t)((voltage_val - voltage_int) * 1000);
          LCD_ShowxNum(150, 130, voltage_frac, 3, 16, 0x80);
          adc_dma_sta = 0; // 清除完成标志，等待下一次DMA传输完成回调
      }
      LED_TOGGLE(LED0_GPIO_Pin);
      delay_ms(100);
  }
}

```

### 2.3 多通道ADC采集（DMA读取）

### 2.4 单通道ADC过采样（16位）


