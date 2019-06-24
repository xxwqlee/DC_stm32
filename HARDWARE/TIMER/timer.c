#include "timer.h"
#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//TIM1-5（CH1、CH2）读取编码器角度
//TIM1：（AF1）PA8、9
//TIM2：（AF1）PA15、PB3
//TIM3：（AF2）PB4、5
//TIM4：（AF2）PB6\7
//TIM5：（AF2）PH10、11

//TIM8（CH1、2、3、4/PC6\7\8\9）TIM9（CH1/PE5）成PWM控制信号（0-3.3V）对应占空比（0-100%）

//
////////////////////////////////////////////////////////////////////////////////// 	 
TIM_HandleTypeDef TIM1_Handler;              //定时器1句柄 
TIM_Encoder_InitTypeDef TIM1_Encoder_Config; //1编码器接口

TIM_HandleTypeDef TIM2_Handler;              //定时器2句柄 
TIM_Encoder_InitTypeDef TIM2_Encoder_Config; //2编码器接口

TIM_HandleTypeDef TIM3_Handler;              //定时器3句柄 
TIM_Encoder_InitTypeDef TIM3_Encoder_Config; //3编码器接口

TIM_HandleTypeDef TIM4_Handler;              //定时器4句柄 
TIM_Encoder_InitTypeDef TIM4_Encoder_Config; //4编码器接口

TIM_HandleTypeDef TIM5_Handler;              //定时器5句柄 
TIM_Encoder_InitTypeDef TIM5_Encoder_Config; //5编码器接口



TIM_HandleTypeDef TIM8_Handler;         //定时器8句柄 
TIM_OC_InitTypeDef TIM8_CH1Handler;     //定时器8通道1句柄
TIM_OC_InitTypeDef TIM8_CH2Handler;     //定时器8通道2句柄
TIM_OC_InitTypeDef TIM8_CH3Handler;     //定时器8通道3句柄
TIM_OC_InitTypeDef TIM8_CH4Handler;     //定时器8通道4句柄

TIM_HandleTypeDef TIM9_Handler;         //定时器9句柄 
TIM_OC_InitTypeDef TIM9_CH1Handler;     //定时器9通道1句柄

TIM_HandleTypeDef TIM10_Handler;         //定时器10句柄 
TIM_OC_InitTypeDef TIM10_CH1Handler;     //定时器10通道1句柄


