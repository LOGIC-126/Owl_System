/*
* @par Copyright (C): 2018-2028, Shenzhen Yahboom Tech
* @file         // intsever.c
* @author       // lly
* @version      // V1.0
* @date         // 20240628
* @brief        // 小车控制平衡的主要中断服务
*/


#include "intsever.h"

void MPU6050_EXTI_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);   //外部中断，需要使能AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); //使能GPIO端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	          //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         //上拉输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource12);
	EXTI_InitStructure.EXTI_Line=EXTI_Line12;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
}






