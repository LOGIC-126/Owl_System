#ifndef __APP_CONTROL_H_
#define __APP_CONTROL_H_

#include "allconfig.h" 

#define MPU6050_INT PAin(12)   //PA12连接到MPU6050的中断引脚

void Get_Angle(u8 way);

#endif

