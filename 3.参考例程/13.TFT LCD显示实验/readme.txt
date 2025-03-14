实验器材:
	阿波罗STM32F429开发板
	
实验目的:
	学习彩色TFTLCD显示屏（MCU屏）的使用
	
硬件资源:
	1,DS0(连接在PB1) 
	2,串口1(波特率:115200,PA9/PA10连接在板载USB转串口芯片CH340上面)
	3,ALIENTEK 2.8/3.5/4.3/7寸TFTLCD模块(仅限MCU屏，16位8080并口驱动) 
	
实验现象:
	本实验利用阿波罗STM32开发板底板的MCU屏接口连接ALIENTEK TFTLCD模块(仅限MCU屏模块)，实现
	TFTLCD模块的显示，下载成功后，通过把LCD模块插入底板上的TFTLCD模块接口，按下复位之后，就
	可以看到LCD模块不停的显示一些信息并不断切换底色。同时该实验会显示LCD驱动器的ID，并且会在
	串口打印（按复位一次，打印一次）。
	
注意事项:
	1,4.3寸和7寸屏需要比较大电流,USB供电可能不足,请用外部电源适配器(推荐外接12V 1A电源).
	2,本例程在LCD_Init函数里面(在lcd.c),用到了printf,如果不初始化串口1,将导致液晶无法显示!! 
	3,本例程仅支持MCU屏，不支持RGB屏！！
	 

参考资料：阿波罗STM32F429开发指南-库函数版本.pdf 第十七章