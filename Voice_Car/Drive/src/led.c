#include "stm32f10x.h"                  // Device header
#include "public.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);   //使能GPIOC的时钟
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13;   //配置GPIO第13引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;   //配置GPIO为通用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;   //配置GPIO速率
	GPIO_Init(GPIOC,&GPIO_InitStruct);   //GPIO初始化函数
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);   //PC13引脚置位（相当于高电平），初始化灯熄灭
}

