#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f10x.h"


/**
  * @brief  GPIO初始化
  * @param  GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed
  * @retval 无
  */
void GPIO_Config(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin,GPIOMode_TypeDef GPIO_Mode,GPIOSpeed_TypeDef GPIO_Speed);

#endif /* __GPIO_H */
