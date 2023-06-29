#include "sign.h"
u16 D_Sine[256]; 
u16 D_Triangular[256];
u16 D_Square[256];

/********正弦波输出表***********/
//cycle	:波形表的位数	(0~256)
void Sine_Wave_Data(u16 cycle ,u16 *D_Sine,float Um)
{  
    u16 i;
    for( i=0;i<cycle;i++)
    D_Sine[i]=(u16)(Um*(sin(i*2*3.1415926/cycle)+1)*4095/2);
}

/********三角波输出表***********/
void Triangular_Wave_Data( u16 cycle ,u16 *D_Triangular,float Um)
{
    u16 i;
    for( i=0;i<cycle/2;i++)
	{
		D_Triangular[i]= (u16)(Um*4095*2*i/cycle);
	}
	for( i=cycle/2;i<cycle;i++)
	{
		D_Triangular[i]= (u16)(Um*2*4095*(cycle-i)/cycle);
	}
        
}

/********方波输出表***********/
void Square_Wave_Data(u16 cycle ,u16 *D_Square,float Um)
{
	u16 i;
	for(i=0;i<cycle/2;i++)
	{
		D_Square[i]=(u16)(0);
	}
	for( i=cycle/2;i<cycle;i++)
	{
		D_Square[i]=(u16)(Um*4095);
	}
}


/****************初始化引脚******************/
void Wave_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //开时钟
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;       //推挽输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;   //输出速率 
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4 ; //选择引脚
    GPIO_SetBits(GPIOA,GPIO_Pin_4)  ;   //拉高输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);      //初始化
}

/******************DAC初始化ˉ*************************/
void Wave_DAC_Config( void)
{
    DAC_InitTypeDef            DAC_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//开DAC时钟
     
  /**************DAC结构初始化*******************/
    DAC_StructInit(&DAC_InitStructure);    
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;//不产生波形
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable; //不使能输出缓存
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//DAC触发为定时器2触发
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);//初始化
    DAC_Cmd(DAC_Channel_1, ENABLE);    //使能DAC的通道1
    DAC_DMACmd(DAC_Channel_1, ENABLE); //使能DAC通道1的DMA  
}

/*********定时器初始化************/
void Wave_TIM_Config(u32 f1)
{
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//开时钟
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Prescaler = 0x0;     //不预分频
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; //不分频<br>　　TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数
    TIM_TimeBaseStructure.TIM_Period = f1;//设置输出频率
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);//设置TIME输出触发为更新模式
}

/*********Sine DMA配置***********/
void Sine_Wave_DMA_Config(void)
{                  
    DMA_InitTypeDef            DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//开启DMA2时钟
     
    DMA_StructInit( &DMA_InitStructure);        //DMA结构体初始化
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//从寄存器读数据
    DMA_InitStructure.DMA_BufferSize = 256;//寄存器大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//优先级非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//关闭内存到内存模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环发送模式
	
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;//外设地址为DAC通道1的地址
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)D_Sine;//波形数据表内存地址
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化
    DMA_Cmd(DMA2_Channel3, ENABLE); //使能DMA通道3  　　  
		
}

/*********Triangular DMA配置***********/
void Triangular_Wave_DMA_Config(void)
{                  
    DMA_InitTypeDef            DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//开启DMA2时钟
     
    DMA_StructInit( &DMA_InitStructure);        //DMA结构体初始化
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//从寄存器读数据
    DMA_InitStructure.DMA_BufferSize = 256;//寄存器大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//优先级非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//关闭内存到内存模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环发送模式
	
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;//外设地址为DAC通道1的地址
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)D_Triangular;//波形数据表内存地址
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化
    DMA_Cmd(DMA2_Channel3, ENABLE); //使能DMA通道3  　　  
		
}

/*********Square DMA配置***********/
void Square_Wave_DMA_Config(void)
{                  
    DMA_InitTypeDef            DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);//开启DMA2时钟
     
    DMA_StructInit( &DMA_InitStructure);        //DMA结构体初始化
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//从寄存器读数据
    DMA_InitStructure.DMA_BufferSize =256;//寄存器大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不递增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//宽度为半字
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;//优先级非常高
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//关闭内存到内存模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;//循环发送模式
	
    DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;//外设地址为DAC通道1的地址
    DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)D_Square;//波形数据表内存地址
    DMA_Init(DMA2_Channel3, &DMA_InitStructure);//初始化
    DMA_Cmd(DMA2_Channel3, ENABLE); //使能DMA通道3  　　  
		
}

/**********正弦波初始化**********************/
//Freq:	频率值（0~17 000)Hz
//Um			 :	电压峰值（0.0~1.5）V
/*******************************************/

void Sine_Wave_Init(u16 Freq,float Um)
{
	u32 f1;
	f1=(u32)(72000000/sizeof(D_Sine)*2/Freq);//计算频率
	Sine_Wave_Data(256,D_Sine,Um);     //生成输出正弦波的波形表
	Wave_GPIO_Config();             //初始化io
	Wave_TIM_Config(f1);            //初始化定时器
	Wave_DAC_Config();              //配置DAC
	Sine_Wave_DMA_Config();              //配置DMA
	TIM_Cmd(TIM2, ENABLE);             //开启定时器
}

/**********三角波初始化**********************/
void Triangular_Wave_Init(u16 Freq,float Um)
{
	u32 f1;
	f1=(u32)(72000000/sizeof(D_Triangular)*2/Freq);//计算频率
	Triangular_Wave_Data(256,D_Triangular,Um);     //生成输出正弦波的波形表
	Wave_GPIO_Config();             //初始化io
	Wave_TIM_Config(f1);            //初始化定时器
	Wave_DAC_Config();              //配置DAC
	Triangular_Wave_DMA_Config();              //配置DMA
	TIM_Cmd(TIM2, ENABLE);             //开启定时器
}

/**********方波初始化**********************/
void Square_Wave_Init(u16 freq,float Um)
{
	u32 f1;
	f1=(u32)(72000000/sizeof(D_Square)*2/freq);//计算频率
	Square_Wave_Data(256,D_Square,Um);     //生成输出正弦波的波形表
	Wave_GPIO_Config();             //初始化io
	Wave_TIM_Config(f1);            //初始化定时器
	Wave_DAC_Config();              //配置DAC
	Square_Wave_DMA_Config();          //配置DMA
	TIM_Cmd(TIM2, ENABLE);             //开启定时器
}

