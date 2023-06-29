#ifndef _SIGN_H
#define _SIGN_H
#include "stm32f10x.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "sys.h"
#include "stm32f10x_dac.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
        
/******DAC寄存器地址声明*******/  
#define DAC_DHR12R1    (u32)&(DAC->DHR12R1)   //DAC通道1输出地址
#define DAC_DHR12R2    (u32)&(DAC->DHR12R2)   //DAC通道2输出地址

/********正弦波输出表***********/
void Sine_Wave_Data(u16 cycle ,u16 *D_Sine,float Um);
void Square_Wave_Data(u16 cycle ,u16 *D_Square,float Um);
/****************初始化引脚******************/
void SineWave_GPIO_Config(void);

/******************DAC初始化ˉ*************************/
void SineWave_DAC_Config( void);

/*********定时器初始化************/
void Wave_TIM_Config(u32 Wave1_Fre);

/*********DMA配置***********/
void Sine_Wave_DMA_Config(void);
void Square_Wave_DMA_Config(void);
void Triangular_Wave_DMA_Config(void);
    
/**********正弦波初始化*****/
void Sine_Wave_Init(u16 freq,float Um);
/**********三角波初始化*****/
void Triangular_Wave_Init(u16 Freq,float Um);
/**********方波初始化*****/
void Square_Wave_Init(u16 freq,float Um);

#endif
