#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "exti.h"
#include "tftlcd.h"
#include "mpu.h"
#include "sdram.h"
#include "touch.h"
#include "malloc.h"
#include "GUI.h"
#include "WM.h"
#include "GUIDEMO.h"
#include "FramewinDLG.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 
************************************************/

//编码器读数转化为角度
double count_to_angle(u16 count)
{
	double angle = 0.0;
	angle = count / 10000 * 360;
	return angle;
}

int main(void)
{
	u16 count1 = 0;
	u16 count2 = 0;
	u16 count3 = 0;
	u16 count4 = 0;
	u16 count5 = 0;	
	double angle1 = 0.0;
	double angle2 = 0.0;
	double angle3 = 0.0;
	double angle4 = 0.0;
	double angle5 = 0.0;
	Write_Through();                //Cahce强制透写
	MPU_Memory_Protection();    	//内存保护
    Cache_Enable();                 //打开L1-Cache
	
    HAL_Init();				           //初始化HAL库
    Stm32_Clock_Init(432, 25, 2, 9);   //设置时钟,216Mhz 
    SDRAM_Init();                   //初始化SDRAM
    delay_init(216);                //延时初始化
	uart2_init(115200);		       	//串口2初始化
    LED_Init();                     //初始化LED	
	EXTI_Init();                    //外部中断初始化
	TFTLCD_Init();                  //初始化LCD
	TP_Init();				        //触摸屏初始化 
	// 初始化定时器1-5用于关节编码器读数
	TIM1_Encoder_Init(0xFFFF-1, 1-1);  //初始化编码器1 逆时针增
	TIM2_Encoder_Init(0xFFFF-1, 1-1);	 //初始化编码器2 逆时针增
	TIM3_Encoder_Init(0xFFFF-1, 1-1);	 //初始化编码器3 顺时针增
	TIM4_Encoder_Init(0xFFFF-1, 1-1);	 //初始化编码器4 顺时针增
	TIM5_Encoder_Init(0xFFFF-1, 1-1);	 //初始化编码器5 顺时针增
	//初始化定时器6/7用于提供系统时钟
//	TIM6_Init(999, 108 - 1);             //1KHZ 定时器6设置为1ms
//  TIM7_Init(999, 1080 - 1);            //触摸屏扫描速度,100HZ.
	//初始化定时器8/9用于控制PMW输出
	TIM8_PWM_Init(100-1, 108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
	TIM9_PWM_Init(100-1, 108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
	//内存初始化
	my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMDTCM);		    //初始化DTCM内存池
	
	//初始化触摸屏界面
	__HAL_RCC_CRC_CLK_ENABLE();		 //使能CRC时钟
	WM_SetCreateFlags(WM_CF_MEMDEV); //开启STemWin存储设备
	GUI_Init(); 					 //初始化GUI
    GUI_SetBkColor(GUI_BLACK);       //背景颜色
	GUI_Clear();				     //清屏
	GUI_SetColor(GUI_YELLOW);        //设置颜色
	GUI_SetFont(&GUI_Font24_ASCII);  //设置字体
	GUI_DispStringHCenterAt("Auxiliary Drilling Device For Spinal Surgery", 240, 0); //指定位置显示
	GUI_DispStringAt("relative angles", 10, 520); //指定位置显示
	GUI_DispStringAt("angle1:", 10, 550); //指定位置显示
	GUI_DispStringAt("angle2:", 10, 600); //指定位置显示
	GUI_DispStringAt("angle3:", 10, 650); //指定位置显示
	GUI_DispStringAt("angle4:", 10, 700); //指定位置显示
	GUI_DispStringAt("angle5:", 10, 750); //指定位置显示
	CreateFramewin();
//	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
//	GUIDEMO_Main();					//STemWin 演示demo
	while(1)
    {
			count1=__HAL_TIM_GET_COUNTER(&TIM1_Handler);
			count2=__HAL_TIM_GET_COUNTER(&TIM2_Handler);
			count3=__HAL_TIM_GET_COUNTER(&TIM3_Handler);
			count4=__HAL_TIM_GET_COUNTER(&TIM4_Handler);
			count5=__HAL_TIM_GET_COUNTER(&TIM5_Handler);
			angle1 = count_to_angle(count1);
			angle2 = count_to_angle(count2);
			angle3 = count_to_angle(count3);
			angle4 = count_to_angle(count4);
			angle5 = count_to_angle(count5);
			GUI_GotoXY(85, 550);
			GUI_DispSFloatFix(angle1,6,1);
			GUI_GotoXY(85, 600);
			GUI_DispSFloatFix(angle2,6,1);
			GUI_GotoXY(85, 650);
			GUI_DispSFloatFix(angle3,6,1);
			GUI_GotoXY(85, 700);
			GUI_DispSFloatFix(angle4,6,1);
			GUI_GotoXY(85, 750);
			GUI_DispSFloatFix(angle5,6,1);
			delay_ms(10);
	}

}

