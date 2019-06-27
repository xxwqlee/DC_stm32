#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "timer.h"
#include "exti.h"
#include "sdram.h"
#include "tftlcd.h"
#include "ltdc.h"
#include "mpu.h"
#include "usmart.h"
#include "malloc.h"
#include "touch.h"
#include "GUI.h"
#include "WM.h"
#include "includes.h"
#include "FramewinDLG.h"
#include "DIALOG.h"
/************************************************
 ALIENTEK STM32开发板

 UCOSIII中以下优先级用户程序不能使用，ALIENTEK
 将这些优先级分配给了UCOSIII的5个系统内部任务
 优先级0：中断服务服务管理任务 OS_IntQTask()
 优先级1：时钟节拍任务 OS_TickTask()
 优先级2：定时任务 OS_TmrTask()
 优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
 优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
************************************************/

//任务优先级
#define START_TASK_PRIO				3
//任务堆栈大小	
#define START_STK_SIZE 				128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO				4
//任务堆栈大小
#define TOUCH_STK_SIZE				128
//任务控制块
OS_TCB TouchTaskTCB;
//任务堆栈
CPU_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];
//touch任务
void touch_task(void *p_arg);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 				5
//任务堆栈大小
#define LED0_STK_SIZE				128
//任务控制块
OS_TCB Led0TaskTCB;
//任务堆栈
CPU_STK LED0_TASK_STK[LED0_STK_SIZE];
//led0任务
void led0_task(void *p_arg);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO			6
//任务堆栈大小
#define EMWINDEMO_STK_SIZE			512
//任务控制块
OS_TCB EmwindemoTaskTCB;
//任务堆栈
CPU_STK EMWINDEMO_TASK_STK[EMWINDEMO_STK_SIZE];
//emwindemo_task任务
void emwindemo_task(void *p_arg);

//编码器读数转化为角度
double count_to_angle(u16 count)
{
	double angle = 0.0;
	angle = count / 10000 * 360;
	return angle;
}

int main(void)
{  
  OS_ERR err;
	CPU_SR_ALLOC();
    
	Write_Through();                //Cahce强制透写
	MPU_Memory_Protection();        //保护相关存储区域
	Cache_Enable();                 //打开L1-Cache
      
	Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
	HAL_Init();				        //初始化HAL库
	delay_init(216);                //延时初始化
	uart2_init(115200);		        //串口初始化
	EXTI_Init();                     //按键初始化
	LED_Init();                     //初始化LED
	SDRAM_Init();                   //SDRAM初始化
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
  my_mem_init(SRAMIN);		    //初始化内部内存池
	my_mem_init(SRAMEX);		    //初始化外部内存池
	my_mem_init(SRAMDTCM);		    //初始化DTCM内存池
    
  OSInit(&err);		            //初始化UCOSIII
	OS_CRITICAL_ENTER();            //进入临界区
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
	OS_CRITICAL_EXIT();	            //退出临界区	 
	OSStart(&err);                  //开启UCOSIII
	while(1);
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;

	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	//使能时间片轮转调度功能,设置默认的时间片长度
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	__HAL_RCC_CRC_CLK_ENABLE();		//使能CRC时钟
    WM_SetCreateFlags(WM_CF_MEMDEV);//开启STemWin存储设备
	GUI_Init();  			//STemWin初始化
	WM_MULTIBUF_Enable(1);  //开启STemWin多缓冲,RGB屏可能会用到
	OS_CRITICAL_ENTER();	//进入临界区
	//STemWin Demo任务	
	OSTaskCreate((OS_TCB*     )&EmwindemoTaskTCB,		
				 (CPU_CHAR*   )"Emwindemo task", 		
                 (OS_TASK_PTR )emwindemo_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )EMWINDEMO_TASK_PRIO,     
                 (CPU_STK*    )&EMWINDEMO_TASK_STK[0],	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE/10,	
                 (CPU_STK_SIZE)EMWINDEMO_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);
	//触摸屏任务
	OSTaskCreate((OS_TCB*     )&TouchTaskTCB,		
				 (CPU_CHAR*   )"Touch task", 		
                 (OS_TASK_PTR )touch_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )TOUCH_TASK_PRIO,     
                 (CPU_STK*    )&TOUCH_TASK_STK[0],	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE/10,	
                 (CPU_STK_SIZE)TOUCH_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);			 
	//LED0任务
	OSTaskCreate((OS_TCB*     )&Led0TaskTCB,		
				 (CPU_CHAR*   )"Led0 task", 		
                 (OS_TASK_PTR )led0_task, 			
                 (void*       )0,					
                 (OS_PRIO	  )LED0_TASK_PRIO,     
                 (CPU_STK*    )&LED0_TASK_STK[0],	
                 (CPU_STK_SIZE)LED0_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED0_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void*       )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR*     )&err);	                 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务			 
	OS_CRITICAL_EXIT();	//退出临界区
}

//EMWINDEMO任务
void emwindemo_task(void *p_arg)
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
	//更换皮肤
	BUTTON_SetDefaultSkin(BUTTON_SKIN_FLEX); 
	CHECKBOX_SetDefaultSkin(CHECKBOX_SKIN_FLEX);
	DROPDOWN_SetDefaultSkin(DROPDOWN_SKIN_FLEX);
	FRAMEWIN_SetDefaultSkin(FRAMEWIN_SKIN_FLEX);
	HEADER_SetDefaultSkin(HEADER_SKIN_FLEX);
	MENU_SetDefaultSkin(MENU_SKIN_FLEX);
	MULTIPAGE_SetDefaultSkin(MULTIPAGE_SKIN_FLEX);
	PROGBAR_SetDefaultSkin(PROGBAR_SKIN_FLEX);
	RADIO_SetDefaultSkin(RADIO_SKIN_FLEX);
	SCROLLBAR_SetDefaultSkin(SCROLLBAR_SKIN_FLEX);
	SLIDER_SetDefaultSkin(SLIDER_SKIN_FLEX);
	SPINBOX_SetDefaultSkin(SPINBOX_SKIN_FLEX);
	CreateFramewin();
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
		GUI_Delay(100);
	}
}

//TOUCH任务
void touch_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		GUI_TOUCH_Exec();	
		OSTimeDlyHMSM(0,0,0,5,OS_OPT_TIME_PERIODIC,&err);//延时5ms
	}
}

//LED0任务
void led0_task(void *p_arg)
{
	OS_ERR err;
	while(1)
	{
		LED0_Toggle;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_PERIODIC,&err);//延时500ms
	}
}

