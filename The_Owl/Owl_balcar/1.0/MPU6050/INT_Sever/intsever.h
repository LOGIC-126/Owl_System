#ifndef __INTSERVER_H_
#define __INTSERVER_H_

#include "allconfig.h"


#define MPU6050_INT PAin(12)   //PA12连接到MPU6050的中断引脚

void MPU6050_EXTI_Init(void);




#endif

