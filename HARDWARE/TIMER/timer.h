#ifndef _TIMER_H
#define _TIMER_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 				  
////////////////////////////////////////////////////////////////////////////////// 
extern TIM_HandleTypeDef TIM1_Handler;              //定时器1句柄 
extern TIM_Encoder_InitTypeDef TIM1_Encoder_Config; //1编码器接口

extern TIM_HandleTypeDef TIM2_Handler;              //定时器2句柄 
extern TIM_Encoder_InitTypeDef TIM2_Encoder_Config; //2编码器接口

extern TIM_HandleTypeDef TIM3_Handler;              //定时器3句柄 
extern TIM_Encoder_InitTypeDef TIM3_Encoder_Config; //3编码器接口

extern TIM_HandleTypeDef TIM4_Handler;              //定时器4句柄 
extern TIM_Encoder_InitTypeDef TIM4_Encoder_Config; //4编码器接口

extern TIM_HandleTypeDef TIM5_Handler;              //定时器5句柄 
extern TIM_Encoder_InitTypeDef TIM5_Encoder_Config; //5编码器接口


extern TIM_HandleTypeDef TIM8_Handler;         //定时器8句柄
extern TIM_OC_InitTypeDef TIM8_CH1Handler;     //定时器8通道1句柄
extern TIM_OC_InitTypeDef TIM8_CH2Handler;     //定时器8通道2句柄
extern TIM_OC_InitTypeDef TIM8_CH3Handler;     //定时器8通道3句柄
extern TIM_OC_InitTypeDef TIM8_CH4Handler;     //定时器8通道4句柄

extern TIM_HandleTypeDef TIM9_Handler;      //定时器9PWM句柄 
extern TIM_OC_InitTypeDef TIM9_CH1Handler;  //定时器9通道1句柄

extern TIM_HandleTypeDef TIM10_Handler;         //定时器10句柄 
extern TIM_OC_InitTypeDef TIM10_CH1Handler;     //定时器10通道1句柄

void TIM1_Encoder_Init(u16 arr,u16 psc);
void TIM2_Encoder_Init(u32 arr,u16 psc);
void TIM3_Encoder_Init(u16 arr,u16 psc);
void TIM4_Encoder_Init(u16 arr,u16 psc);
void TIM5_Encoder_Init(u32 arr,u16 psc);

void TIM8_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM8Compare1(u16 compare);
void TIM_SetTIM8Compare2(u16 compare);
void TIM_SetTIM8Compare3(u16 compare);
void TIM_SetTIM8Compare4(u16 compare);

void TIM9_PWM_Init(u16 arr,u16 psc);
void TIM_SetTIM9Compare1(u16 compare);

void TIM10_PWM_Init(u16 arr,u16 psc);
void SendPulse(u8 distance);
volatile static u16 PU = 0;
volatile static u16 RePU = 0;
#endif

