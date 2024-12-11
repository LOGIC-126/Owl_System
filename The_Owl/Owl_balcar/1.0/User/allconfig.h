#ifndef __ALLCONFIG_H 
#define __ALLCONFIG_H



////JTAG模式设置定义
#define JTAG_SWD_DISABLE   0X02
#define SWD_ENABLE         0X01
#define JTAG_SWD_ENABLE    0X00


//具体实现思想,参考<<CM3权威指南>>第五章(87页~92页).
//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 
//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 
 
//IO口操作,只对单一的IO口!
//确保n的值小于16!
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输出 
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输入 

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输出 
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输入

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输出 
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输入

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输出 
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输入


//电机
//Motor ML MR
typedef enum {
    MOTOR_ID_ML = 0,
    MOTOR_ID_MR,
    MAX_MOTOR
} Motor_ID;

/*小车运行状态枚举*/
typedef enum enCarState_t{
  enSTOP = 0,
  enRUN,
  enBACK,
  enLEFT,
  enRIGHT,
  enTLEFT,
  enTRIGHT,
	
	//ps2模拟值生效
	enps2Fleft, //前左旋
	enps2Fright,//前右旋
	enps2Bleft, //后左旋
	enps2Bright,//后右旋
	
	enAvoid, //超声波躲避  Ultrasonic avoidance
	enFollow, //超声波跟随  Ultrasonic Follow
	enError
}enCarState;


typedef enum Car_mode_t{
	
	
	Normal, //正常模式,包含蓝牙控制 //Normal mode, including Bluetooth controlled 
	U_Follow,//超声波跟随   //Ultrasonic Follow
	U_Avoid, //超声波避障   //Ultrasonic obstacle avoidance
	Weight_M,//负重模式     //Load bearing mode
	
	Mode_Max //最后模式的界限 如果要增加模式，在雷达避障前添加新模式  //If the boundary of the final mode needs to be increased, add a new mode before radar obstacle avoidance
	
	
}Car_Mode;

#include "stm32f10x.h"                  // Device header
//头文件
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#include "delay.h"
#include "GPIO.h"
#include "Sbus.h"
#include "Usblinker.h"
#include "Encoder.h"
#include "motor.h"

//Mpu6050
#include "IOI2C.h"
#include "MPU6050.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "intsever.h"

//平衡车整体控制
#include "app_control.h"
#include "pid_control.h"

//filtering alforithm
#include "filter.h"
#include "KF.h"

//OLED
#include "bsp_oled.h"
#include "bsp_oled_i2c.h"

//存储器
#include "W25Q64.h"


//引出的通用变量
extern float Velocity_Left,Velocity_Right; //轮子的速度
extern uint8_t GET_Angle_Way;                              //获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波 
extern float Angle_Balance,Gyro_Balance,Gyro_Turn;     //平衡倾角 平衡陀螺仪 转向陀螺仪
extern int Motor_Left,Motor_Right;                 	  //电机PWM变量
extern int Temperature;                                //温度变量
extern float Acceleration_Z;                           //Z轴加速度计 
extern int 	Mid_Angle;                          		//机械中值
extern float Move_X,Move_Z;													//Move_X:前进速度  Move_Z：转向速度
extern float battery; 															//电池电量	
extern u8 lower_power_flag; 											//低电压标志,电压恢复标志
extern u32 g_distance; 														//超声波距离值
extern u8 Flag_velocity; 		//速度控制相关变量
extern u8 Stop_Flag;//停止标志

void All_config(void);
void NVIC_Config(void);



#endif
