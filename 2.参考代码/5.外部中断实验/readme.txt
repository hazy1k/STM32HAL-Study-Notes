实验目的:
	学习外部中断的使用	
硬件资源:
	1；DS0(连接在PB1),DS1(连接在PB0) 
	2；按键KEY0(PH3)/KEY1(PH2)/KEY2(PC13)/KEY_UP(PA0)
实验现象:
	本实验,利用外部中断，实现与按键实验类似的效果，即：通过ALIENTEK阿波罗STM32开发板上
	载有的4个按钮（KEY_UP、KEY0、KEY1和KEY2），来控制板上的2个LED（DS0和DS1），其中
	KEY_UP控制DS0，DS1互斥点亮；KEY2控制DS0，按一次亮，再按一次灭；KEY1控制DS1，效
	果同KEY2；KEY0则同时控制DS0和DS1，按一次，他们的状态就翻转一次。