//TIM1 编码器模式初始化
void TIM1_Encoder_Init(u16 arr,u16 psc) 
{	 
		TIM1_Handler.Instance=TIM1;                               //定时器1
    TIM1_Handler.Init.Prescaler=psc;                          //定时器分频
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;         //向上计数模式
    TIM1_Handler.Init.Period=arr;                             //自动重装载值
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TIM1_Encoder_Config.EncoderMode=TIM_ENCODERMODE_TI12;     //TI1和TI2均计数
		TIM1_Encoder_Config.IC1Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM1_Encoder_Config.IC1Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM1_Encoder_Config.IC1Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM1_Encoder_Config.IC1Filter=0;                          //配置输入滤波器，不滤波
		TIM1_Encoder_Config.IC2Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM1_Encoder_Config.IC2Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM1_Encoder_Config.IC2Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM1_Encoder_Config.IC2Filter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_Encoder_Init(&TIM1_Handler,&TIM1_Encoder_Config); //初始化TIM1编码器接口	
		HAL_TIM_Encoder_Start(&TIM1_Handler,TIM_CHANNEL_ALL);     //打开TIM1的通道
}
//TIM2 编码器模式初始化
void TIM2_Encoder_Init(u32 arr,u16 psc) 
{	 
		TIM2_Handler.Instance=TIM2;                               //定时器2
    TIM2_Handler.Init.Prescaler=psc;                          //定时器分频
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;         //向上计数模式
    TIM2_Handler.Init.Period=arr;                             //自动重装载值
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TIM2_Encoder_Config.EncoderMode=TIM_ENCODERMODE_TI12;     //TI1和TI2均计数
		TIM2_Encoder_Config.IC1Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM2_Encoder_Config.IC1Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM2_Encoder_Config.IC1Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM2_Encoder_Config.IC1Filter=0;                          //配置输入滤波器，不滤波
		TIM2_Encoder_Config.IC2Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM2_Encoder_Config.IC2Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM2_Encoder_Config.IC2Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM2_Encoder_Config.IC2Filter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_Encoder_Init(&TIM2_Handler,&TIM2_Encoder_Config); //初始化TIM2编码器接口	
		HAL_TIM_Encoder_Start(&TIM2_Handler,TIM_CHANNEL_ALL);     //打开TIM2的通道
}
//TIM3 编码器模式初始化
void TIM3_Encoder_Init(u16 arr,u16 psc) 
{	 
		TIM3_Handler.Instance=TIM3;                               //定时器3
    TIM3_Handler.Init.Prescaler=psc;                          //定时器分频
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;         //向上计数模式
    TIM3_Handler.Init.Period=arr;                             //自动重装载值
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TIM3_Encoder_Config.EncoderMode=TIM_ENCODERMODE_TI12;     //TI1和TI2均计数
		TIM3_Encoder_Config.IC1Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM3_Encoder_Config.IC1Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM3_Encoder_Config.IC1Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM3_Encoder_Config.IC1Filter=0;                          //配置输入滤波器，不滤波
		TIM3_Encoder_Config.IC2Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM3_Encoder_Config.IC2Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM3_Encoder_Config.IC2Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM3_Encoder_Config.IC2Filter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_Encoder_Init(&TIM3_Handler,&TIM3_Encoder_Config); //初始化TIM2编码器接口	
		HAL_TIM_Encoder_Start(&TIM3_Handler,TIM_CHANNEL_ALL);     //打开TIM2的通道
}
//TIM4 编码器模式初始化
void TIM4_Encoder_Init(u16 arr,u16 psc) 
{	 
		TIM4_Handler.Instance=TIM4;                               //定时器4
    TIM4_Handler.Init.Prescaler=psc;                          //定时器分频
    TIM4_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;         //向上计数模式
    TIM4_Handler.Init.Period=arr;                             //自动重装载值
    TIM4_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TIM4_Encoder_Config.EncoderMode=TIM_ENCODERMODE_TI12;     //TI1和TI2均计数
		TIM4_Encoder_Config.IC1Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM4_Encoder_Config.IC1Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM4_Encoder_Config.IC1Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM4_Encoder_Config.IC1Filter=0;                          //配置输入滤波器，不滤波
		TIM4_Encoder_Config.IC2Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM4_Encoder_Config.IC2Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM4_Encoder_Config.IC2Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM4_Encoder_Config.IC2Filter=0;                          //配置输入滤波器，不滤波
    HAL_TIM_Encoder_Init(&TIM4_Handler,&TIM4_Encoder_Config); //初始化TIM2编码器接口	
		HAL_TIM_Encoder_Start(&TIM4_Handler,TIM_CHANNEL_ALL);     //打开TIM2的通道
}

//TIM5 编码器模式初始化
void TIM5_Encoder_Init(u32 arr,u16 psc) 
{	 
		TIM5_Handler.Instance=TIM5;                               //定时器5
    TIM5_Handler.Init.Prescaler=psc;                          //定时器分频
    TIM5_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;         //向上计数模式
    TIM5_Handler.Init.Period=arr;                             //自动重装载值
    TIM5_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
		TIM5_Encoder_Config.EncoderMode=TIM_ENCODERMODE_TI12;     //TI1和TI2均计数
		TIM5_Encoder_Config.IC1Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM5_Encoder_Config.IC1Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI1上
    TIM5_Encoder_Config.IC1Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM5_Encoder_Config.IC1Filter=0;                          //配置输入滤波器，不滤波
		TIM5_Encoder_Config.IC2Polarity=TIM_ICPOLARITY_FALLING;    //双边沿捕获
    TIM5_Encoder_Config.IC2Selection=TIM_ICSELECTION_DIRECTTI;//映射到TI2上
    TIM5_Encoder_Config.IC2Prescaler=TIM_ICPSC_DIV1;          //配置输入分频，不分频
    TIM5_Encoder_Config.IC2Filter=0;                          //配置输入滤波器，不滤波 
    HAL_TIM_Encoder_Init(&TIM5_Handler,&TIM5_Encoder_Config); //初始化TIM5编码器接口	
		HAL_TIM_Encoder_Start(&TIM5_Handler,TIM_CHANNEL_ALL);     //打开TIM5的通道
}


