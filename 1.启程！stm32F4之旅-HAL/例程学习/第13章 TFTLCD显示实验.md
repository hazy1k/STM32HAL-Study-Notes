# 第十三章 TFTLCD显示实验

## 1. 硬件设计

本实验用到的硬件资源有：

- 指示灯DS0

- TFT LCD模块

![屏幕截图 2024 09 20 105553](https://img.picgo.net/2024/09/20/-2024-09-20-105553de0731c46735b375.png)

![屏幕截图 2024 09 20 105641](https://img.picgo.net/2024/09/20/-2024-09-20-10564168e56c0044c82e01.png)

## 2. 软件设计

在此我们只分析主函数，驱动代码屏幕厂商都会提供，我们只需要会有功能函数就行

- 主函数

```c
int main(void)
{
	u8 x=0;
	u8 lcd_id[12]; // 存放LCD ID字符串
	
    HAL_Init();                 // 初始化HAL库    
    Stm32_Clock_Init(336,8,2,7);// 设置时钟,168Mhz
	delay_init(168);            // 初始化延时函数
	uart_init(115200);          // 初始化USART
	LED_Init();			        // 初始化LED	
 	LCD_Init();           	    // 初始化LCD FSMC接口
	POINT_COLOR=RED;     	    // 画笔颜色：红色
	sprintf((char*)lcd_id, "LCD ID:%04X", lcddev.id); // 将LCD ID打印到lcd_id数组。
  	while(1) 
	{		 
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break; 
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
		}
		POINT_COLOR = RED;	  
		LCD_ShowString(30,40,210,24,24,"Explorer STM32F4");	
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"hello world!");
 		LCD_ShowString(30,110,200,16,16,lcd_id); // 显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2024/9/20");	      					 
		x++;
		if(x==12)x=0;
		LED0=!LED0;	 
		delay_ms(1000);	
	} 
}
```

该部分代码将显示一些固定的字符，字体大小包括 24 * 12、 16 * 8 和 12*6 等三种，同时显示LCD 驱动 IC 的型号，然后不停的切换背景颜色，每 1s 切换一次。而 LED0 也会不停的闪烁，指示程序已经在运行了。 其中我们用到一个 sprintf 的函数，该函数用法同 printf，只是 sprintf把打印内容输出到指定的内存区间上


