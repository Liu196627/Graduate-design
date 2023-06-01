#include "stm32f10x.h"                  // Device header
#include "public.h"


void motor(void)//电机的GPIO口的初始化
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能端口时钟
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);	   //
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);//初始化GPIO
	
}


void Car_Run(int Speed)
{
	LeftMotor_Go();
	RightMotor_Go();
	LeftMotorPWM(Speed);		  
	RightMotorPWM(Speed);	
}

void Car_Back(int Speed)
{
	LeftMotor_Back();
	RightMotor_Back();

	LeftMotorPWM(Speed);		  
	RightMotorPWM(Speed);	
}

void Car_Right(int Speed)
{
	LeftMotor_Stop();
	RightMotor_Go();

	LeftMotorPWM(0);		  
	RightMotorPWM(Speed);		
}

void Car_Left(int Speed)
{
	LeftMotor_Go();
	RightMotor_Stop();

	LeftMotorPWM(Speed);		  
	RightMotorPWM(0);		
}

void Car_Stop(void)
{
	LeftMotor_Stop();
	RightMotor_Stop();

	LeftMotorPWM(0);		  
	RightMotorPWM(0);		
}


void Car_SpinRight(int LeftSpeed, int RightSpeed)
{
	LeftMotor_Back();
	RightMotor_Go();

	LeftMotorPWM(LeftSpeed);		  
	RightMotorPWM(RightSpeed);		
}

void Car_SpinLeft(int LeftSpeed, int RightSpeed)
{
	LeftMotor_Go();
	RightMotor_Back();

	LeftMotorPWM(LeftSpeed);		  
	RightMotorPWM(RightSpeed);		
}
