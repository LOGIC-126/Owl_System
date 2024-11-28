#ifndef __BEEP_H
#define __BEEP_H

#include "allconfig.h"

#define BEEP_RCC   RCC_APB2Periph_GPIOA
#define BEEP_PORT  GPIOA
#define BEEP_PIN   GPIO_Pin_11

#define BEEP_ON  GPIO_SetBits(BEEP_PORT,BEEP_PIN)
#define BEEP_OFF  GPIO_ResetBits(BEEP_PORT,BEEP_PIN)

#define BEEP_BEEP PAout(11) 


void init_beep(void);
void beep_timer(void);

void open_beep(u32 beep_time);


#endif
