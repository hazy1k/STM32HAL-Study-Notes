# 第十二章 OLED显示实验

## 1. 硬件设计

本实验使用到的硬件资源有：

- 指示灯DS0

- OLED模块

OLED 模块的电路在前面已有详细说明了，这里我们介绍 OLED 模块与探索者 STM32F4开发板的连接， 开发板底板的 OLED/CAMERA 接口（P8 接口） 可以和 ALIENTEK OLED 模块直接对插

![屏幕截图 2024 09 19 105921](https://img.picgo.net/2024/09/19/-2024-09-19-1059219b89e6e642bcc0da.png)

图中圈出来的部分就是连接 OLED 的接口，这里在硬件上， OLED 与探索者 STM32F4 开发板的 IO 口对应关系如下：

- OLED_CS 对应 DCMI_VSYNC，即： PB7; 

- OLED_RS 对应 DCMI_SCL，即： PD6; 

- OLED_WR 对应 DCMI_HREF，即： PA4; 

- OLED_RD 对应 DCMI_SDA，即： PD7; 

- OLED_RST 对应 DCMI_RESET， 即： PG15;

- OLED_D[7:0]对应 DCMI_D[7:0]，即： PE6/PE5/PB6/PC11/PC9/PC8/PC7/PC6;

![屏幕截图 2024 09 19 110604](https://img.picgo.net/2024/09/19/-2024-09-19-1106045d3374e5c384a8da.png)

## 2. 软件设计

oled.c 的代码，由于比较长，这里我们就不贴出来了， 仅介绍几个比较重要的函数。首先是 OLED_Init 函数， 该函数的结构比较简单，开始是对 IO 口的初始化，这里我们用了宏定义OLED_MODE 来决定要设置的 IO 口，其他就是一些初始化序列了，我们按照厂家提供的资料来做就可以。最后要说明一点的是，因为 OLED 是无背光的，在初始化之后，我们把显存都清空了，所以我们在屏幕上是看不到任何内容的，跟没通电一个样，不要以为这就是初始化失败，要写入数据模块才会显示的。 OLED_Init 函数代码如下：

```c
//初始化SSD1306					    
void OLED_Init(void)
{ 	 		 
    GPIO_InitTypeDef  GPIO_Initure;
	
    __HAL_RCC_GPIOA_CLK_ENABLE();   //使能GPIOA时钟
    __HAL_RCC_GPIOB_CLK_ENABLE();   //使能GPIOB时钟
    __HAL_RCC_GPIOC_CLK_ENABLE();   //使能GPIOC时钟
    __HAL_RCC_GPIOD_CLK_ENABLE();   //使能GPIOD时钟
    __HAL_RCC_GPIOE_CLK_ENABLE();   //使能GPIOE时钟
    __HAL_RCC_GPIOG_CLK_ENABLE();   //使能GPIOG时钟
    
#if OLED_MODE==1		//使用8080并口模式		
	
	//GPIO初始化设置      
    GPIO_Initure.Pin=GPIO_PIN_4;         	//PA4
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;   	//快速
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);   	//初始化
	
    //PB6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;	
	HAL_GPIO_Init(GPIOB,&GPIO_Initure);//初始化

    //PC6,7,8,9,11
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_11;	
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);//初始化	
  
    //PD6,7
	GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;	
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);//初始化	
	
    //PE5,6
	GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6;	
	HAL_GPIO_Init(GPIOE,&GPIO_Initure);//初始化	
	
	//PG15
	GPIO_Initure.Pin=GPIO_PIN_15;	
	HAL_GPIO_Init(GPIOG,&GPIO_Initure);//初始化	
	
	OLED_WR=1;
  	OLED_RD=1; 
#else					//使用4线SPI 串口模式

	//GPIO初始化设置      
    GPIO_Initure.Pin=GPIO_PIN_7;         	//PB7
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;	//推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;        	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;   	//高速
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);   	//初始化
	
    //PC6,7
    GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;	
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);//初始化

    //PD6
    GPIO_Initure.Pin=GPIO_PIN_6;	
	HAL_GPIO_Init(GPIOD,&GPIO_Initure);//初始化	
	
	//PG15
    GPIO_Initure.Pin=GPIO_PIN_15;	
	HAL_GPIO_Init(GPIOG,&GPIO_Initure);//初始化
	
	OLED_SDIN=1;
	OLED_SCLK=1;
#endif
	OLED_CS=1;
	OLED_RS=1;	 
	
	OLED_RST=0;
	delay_ms(100);
	OLED_RST=1; 
					  
	OLED_WR_Byte(0xAE,OLED_CMD); //关闭显示
	OLED_WR_Byte(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
	OLED_WR_Byte(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
	OLED_WR_Byte(0xA8,OLED_CMD); //设置驱动路数
	OLED_WR_Byte(0X3F,OLED_CMD); //默认0X3F(1/64) 
	OLED_WR_Byte(0xD3,OLED_CMD); //设置显示偏移
	OLED_WR_Byte(0X00,OLED_CMD); //默认为0
	OLED_WR_Byte(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.										    
	OLED_WR_Byte(0x8D,OLED_CMD); //电荷泵设置
	OLED_WR_Byte(0x14,OLED_CMD); //bit2，开启/关闭
	OLED_WR_Byte(0x20,OLED_CMD); //设置内存地址模式
	OLED_WR_Byte(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
	OLED_WR_Byte(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
	OLED_WR_Byte(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
	OLED_WR_Byte(0xDA,OLED_CMD); //设置COM硬件引脚配置
	OLED_WR_Byte(0x12,OLED_CMD); //[5:4]配置	 
	OLED_WR_Byte(0x81,OLED_CMD); //对比度设置
	OLED_WR_Byte(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
	OLED_WR_Byte(0xD9,OLED_CMD); //设置预充电周期
	OLED_WR_Byte(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
	OLED_WR_Byte(0xDB,OLED_CMD); //设置VCOMH 电压倍率
	OLED_WR_Byte(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;
	OLED_WR_Byte(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
	OLED_WR_Byte(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
	OLED_WR_Byte(0xAF,OLED_CMD); //开启显示	 
	OLED_Clear();
}  
```

接着， 要介绍的是OLED_Refresh_Gram 函数。我们在 STM32F4内部定义了一个块GRAM： u8 OLED_GRAM[128][8];此部分 GRAM 对应 OLED 模块上的 GRAM。在操作的时候，我们只要修改 STM32F4 内部的 GRAM 就可以了，然后通过 OLED_Refresh_Gram 函数把 GRAM 一次刷新到 OLED 的 GRAM 上。该函数代码如下

```c
// 更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); 
	}   
}
```

OLED_Refresh_Gram 函数先设置页地址，然后写入列地址（也就是纵坐标），然后从 0 开始写入 128 个字节，写满该页，最后循环把 8 页的内容都写入，就实现了整个从 STM32F4 显存到 OLED 显存的拷贝。

OLED_Refresh_Gram 函数还用到了一个外部函数， 也就是我们接着要介绍的函数： OLED_WR_Byte，该函数直接和硬件相关，函数代码如下：

```c
//通过拼凑的方法向OLED输出一个8位数据
//data:要输出的数据
void OLED_Data_Out(u8 data)
{
	u16 dat=data&0X0F;
	GPIOC->ODR&=~(0XF<<6);		//清空6~9
	GPIOC->ODR|=dat<<6;			//D[3:0]-->PC[9:6]
    
    GPIOC->ODR&=~(0X1<<11);		//清空11
    GPIOC->ODR|=((data>>4)&0x01)<<11;
    
    GPIOB->ODR&=~(0X1<<6);		//清空6
    GPIOB->ODR|=((data>>5)&0x01)<<6;
    
    GPIOE->ODR&=~(0X3<<5);		//清空5,6
    GPIOE->ODR|=((data>>6)&0x01)<<5;
    GPIOE->ODR|=((data>>7)&0x01)<<6;
} 
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	OLED_Data_Out(dat);	
 	OLED_RS=cmd;
	OLED_CS=0;	
	OLED_WR=0;	  
	OLED_WR=1;   
	OLED_CS=1;   
	OLED_RS=1;   
} 	    	    
#else
//向SSD1306写入一个字节。
//dat:要写入的数据/命令
//cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{	
	u8 i;			  
	OLED_RS=cmd; //写命令 
	OLED_CS=0;		  
	for(i=0;i<8;i++)
	{			  
		OLED_SCLK=0;
		if(dat&0x80)OLED_SDIN=1;
		else OLED_SDIN=0;
		OLED_SCLK=1;
		dat<<=1;   
	}				 
	OLED_CS=1;		  
	OLED_RS=1;   	  
} 
#endif
```

首先，我们看 OLED_Data_Out 函数，这就是我们前面说的，因为 OLED 的 D0~D7 不是接的连续 IO，所以必须将数据，拆分到各个 IO，以实现一次完整的数据传输，该函数就是根据我们 OLED_D[7:0]具体连接的 IO，对数据进行拆分，然后输出给对应位的各个 IO，实现并口数据输出。 这种方式会降低并口速度，但是我们 OLED 模块，是单色的，数据量不是很大，所以这种方式也不会造成视觉上的影响，大家可以放心使用，但是如果是 TFTLCD，就不推荐了。

然后，看 OLED_WR_Byte 函数， 这里有 2 个一样的函数，通过宏定义 OLED_MODE 来决定使用哪一个。如果 OLED_MODE=1，就定义为并口模式，选择第一个函数，而如果为 0，则为 4 线串口模式，选择第二个函数。这两个函数输入参数均为 2 个： dat 和 cmd， dat 为要写入的数据， cmd 则表明该数据是命令还是数据。这两个函数的时序操作就是根据上面我们对 8080接口以及 4 线 SPI 接口的时序来编写的。


