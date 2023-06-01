#include "stm32f10x.h"                  
#include "public.h"



void TIM3_PWM_Init()
{  
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ�ܶ˿�ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//��ʼ��GPIO

	TIM_TimeBaseStructure.TIM_Period = 899; //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =0; //Ԥ��Ƶϵ��
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure); 
	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��PWMģʽ1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //����Ƚϼ��Ե�
	
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);  	
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
 
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ�ܶ�ʱ��3ͨ��3Ԥװ�ؼĴ���
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ�ܶ�ʱ��3ͨ��4Ԥװ�ؼĴ���

	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
}

