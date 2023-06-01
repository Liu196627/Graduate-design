#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "public.h"

void motor(void);

void Car_Run(int Speed);
void Car_Back(int Speed);
void Car_Right(int Speed);
void Car_Left(int Speed);
void Car_SpinRight(int LeftSpeed, int RightSpeed);
void Car_SpinLeft(int LeftSpeed, int RightSpeed);
void Car_Stop(void);


#define  LeftMotor_Go()			{GPIO_SetBits(GPIOB, GPIO_Pin_5); GPIO_ResetBits(GPIOB, GPIO_Pin_6);}
#define  LeftMotor_Back()		{GPIO_ResetBits(GPIOB, GPIO_Pin_5); GPIO_SetBits(GPIOB, GPIO_Pin_6);}
#define  LeftMotor_Stop()		{GPIO_ResetBits(GPIOB, GPIO_Pin_5); GPIO_ResetBits(GPIOB, GPIO_Pin_6);}

#define  RightMotor_Go()		{GPIO_SetBits(GPIOB, GPIO_Pin_7); GPIO_ResetBits(GPIOB, GPIO_Pin_8);}
#define  RightMotor_Back()		{GPIO_ResetBits(GPIOB, GPIO_Pin_7); GPIO_SetBits(GPIOB, GPIO_Pin_8);}
#define  RightMotor_Stop()		{GPIO_ResetBits(GPIOB, GPIO_Pin_7); GPIO_ResetBits(GPIOB, GPIO_Pin_8);}

#define  LeftMotorPWM(Speed)	TIM_SetCompare3(TIM3, Speed);
#define  RightMotorPWM(Speed)	TIM_SetCompare4(TIM3, Speed);		

#endif
