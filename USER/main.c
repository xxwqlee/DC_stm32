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
/************************************************
 ALIENTEK 阿波罗STM32F7开发板 
************************************************/

//u16 testsram[250000] __attribute__((at(0XC0000000)));//测试用数组

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

//清空屏幕并在右上角显示"RST"
void Load_Drow_Dialog(void)
{
		LCD_Clear(WHITE);//清屏   
		POINT_COLOR=BLUE;//设置字体为蓝色 
		LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//显示清屏区域
		POINT_COLOR=RED;//设置画笔蓝色 
}
////////////////////////////////////////////////////////////////////////////////
//电容触摸屏专有部分
//画水平线
//x0,y0:坐标
//len:线长度
//color:颜色
void gui_draw_hline(u16 x0,u16 y0,u16 len,u16 color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//画实心圆
//x0,y0:坐标
//r:半径
//color:颜色
void gui_fill_circle(u16 x0,u16 y0,u16 r,u16 color)
{											  
	u32 i;
	u32 imax = ((u32)r*707)/1000+1;
	u32 sqmax = (u32)r*(u32)r+(u32)r/2;
	u32 x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  
//两个数之差的绝对值 
//x1,x2：需取差值的两个数
//返回值：|x1-x2|
u16 my_abs(u16 x1,u16 x2)
{			 
	if(x1>x2)return x1-x2;
	else return x2-x1;
}  
//画一条粗线
//(x1,y1),(x2,y2):线条的起始坐标
//size：线条的粗细程度
//color：线条的颜色
void lcd_draw_bline(u16 x1, u16 y1, u16 x2, u16 y2,u8 size,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		gui_fill_circle(uRow,uCol,size,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}   
////////////////////////////////////////////////////////////////////////////////
//5个触控点的颜色(电容触摸屏用)												 
const u16 POINT_COLOR_TBL[5]={RED,GREEN,BLUE,BROWN,GRED};  

//电容触摸屏测试函数
void ctp_test(void)
{
	u8 t=0;
	u8 i=0;	  	
	u16 count1 = 0;
	u16 count2 = 0;
	u16 count3 = 0;
	u16 count4 = 0;
	u16 count5 = 0;	
 	u16 lastpos[5][2];		//最后一次的数据 
	while(1)
	{
		tp_dev.scan(0);
		for(t=0;t<5;t++)
		{
			if((tp_dev.sta)&(1<<t))
			{
                //printf("X坐标:%d,Y坐标:%d\r\n",tp_dev.x[0],tp_dev.y[0]);
				if(tp_dev.x[t]<lcddev.width&&tp_dev.y[t]<lcddev.height)
				{
					if(lastpos[t][0]==0XFFFF)
					{
						lastpos[t][0] = tp_dev.x[t];
						lastpos[t][1] = tp_dev.y[t];
					}
                    
					lcd_draw_bline(lastpos[t][0],lastpos[t][1],tp_dev.x[t],tp_dev.y[t],2,POINT_COLOR_TBL[t]);//画线
					lastpos[t][0]=tp_dev.x[t];
					lastpos[t][1]=tp_dev.y[t];
					if(tp_dev.x[t]>(lcddev.width-24)&&tp_dev.y[t]<20)
					{
						Load_Drow_Dialog();//清除
					}
				}
			}else lastpos[t][0]=0XFFFF;
		}
		
		delay_ms(5);i++;
		if(i%20==0)LED0_Toggle;
		
		 	
		count1=__HAL_TIM_GET_COUNTER(&TIM1_Handler);
		count2=__HAL_TIM_GET_COUNTER(&TIM2_Handler);
		count3=__HAL_TIM_GET_COUNTER(&TIM3_Handler);
		count4=__HAL_TIM_GET_COUNTER(&TIM4_Handler);
		count5=__HAL_TIM_GET_COUNTER(&TIM5_Handler);
		LCD_ShowxNum(30,500,count1,6,24,0);
		LCD_ShowxNum(30,540,count2,6,24,0);
		LCD_ShowxNum(30,580,count3,6,24,0);
		LCD_ShowxNum(30,620,count4,6,24,0);
		LCD_ShowxNum(30,660,count5,6,24,0);
//		printf("%d   %d   %d   %d    %d\n",count1,count2,count3,count4,count5);	
		
	}	
}


int main(void)
{
		u16 val1 = 0;   //0.68V
		u16 val2 = 0;   //1.33V
		u16 val3 = 0;   //1.99V
		u16 val4 = 0;   //2.64V
		u16 val5 = 0;     //3.30V
		
		u8 lcd_id[12];
//		u8 x = 0;
//		u32 ts = 0;
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				              //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    SDRAM_Init();                   //初始化SDRAM
    delay_init(216);                //延时初始化
		uart2_init(115200);		       	 //串口2初始化
    LED_Init();                     //初始化LED	
		EXTI_Init();                    //外部中断初始化
    TIM8_PWM_Init(100-1,108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
		TIM9_PWM_Init(100-1,108-1);     //216M/108=2M的计数频率，自动重装载为100，那么PWM频率为2M/100=20kHZ
		TFTLCD_Init();                     //初始化LCD
		tp_dev.init();				    			//触摸屏初始化 
		my_mem_init(SRAMIN);		    //初始化内部内存池
		my_mem_init(SRAMEX);		    //初始化外部内存池
		my_mem_init(SRAMDTCM);		    //初始化DTCM内存池
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
//		for(ts=0;ts<250000;ts++)
//		{
//		testsram[ts]=ts;//预存测试数据	 
//  	}
//		fsmc_sdram_test(30,170);//测试SRAM容量
//		LCD_ShowxNum(30,190,testsram[ts],6,16,0);//显示测试数据	
//		printf("testsram[%d]:%d\r\n",ts,testsram[ts]);
		
		Load_Drow_Dialog();	 	
	  ctp_test();//电容屏测试

}