//编码器模式初始化回调函数
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
	
  GPIO_InitTypeDef GPIO_Initure;                  //初始化IO
  if(htim->Instance==TIM1)   
  {
		__HAL_RCC_TIM1_CLK_ENABLE();                  //使能定时器1
		__HAL_RCC_GPIOA_CLK_ENABLE();		              //开启GPIOA时钟
	
		GPIO_Initure.Pin=GPIO_PIN_8|GPIO_PIN_9;      //PA8\9
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	        //复用推挽
    GPIO_Initure.Pull=GPIO_PULLUP;        		    //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	    //高速
		GPIO_Initure.Alternate=GPIO_AF1_TIM1;
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
  }
  else if(htim->Instance==TIM2)   
  {
		__HAL_RCC_TIM2_CLK_ENABLE();                    //使能定时器2
		__HAL_RCC_GPIOA_CLK_ENABLE();		            //开启GPIOA时钟
		__HAL_RCC_GPIOB_CLK_ENABLE();                   //开启GPIOB时钟
	
		GPIO_Initure.Pin=GPIO_PIN_3;                    //PB3
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	        //复用推挽
    GPIO_Initure.Pull=GPIO_PULLUP;        		    //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	    //高速
		GPIO_Initure.Alternate=GPIO_AF1_TIM2;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
	  
		GPIO_Initure.Pin=GPIO_PIN_15;          		    //PA15
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
  }
	else if(htim->Instance==TIM4)   
  {
		__HAL_RCC_TIM4_CLK_ENABLE();                  //使能定时器4
		__HAL_RCC_GPIOB_CLK_ENABLE();		              //开启GPIOD时钟
	
		GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7;     //PB6/7
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	        //复用推挽
    GPIO_Initure.Pull=GPIO_PULLUP;        		    //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	    //高速
		GPIO_Initure.Alternate=GPIO_AF2_TIM4;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  }
	else if(htim->Instance==TIM3)   
  {
		__HAL_RCC_TIM3_CLK_ENABLE();                  //使能定时器3
		__HAL_RCC_GPIOB_CLK_ENABLE();		              //开启GPIOB时钟
	
		GPIO_Initure.Pin=GPIO_PIN_4|GPIO_PIN_5;       //PB4/5
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	        //复用推挽
    GPIO_Initure.Pull=GPIO_PULLUP;        		    //上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     	    //高速
		GPIO_Initure.Alternate=GPIO_AF2_TIM3;
    HAL_GPIO_Init(GPIOB,&GPIO_Initure);
  }
	else if(htim->Instance==TIM5)
  {
		__HAL_RCC_TIM5_CLK_ENABLE();                    //使能定时器5
		__HAL_RCC_GPIOH_CLK_ENABLE();		            //开启GPIOH时钟
	
		GPIO_Initure.Pin=GPIO_PIN_10|GPIO_PIN_11;       //PH10,PH11
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	        //复用推挽
    GPIO_Initure.Pull=GPIO_PULLUP;        		   	//上拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     		//高速
		GPIO_Initure.Alternate=GPIO_AF2_TIM5;			//PH10,11复用为TIM5_CH1,2
    HAL_GPIO_Init(GPIOH,&GPIO_Initure);
  }
}


