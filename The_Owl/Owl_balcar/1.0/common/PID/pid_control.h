#ifndef __PID_CONTROL_H
#define __PID_CONTROL_H

#include "allconfig.h"

int Balance_PD(float Angle,float Gyro);
int Velocity_PI(int encoder_left,int encoder_right);
int Turn_PD(float gyro);
int myabs(int a);

#endif

