#include "key.h"
#include "delay.h"

#define KEY11 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)
#define KEY22 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)
#define KEY33 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define KEY41 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define KEY42 GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define KEY51 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3)
#define KEY52 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK Mini STM32开发板
//按键输入 驱动代码		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2014/3/06
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									   
//////////////////////////////////////////////////////////////////////////////////	 
 	    
//按键初始化函数 
//PA15和PC5 设置成输入
void KEY_Init(void)
{
	
		GPIO_InitTypeDef GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTC时钟

		GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//关闭jtag，使能SWD，可以用SWD模式调试
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;//PC1
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC1
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;//PA2
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA2
	 
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;//PC6
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PA3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA3
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PA5
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA5
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;//PC3
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC3
		
		GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//PC5
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
		GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOC5
} 
//按键处理函数
//返回按键值
//mode:0,不支持连续按;1,支持连续按;
//返回值：
//0，没有任何按键按下
//KEY0_PRES，KEY0按下
//KEY1_PRES，KEY1按下
//WKUP_PRES，WK_UP按下 
//注意此函数有响应优先级,KEY0>KEY1>WK_UP!!

	u8 KEY_Scan(u8 mode)
	{	 
		static u8 key_up=1;//按键按松开标志
		if(mode)key_up=1;  //支持连按		  
		if(key_up&&(KEY11==0||KEY22==0||KEY33==0))
		{
			delay_ms(10);//去抖动 
			key_up=0;
			if(KEY11==0)return KEY0_PRES;
			else if(KEY22==0)return KEY1_PRES;
			else if(KEY33==0)return WKUP_PRES; 
		}else if(KEY11==1&&KEY22==1&&KEY33==1)key_up=1; 	     
		return 0;// 无按键按下
	}