//TIM8 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM8_PWM_Init(u16 arr,u16 psc)
{ 
	
    TIM8_Handler.Instance=TIM8;            //定时器8
    TIM8_Handler.Init.Prescaler=psc;       //定时器分频
    TIM8_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM8_Handler.Init.Period=arr;          //自动重装载值
    TIM8_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM8_Handler);       //初始化PWM
    
    TIM8_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM8_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM8_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
		TIM8_CH1Handler.OCFastMode = TIM_OCFAST_DISABLE;
		TIM8_CH1Handler.OCIdleState = TIM_OCIDLESTATE_RESET;
		TIM8_CH1Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
		TIM8_CH2Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM8_CH2Handler.Pulse=arr/3;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为33%
    TIM8_CH2Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
		TIM8_CH2Handler.OCFastMode = TIM_OCFAST_DISABLE;
		TIM8_CH2Handler.OCIdleState = TIM_OCIDLESTATE_RESET;
		TIM8_CH2Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	
		TIM8_CH3Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM8_CH3Handler.Pulse=arr/4;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为25%
    TIM8_CH3Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
		TIM8_CH3Handler.OCFastMode = TIM_OCFAST_DISABLE;
		TIM8_CH3Handler.OCIdleState = TIM_OCIDLESTATE_RESET;
		TIM8_CH3Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		
		TIM8_CH4Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM8_CH4Handler.Pulse=arr/5;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为20%
    TIM8_CH4Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为高
		TIM8_CH4Handler.OCFastMode = TIM_OCFAST_DISABLE;
		TIM8_CH4Handler.OCIdleState = TIM_OCIDLESTATE_RESET;
		TIM8_CH4Handler.OCNIdleState = TIM_OCNIDLESTATE_RESET;
		
		
    HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH1Handler,TIM_CHANNEL_1);//配置TIM8通道1
		HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH2Handler,TIM_CHANNEL_2);//配置TIM8通道2
		HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH3Handler,TIM_CHANNEL_3);//配置TIM8通道3
		HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH4Handler,TIM_CHANNEL_4);//配置TIM8通道4
    HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_1);//开启通道1234
		HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_2);//开启通道1234
		HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_3);//开启通道1234
		HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_4);//开启通道1234
			

}
//TIM9 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM9_PWM_Init(u16 arr,u16 psc)
{ 
    TIM9_Handler.Instance=TIM9;            //定时器9
    TIM9_Handler.Init.Prescaler=psc;       //定时器分频
    TIM9_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM9_Handler.Init.Period=arr;          //自动重装载值
    TIM9_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM9_Handler);       //初始化PWM
    
    TIM9_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM9_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM9_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为低 
    HAL_TIM_PWM_ConfigChannel(&TIM9_Handler,&TIM9_CH1Handler,TIM_CHANNEL_1);//配置TIM9通道1
    HAL_TIM_PWM_Start(&TIM9_Handler,TIM_CHANNEL_1);//开启PWM通道1
}

//TIM10 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM10_PWM_Init(u16 arr,u16 psc)
{ 
    TIM10_Handler.Instance=TIM10;            //定时器10
    TIM10_Handler.Init.Prescaler=psc;       //定时器分频
    TIM10_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//向上计数模式
    TIM10_Handler.Init.Period=arr;          //自动重装载值
    TIM10_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM10_Handler);       //初始化TIM10
    
    TIM10_CH1Handler.OCMode=TIM_OCMODE_PWM1; //模式选择PWM1
    TIM10_CH1Handler.Pulse=arr/2;            //设置比较值,此值用来确定占空比，
                                            //默认比较值为自动重装载值的一半,即占空比为50%
    TIM10_CH1Handler.OCPolarity=TIM_OCPOLARITY_HIGH; //输出比较极性为high
    HAL_TIM_PWM_ConfigChannel(&TIM10_Handler,&TIM10_CH1Handler,TIM_CHANNEL_1);//配置TIM10通道1
    HAL_TIM_PWM_Start_IT(&TIM10_Handler,TIM_CHANNEL_1);//开启TIM10通道1中断模式
}
//  步进电机控制函数（10000脉冲/转，导程10mm）
//  距离单位mm，转速单位
void SendPulse(u8 distance)
{
	
	PU = distance * 1000 -1;
	TIM10_PWM_Init(1000-1,108-1);     //216M/108=2M的计数频率，自动重装载为1000，那么PWM频率为2M/1000=2kHZ
}
	

