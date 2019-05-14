#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "exti.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 
************************************************/

int main(void)
{
		u16 val1 = 0;   //0.68V
		u16 val2 = 0;   //1.33V
		u16 val3 = 0;   //1.99V
		u16 val4 = 0;   //2.64V
		u16 val5 = 0;     //3.30V
		u16 count1 = 0;
		u16 count2 = 0;
		u16 count3 = 0;
		u16 count4 = 0;
		u16 count5 = 0;
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
		uart2_init(115200);		        //串口2初始化
    LED_Init();                     //初始化LED	
		EXTI_Init();                    //外部中断初始化
    TIM8_PWM_Init(100-1,108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
		TIM9_PWM_Init(100-1,108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
		
		// 关节编码器读数
		TIM1_Encoder_Init(0xFFFF-1,1-1);  //初始化编码器1 逆时针增
		TIM2_Encoder_Init(0xFFFF-1,1-1);	//初始化编码器2 逆时针增
		TIM3_Encoder_Init(0xFFFF-1,1-1);	//初始化编码器3 顺时针增
		TIM4_Encoder_Init(0xFFFF-1,1-1);	//初始化编码器4 顺时针增
		TIM5_Encoder_Init(0xFFFF-1,1-1);	//初始化编码器5 顺时针增
	  
		TIM_SetTIM8Compare1(val1);	//修改比较值，修改占空比
		TIM_SetTIM8Compare2(val2);	//修改比较值，修改占空比
		TIM_SetTIM8Compare3(val3);	//修改比较值，修改占空比
		TIM_SetTIM8Compare4(val4);	//修改比较值，修改占空比
		TIM_SetTIM9Compare1(val5);	//修改比较值，修改占空比

		
    while(1)
    {
				delay_ms(1000);	 	
				count1=__HAL_TIM_GET_COUNTER(&TIM1_Handler);
				count2=__HAL_TIM_GET_COUNTER(&TIM2_Handler);
				count3=__HAL_TIM_GET_COUNTER(&TIM3_Handler);
				count4=__HAL_TIM_GET_COUNTER(&TIM4_Handler);
				count5=__HAL_TIM_GET_COUNTER(&TIM5_Handler);
				printf("%d   %d   %d   %d    %d\n",count1,count2,count3,count4,count5);	

				
    } 
}
