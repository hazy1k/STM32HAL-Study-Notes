# 第十五章 TFTLCD显示

## 1. TFTLCD简介

TFT-LCD 即薄膜晶体管液晶显示器。其英文全称为： Thin Film Transistor-Liquid Crystal Display。 TFT-LCD 与无源 TN-LCD、 STN-LCD 的简单矩阵不同，它在液晶显示屏的每一个象素上都设置有一个薄膜晶体管（TFT），可有效地克服非选通时的串扰，使显示液晶屏的静态特性与扫描线数无关，因此大大提高了图像质量。 TFT-LCD 也被叫做真彩液晶显示器。上一章介绍了 OLED 模块，本章，我们给大家介绍 ALIENTEK TFTLCD 模块、

实物图如下：

![屏幕截图 2024 09 20 103223](https://img.picgo.net/2024/09/20/-2024-09-20-103223fc8e00103b646258.png)

模块原理图如下：

![屏幕截图 2024 09 20 103434](https://img.picgo.net/2024/09/20/-2024-09-20-1034349e086ad126eebf26.png)

接口定义图：

![屏幕截图 2024 09 20 103512](https://img.picgo.net/2024/09/20/-2024-09-20-10351255c76f1adad54177.png)

ALIENTEK TFTLCD 模块采用 16 位的并方式与外部连接， 之所以不采用 8 位的方式，是因为彩屏的数据量比较大，尤其在显示图片的时候，如果用 8 位数据线，就会比 16 位方式慢一倍以上，我们当然希望速度越快越好，所以我们选择 16 位的接口。图18.1.1.3 还列出了触摸屏芯片的接口，关于触摸屏本章我们不多介绍，后面的章节会有详细的介绍。 该模块的 80 并口有如下一些信号线：

- CS： TFTLCD 片选信号。

- WR：向 TFTLCD 写入数据。

- RD：从 TFTLCD 读取数据。

- D[15： 0]： 16 位双向数据线。

- RST：硬复位 TFTLCD。

- RS：命令/数据标志（0，读写命令； 1，读写数据）。

## 2. 一般TFTLCD模块使用流程

![屏幕截图 2024 09 20 104053](https://img.picgo.net/2024/09/20/-2024-09-20-1040539447a75ca0b5125e.png)

任何 LCD，使用流程都可以简单的用以上流程图表示。其中硬复位和初始化序列，只需要执行一次即可。而画点流程就是：设置坐标→写 GRAM 指令→写入颜色数据，然后在 LCD 上面，我们就可以看到对应的点显示我们写入的颜色了。读点流程为：设置坐标→读 GRAM 指令→读取颜色数据，这样就可以获取到对应点的颜色数据了。

以上只是最简单的操作，也是最常用的操作，有了这些操作，一般就可以正常使用 TFTLCD了。接下来我们将该模块用来来显示字符和数字， 通过以上介绍，我们可以得出 TFTLCD 显示需要的相关设置步骤如下：

### 2.1 设置STM32F4与TFTLCD模块相连接的IO

这一步，先将我们与 TFTLCD 模块相连的 IO 口进行初始化，以便驱动 LCD。这里我们用到的是 FSMC

### 2.2 初始化TFTLCD模块

初始化序列，就是向 LCD 控制器写入一系列的设置值（比如伽马校准），这些初始化序列一般 LCD 供应商会提供给客户，我们直接使用这些序列即可，不需要深入研究。在初始化之后， LCD 才可以正常使用。

### 2.3 通过函数将字符和数字显示到TFTLCD模块上

设置坐标→写 GRAM 指令→写 GRAM 来实现，但是这个步骤，只是一个点的处理，我们要显示字符/数字，就必须要多次使用这个步骤，从而达到显示字符/数字的目的，所以需要设计一个函数来实现数字/字符的显示，之后调用该函数，就可以实现数字/字符的显示了。

## 3. FSMC简介

STM32F407 或 STM32F417 系列芯片都带有 FSMC 接口， ALIENTEK 探索者 STM32F4 开发板的主芯片为 STM32F407ZGT6，是带有 FSMC 接口的。

FSMC，即灵活的静态存储控制器，能够与同步或异步存储器和 16 位 PC 存储器卡连接， STM32F4 的 FSMC 接口支持包括 SRAM、 NAND FLASH、 NOR FLASH 和 PSRAM 等存储器。FSMC 的框图如图：

![屏幕截图 2024 09 20 104703](https://img.picgo.net/2024/09/20/-2024-09-20-104703e6bf60a5b009d723.png)

从上图我们可以看出， STM32F4 的 FSMC 将外部设备分为 2 类： NOR/PSRAM 设备、NAND/PC 卡设备。他们共用地址数据总线等信号，他们具有不同的 CS 以区分不同的设备，比如本章我们用到的 TFTLCD 就是用的 FSMC_NE4 做片选，其实就是将 TFTLCD 当成 SRAM 来控制。

这里我们介绍下为什么可以把 TFTLCD 当成 SRAM 设备用：首先我们了解下外部 SRAM的连接，外部 SRAM 的控制一般有：地址线（如 A0~A18）、数据线（如 D0~D15）、写信号（WE）、读信号（OE）、片选信号（CS），如果 SRAM 支持字节控制，那么还有 UB/LB 信号。而 TFTLCD的信号我们在 18.1.1 节有介绍，包括： RS、 D0~D15、 WR、 RD、 CS、 RST 和 BL 等，其中真正在操作 LCD 的时候需要用到的就只有： RS、 D0~D15、 WR、 RD 和 CS。其操作时序和 SRAM的控制完全类似，唯一不同就是 TFTLCD 有 RS 信号，但是没有地址信号。

TFTLCD 通过 RS 信号来决定传送的数据是数据还是命令，本质上可以理解为一个地址信号，比如我们把 RS 接在 A0 上面，那么当 FSMC 控制器写地址 0 的时候，会使得 A0 变为 0，对 TFTLCD 来说，就是写命令。而 FSMC 写地址 1 的时候， A0 将会变为 1，对 TFTLCD 来说，就是写数据了。这样，就把数据和命令区分开了，他们其实就是对应 SRAM 操作的两个连续地址。当然 RS 也可以接在其他地址线上，探索者 STM32F4 开发板是把 RS 连接在 A6 上面的。

STM32F4 的 FSMC 支持 8/16/32 位数据宽度，我们这里用到的 LCD 是 16 位宽度的，所以在设置的时候，选择 16 位宽就 OK 了。我们再来看看 FSMC 的外部设备地址映像， STM32F4的 FSMC 将外部存储器划分为固定大小为 256M 字节的四个存储块，如图所示：

![屏幕截图 2024 09 20 104754](https://img.picgo.net/2024/09/20/-2024-09-20-104754d51fb9bfecb65195.png)

## 4. FSMC初始化函数

在 HAL 库中提供了 FSMC 初始化函数为

```c
HAL_SRAM_Init ();
```

下面我们看看函数定义：

```c
HAL_StatusTypeDef HAL_SRAM_Init(SRAM_HandleTypeDef *hsram,
FMC_NORSRAM_TimingTypeDef *Timing, FMC_NORSRAM_TimingTypeDef *ExtTiming)
```

这个函数有三个入口参数，SRAM_HandleTypeDef 类 型 指 针 变 量 、FMC_NORSRAM_TimingTypeDef 类型指针变量、 FMC_NORSRAM_TimingTypeDef 类型指针变量。

FMC_NORSRAM_TimingTypeDef 指针类型的成员变量。前面我们讲到， FSMC 有读时序和写时序之分，所以这里就是用来设置读时序和写时序的参数了， 也就是说，这两个参数是用来配置寄存器 FSMC_BTRx 和 FSMC_BWTRx，后面我们会讲解到。下面我们主要来看看模式A 下的相关配置参数：

- 参数 NSBank 用来设置使用到的存储块标号和区号，前面讲过，我们是使用的存储块 1 区号 4，所以选择值为 FSMC_NORSRAM_BANK4。

- 参数 MemoryType 用来设置存储器类型，我们这里是 SRAM，所以选择值为FSMC_MEMORY_TYPE_SRAM。

- 参数 MemoryDataWidth 用来设置数据宽度，可选 8 位还是 16 位，这里我们是 16 位数据宽度，所以选择值为 FSMC_NORSRAM_MEM_BUS_WIDTH_16。

- 参数 WriteOperation 用来设置写使能，毫无疑问，我们前面讲解过我们要向 TFT 写数据，所以要写使能， 这里我们选择 FSMC_WRITE_OPERATION_ENABLE。

- 参数 ExtendedMode 是设置扩展模式使能位，也就是是否允许读写不同的时序，这里我们采取的读写不同时序，所以设置值为 FSMC_EXTENDED_MODE_ENABLE。

上面的这些参数是与模式 A 相关的，下面我们也来稍微了解一下其他几个参数的意义吧：参数 DataAddressMux 用来设置地址/数据复用使能，若设置为使能，那么地址的低 16 位和数据将共用数据总线，仅对 NOR 和 PSRAM 有效，所以我们设置为默认值不复用，值FSMC_DATA_ADDRESS_MUX_DISABLE。

其他参数在成组模式同步模式才需要设置，大家可以参考中文参考手册了解相关参数的意思。

接下来我们看看设置读写时序参数的两个变量 FSMC_ReadWriteTim 和 FSMC_WriteTim，他们都是 FSMC_NORSRAM_TimingTypeDef 结构体指针类型，这两个参数在初始化的时候分别用来初始化片选控制寄存器 FSMC_BTRx 和写操作时序控制寄存器 FSMC_BWTRx。 下面我们看看 FSMC_NORSRAMTimingInitTypeDef 类型的定义：

```c
typedef struct
{
    uint32_t AddressSetupTime;
    uint32_t AddressHoldTime;
    uint32_t DataSetupTime;
    uint32_t BusTurnAroundDuration;
    uint32_t CLKDivision;
    uint32_t DataLatency;
    uint32_t AccessMode;
}FSMC_NORSRAM_TimingTypeDef
```

这个结构体有 7 个参数用来设置 FSMC 读写时序。其实这些参数的意思我们前面在讲解 FSMC的时序的时候有提到，主要是设计地址建立保持时间，数据建立时间等等配置，对于我们的实验中，读写时序不一样，读写速度要求不一样，所以对于参数 FSMC_DataSetupTime 设置了不同的值，大家可以对照理解一下。