//定时器底层驱动，时钟使能，引脚配置
//此函数会被HAL_TIM_PWM_Init()调用
//htim:定时器句柄
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
   
		GPIO_InitTypeDef GPIO_Initure;
		
		if(htim->Instance==TIM8)
		{
			__HAL_RCC_TIM8_CLK_ENABLE();			//使能定时器8
			__HAL_RCC_GPIOC_CLK_ENABLE();			//开启GPIOC时钟
			
			GPIO_Initure.Pin=GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;           	//PC6789
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
			GPIO_Initure.Alternate=GPIO_AF3_TIM8;	//PC6、7、8、9复用为TIM8_CH1、2、3、4
			HAL_GPIO_Init(GPIOC,&GPIO_Initure);

		}
		else if(htim->Instance==TIM9)
		{
			__HAL_RCC_TIM9_CLK_ENABLE();			//使能定时器9
			__HAL_RCC_GPIOE_CLK_ENABLE();			//开启GPIOE时钟
		
			GPIO_Initure.Pin=GPIO_PIN_5;           	//PE5
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
			GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
			GPIO_Initure.Alternate=GPIO_AF3_TIM9;	//PE5复用为TIM9_CH1
			HAL_GPIO_Init(GPIOE,&GPIO_Initure);
		}
		else if(htim->Instance==TIM10)
		{
			__HAL_RCC_TIM10_CLK_ENABLE();			//使能定时器10
			__HAL_RCC_GPIOF_CLK_ENABLE();			//开启GPIOF时钟
		
			GPIO_Initure.Pin=GPIO_PIN_6;           	//PF6
			GPIO_Initure.Mode=GPIO_MODE_AF_PP;  	//复用推完输出
			GPIO_Initure.Pull=GPIO_PULLDOWN;          //上拉
			GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //高速
			GPIO_Initure.Alternate=GPIO_AF3_TIM10;	//PF6复用为TIM9_CH1
			HAL_GPIO_Init(GPIOF,&GPIO_Initure);
			HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 3, 3);  //中断优先级1/3
			HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);          //开启TIM10中断
			__HAL_TIM_ENABLE_IT(htim, TIM_IT_UPDATE);        //开启TIM10更新中断
		}
}

void TIM1_UP_TIM10_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&TIM10_Handler);
}

//TIM10中断服务函数调用
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{	
	
	
	if(htim==(&TIM10_Handler))
	{
//		printf("%d    %d\n",PU, RePU);	
		RePU ++;
		if(RePU>PU)
		{
			HAL_TIM_PWM_Stop_IT(&TIM10_Handler,TIM_CHANNEL_1);
			RePU = 0;
			PU = 0 ;
		}
		
	}
}

//设置TIM通道 1的占空比
//compare:比较值
void TIM_SetTIM9Compare1(u16 compare)
{
	TIM9->CCR1=compare; 
}
void TIM_SetTIM8Compare1(u16 compare)
{
	TIM8_CH1Handler.Pulse=compare; 
	HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH1Handler,TIM_CHANNEL_1);//配置TIM8通道1
	HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_1);//开启通道1234
		
}
void TIM_SetTIM8Compare2(u16 compare)
{
	TIM8_CH2Handler.Pulse=compare; 
	HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH2Handler,TIM_CHANNEL_2);//配置TIM8通道2
	HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_2);//开启通道1234
		
}
void TIM_SetTIM8Compare3(u16 compare)
{
	TIM8_CH3Handler.Pulse=compare; 
	HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH3Handler,TIM_CHANNEL_3);//配置TIM8通道3
	HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_3);//开启通道1234
}
void TIM_SetTIM8Compare4(u16 compare)
{
	TIM8_CH4Handler.Pulse=compare; 
	HAL_TIM_PWM_ConfigChannel(&TIM8_Handler,&TIM8_CH4Handler,TIM_CHANNEL_4);//配置TIM8通道4
	HAL_TIM_PWM_Start(&TIM8_Handler,TIM_CHANNEL_4);//开启通道1234
}
