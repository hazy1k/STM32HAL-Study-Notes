实验目的:
	学习定时器的输入捕获功能	
硬件资源:
	1：DS0(连接在PF9)  
	2：KEY_UP按键(PA0)
	3：定时器5(TIM5),TIM5的通道1(TIM5_CH1,连接在PA0上面),使用定时器的输入捕获功能来捕捉PA0上面的高电平脉宽. 
实验现象:
	本实验利用TIM5_CH1来做输入捕获，我们将捕获PA0上的高电平脉宽，并将脉宽时间通过串口打印出来，大家可以通过按WK_UP按键，模拟输入高电平。
	同时，本实验将保留上一个实验的PWM输出，DS0还是会由暗-->亮的循环变化，大家可以通过杜邦线连接PA0和PB1，来观测PWM输出的高电平时间。