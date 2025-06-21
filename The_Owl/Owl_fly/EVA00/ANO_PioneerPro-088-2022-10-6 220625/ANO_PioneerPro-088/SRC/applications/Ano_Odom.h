#ifndef __ANO_ODOM_H
#define	__ANO_ODOM_H

#include "stm32f4xx.h"
typedef struct
{
	float X;
	float Y;
}Position2D;

extern Position2D total_displacement;

void PosCalculation_Task(u8 dT_ms);

#endif
