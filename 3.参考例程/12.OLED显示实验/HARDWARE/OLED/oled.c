#include "oled.h"
#include "stdlib.h"
#include "delay.h"
#include "oledfont.h"
	   
u8 OLED_GRAM[128][8];	 

// 更新显存到LCD		 
void OLED_Refresh_Gram(void)
{
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
		OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)	
			OLED_WR_Byte(OLED_GRAM[n][i],OLED_DATA); // 写入数据到GRAM 
	}   
}
#if OLED_MODE == 1	//8080并口
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
// 向SSD1306写入一个字节。
// dat:要写入的数据/命令
// cmd:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 cmd)
{
	OLED_Data_Out(dat); // 输出数据	
 	OLED_RS = cmd; // 写命令 
	OLED_CS = 0;   // 拉低片选	
	OLED_WR = 0;   // 拉低写数据线
	OLED_WR = 1;   // 拉高写数据线，准备发送数据
	OLED_CS = 1;   // 拉高片选，结束数据传输
	OLED_RS = 1;   // 写命令 
} // 下面的代码是SPI方式的驱动方式,可以参考	    	    
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
	  	  
//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	u8 i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}
//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空				   
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 pos,bx,temp=0;
	if(x>127||y>63)return;//超出范围了.
	pos=7-y/8;
	bx=y%8;
	temp=1<<(7-bx);
	if(t)OLED_GRAM[x][pos]|=temp;
	else OLED_GRAM[x][pos]&=~temp;	    
}
//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot)  
{  
	u8 x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode)
{      			    
	u8 temp,t,t1;
	u8 y0=y;
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}
//m^n函数
u32 mypow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(u8 x,u8 y,const u8 *p,u8 size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}

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
    
#if OLED_MODE == 1		//使用8080并口模式		
	
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
	
	OLED_WR=1; // 8080并口模式下,WR为高电平有效，所以先置1
  	OLED_RD=1; // 8080并口模式下,RD为高电平有效，所以先置1
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