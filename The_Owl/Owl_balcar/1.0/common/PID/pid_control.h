#ifndef __PID_CONTROL_H
#define __PID_CONTROL_H

#include "allconfig.h"


extern float Balance_Kp;  // 直立环PD控制参数 - Kp
extern float Balance_Kd;  // 直立环PD控制参数 - Kd
extern float Velocity_Kp; // 速度环PI控制参数 - Kp
extern float Velocity_Ki; // 速度环PI控制参数 - Ki
extern float Turn_Kp;     // 转向环PD控制参数 - Kp
extern float Turn_Kd;     // 转向环PD控制参数 - Kd

void Get_USART_PID(float* datas);
void Get_W25Q64_PID(void);
void SavePID(void);
int Balance_PD(float Angle,float Gyro);
int Velocity_PI(int encoder_left,int encoder_right);
int Turn_PD(float gyro);
int myabs(int a);

#endif

