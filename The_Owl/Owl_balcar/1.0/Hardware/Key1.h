#ifndef __BEEP_H
#define __BEEP_H

#include "allconfig.h"

#define KEY1_GPIO_PORT GPIOA
#define KEY1_GPIO_PIN  GPIO_Pin_8
#define KEY1_GPIO_CLK  RCC_APB2Periph_GPIOA

#define KEY_INT PAin(8)

void Key1_GPIO_Init(void);

#endif
