#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1|GPIO_Pin_7; // TIM2-PA1 TIM3-PA7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	TIM_InternalClockConfig(TIM3);
	TIM_TimeBaseInitTypeDef TIM_TimebaseInitStructure;
	TIM_TimebaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimebaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimebaseInitStructure.TIM_Period = 20000 - 1;
	TIM_TimebaseInitStructure.TIM_Prescaler = 72 - 1;
	TIM_TimebaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2,&TIM_TimebaseInitStructure);
	TIM_TimeBaseInit(TIM3,&TIM_TimebaseInitStructure);
	
	TIM_OCInitTypeDef TIM_OCInitStructure;
	// 初始化结构体
	TIM_OCStructInit(&TIM_OCInitStructure);
	// OC输出模式
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	// 级性选择
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High  ;
	// 输出使能
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	// CCR值
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM2,&TIM_OCInitStructure);
	TIM_OC2Init(TIM3,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM2,ENABLE);
	TIM_Cmd(TIM3,ENABLE);
}

void PWM_SetCompare2_TIM2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);		//设置CCR2的值
}

void PWM_SetCompare2_TIM3(uint16_t Compare)
{
	TIM_SetCompare2(TIM3, Compare);		//设置CCR2的值
}