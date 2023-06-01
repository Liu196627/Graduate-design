#include "stm32f10x.h"                  
#include "public.h"



void TIM3_PWM_Init()
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//使能定时器3时钟	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能端口时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIO

	TIM_TimeBaseStructure.TIM_Period = 899; //自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler =0; //预分频系数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择PWM模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出比较极性地
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能定时器3通道3预装载寄存器
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能定时器3通道4预装载寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

