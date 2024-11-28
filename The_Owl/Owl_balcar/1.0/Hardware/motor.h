#ifndef __MOTOR_H
#define __MOTOR_H

#include "allconfig.h"

#define L_PWMA   TIM8->CCR1  //PC6
#define L_PWMB   TIM8->CCR2  //PC7

#define R_PWMA   TIM8->CCR3  //PC8
#define R_PWMB   TIM8->CCR4  //PC9

#define PI 3.14159265							//PI圆周率
#define Control_Frequency  200.0	//编码器读取频率
#define Diameter_67  67.0 				//轮子直径67mm 
#define EncoderMultiples   4.0 		//编码器倍频数
#define Encoder_precision  11.0 	//编码器精度 11线
#define Reduction_Ratio  30.0			//减速比30
#define Perimeter  210.4867 			//周长，单位mm

#define MOTOR_IGNORE_PULSE (1300)//死区  1450 25Khz   此值需要看静止状态微调


void Balance_PWM_Init(u16 arr,u16 psc);
void Balance_Motor_Init(void);

int myabs(int a);
	
void Set_Pwm(int motor_left,int motor_right);
int PWM_Limit(int IN,int max,int min);


uint8_t Turn_Off(float angle, float voltage);
	
int PWM_Ignore(int pulse);
#endif
