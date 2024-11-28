#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"
#include "GPIO.h"

/**
  * @brief  PWM初始化函数
  * @param  gpioConfig: 包含GPIO配置的结构体
  * @param  Frequency: 目标PWM频率（单位：Hz）
  * @retval 无
  */
void PWM_Init(The_GPIO_Config gpioConfig, float Frequency);

/**
  * @brief  设置PWM占空比函数
  * @param  DutyCycle: 占空比（范围：0.0f到100.0f）
  * @retval 无
  */
void PWM_SetCompare1(uint16_t DutyCycle);

#endif /* __PWM_H */
