#ifndef __MPU6050_H
#define __MPU6050_H

#include "stm32f10x.h"                  // Device header
#include "MPU6050_Reg.h"
#include "Delay.h"
#include "Usblinker.h"
#include "GPIO.h"

#define MPU6050_ADDRESS		0xD0		//MPU6050的I2C从机地址
#define MPU6050_I2C			I2C2
#define MPU6050_RCC_I2C		RCC_APB1Periph_I2C2
#define MPU6050_RCC_GPIO	RCC_APB2Periph_GPIOB
#define MPU6050_I2C_GPIO_PORT 			GPIOB
#define MPU6050_SCL_Pin			GPIO_Pin_10
#define MPU6050_SDA_Pin			GPIO_Pin_11

struct Offset{				//零偏变量组
	int16_t x; 
	int16_t y; 
	int16_t z; 	
	};

extern struct Offset Off_Acc;
extern struct Offset Off_Gyr;	

void MPU6050_WriteReg(uint8_t RegAddress, uint8_t Data);
uint8_t MPU6050_ReadReg(uint8_t RegAddress);

void MPU6050_Config(void);
uint8_t MPU6050_GetID(void);
void MPU6050_GetData(int16_t *AccX, int16_t *AccY, int16_t *AccZ, 
						int16_t *GyroX, int16_t *GyroY, int16_t *GyroZ);
void MPU6050_Calibrate(void);
	
#endif
