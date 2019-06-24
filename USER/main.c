#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "timer.h"
#include "exti.h"
#include "lcd.h"
#include "mpu.h"
#include "sdram.h"
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 
************************************************/

u16 testsram[250000] __attribute__((at(0XC0000000)));//测试用数组

//SDRAM内存测试	    
void fsmc_sdram_test(u16 x,u16 y)
{  
	u32 i=0;  	  
	u32 temp=0;	   
	u32 sval=0;	//在地址0读到的数据	  				   
  	LCD_ShowString(x,y,180,y+16,16,"Ex Memory Test:    0KB "); 
	//每隔16K字节,写入一个数据,总共写入2048个数据,刚好是32M字节
	for(i=0;i<32*1024*1024;i+=16*1024)
	{
		*(vu32*)(Bank5_SDRAM_ADDR+i)=temp; 
		temp++;
	}
	//依次读出之前写入的数据,进行校验		  
 	for(i=0;i<32*1024*1024;i+=16*1024) 
	{	
  		temp=*(vu32*)(Bank5_SDRAM_ADDR+i);
		if(i==0)sval=temp;
 		else if(temp<=sval)break;//后面读出的数据一定要比第一次读到的数据大.	   		   
		LCD_ShowxNum(x+15*8,y,(u16)(temp-sval+1)*16,5,16,0);	//显示内存容量  
		printf("SDRAM Capacity:%dKB\r\n",(u16)(temp-sval+1)*16);//打印SDRAM容量
 	}					 
}	

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
		u8 lcd_id[12];
		u8 x = 0;
		u32 ts = 0;
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    SDRAM_Init();                   //初始化SDRAM
    delay_init(216);                //延时初始化
		uart2_init(115200);		        //串口2初始化
    LED_Init();                     //初始化LED	
		EXTI_Init();                    //外部中断初始化
    TIM8_PWM_Init(100-1,108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
		TIM9_PWM_Init(100-1,108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
		LCD_Init();                     //初始化LCD
    POINT_COLOR=RED; 
		sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。	
		
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
		
		//内存保护
		MPU_Memory_Protection();
//		MPU_Set_Protection(0X20002000,MPU_REGION_SIZE_128B,MPU_REGION_NUMBER0,MPU_REGION_PRIV_RO_URO);//只读  
//		printf("MPU open!\r\n");	//提示MPU打开
		

		//SDRAM测试
		for(ts=0;ts<250000;ts++)
		{
		testsram[ts]=ts;//预存测试数据	 
  	}
		fsmc_sdram_test(30,170);//测试SRAM容量
		LCD_ShowxNum(30,190,testsram[ts],6,16,0);//显示测试数据	
		printf("testsram[%d]:%d\r\n",ts,testsram[ts]);
		
    while(1)
    {
				delay_ms(1000);	 	
				count1=__HAL_TIM_GET_COUNTER(&TIM1_Handler);
				count2=__HAL_TIM_GET_COUNTER(&TIM2_Handler);
				count3=__HAL_TIM_GET_COUNTER(&TIM3_Handler);
				count4=__HAL_TIM_GET_COUNTER(&TIM4_Handler);
				count5=__HAL_TIM_GET_COUNTER(&TIM5_Handler);
				printf("%d   %d   %d   %d    %d\n",count1,count2,count3,count4,count5);	
				switch(x)
				{
						case 0:LCD_Clear(WHITE);break;
						case 1:LCD_Clear(BLACK);break;
						case 2:LCD_Clear(BLUE);break;
						case 3:LCD_Clear(RED);break;
						case 4:LCD_Clear(MAGENTA);break;
						case 5:LCD_Clear(GREEN);break;
						case 6:LCD_Clear(CYAN);break; 
						case 7:LCD_Clear(YELLOW);break;
						case 8:LCD_Clear(BRRED);break;
						case 9:LCD_Clear(GRAY);break;
						case 10:LCD_Clear(LGRAY);break;
						case 11:LCD_Clear(BROWN);break;
				}
				POINT_COLOR=RED;	  
				LCD_ShowString(10,40,260,32,32,"Apollo STM32F4/F7"); 	
				LCD_ShowString(10,80,240,24,24,"TFTLCD TEST");
				LCD_ShowString(10,110,240,16,16,"ATOM@ALIENTEK");
				LCD_ShowString(10,130,240,16,16,lcd_id);		//显示LCD ID	      					 
				LCD_ShowString(10,150,240,12,12,"2016/7/11");	      					 
				x++;
				if(x==12)x=0;
				LED0_Toggle;

				
    } 
}
