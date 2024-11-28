#include "app_control.h"


float battery = 12;//初始状态处于满电 12v


/**************************************************************************
Function: Get angle
Input   : way：The algorithm of getting angle 1：DMP  2：kalman  3：Complementary filtering
Output  : none
函数功能：获取角度	
入口参数：way：获取角度的算法 1：DMP  2：卡尔曼 3：互补滤波
返回  值：无
**************************************************************************/	
void Get_Angle(u8 way)
{ 
	float gyro_x,gyro_y,accel_x,accel_y,accel_z;
	float Accel_Y,Accel_Z,Accel_X,Accel_Angle_x,Accel_Angle_y,Gyro_X,Gyro_Z,Gyro_Y;
	Temperature=Read_Temperature();      //读取MPU6050内置温度传感器数据，近似表示主板温度。 //Read the data from the MPU6050 built-in temperature sensor, which approximately represents the motherboard temperature.
	if(way==1)                           //DMP的读取在数据采集中断读取，严格遵循时序要求  //The reading of DMP is interrupted during data collection, strictly following the timing requirements
	{	
		Read_DMP();                      	 //读取加速度、角速度、倾角  //Read acceleration, angular velocity, and tilt angle
		Angle_Balance=Pitch;             	 //更新平衡倾角,前倾为正，后倾为负 //Update the balance tilt angle, with positive forward tilt and negative backward tilt
		Gyro_Balance=gyro[0];              //更新平衡角速度,前倾为正，后倾为负  //Update the balance angular velocity, with positive forward tilt and negative backward tilt
		Gyro_Turn=gyro[2];                 //更新转向角速度 //Update steering angular velocity
		Acceleration_Z=accel[2];           //更新Z轴加速度计 //Update Z-axis accelerometer
	}			
	else
	{
		Gyro_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_XOUT_L);    //读取X轴陀螺仪 //Read X-axis gyroscope
		Gyro_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_YOUT_L);    //读取Y轴陀螺仪 //Read Y-axis gyroscope
		Gyro_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_GYRO_ZOUT_L);    //读取Z轴陀螺仪 //Read Z-axis gyroscope
		Accel_X=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_XOUT_L); //读取X轴加速度计 //Read X-axis accelerometer
		Accel_Y=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_YOUT_L); //读取X轴加速度计 //Read Y-axis accelerometer
		Accel_Z=(I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_H)<<8)+I2C_ReadOneByte(devAddr,MPU6050_RA_ACCEL_ZOUT_L); //读取Z轴加速度计 //Read Z-axis accelerometer
		if(Gyro_X>32768)  Gyro_X-=65536;                 //数据类型转换  也可通过short强制类型转换 Data type conversion can also be enforced through short type conversion
		if(Gyro_Y>32768)  Gyro_Y-=65536;                 //数据类型转换  也可通过short强制类型转换 Data type conversion can also be enforced through short type conversion
		if(Gyro_Z>32768)  Gyro_Z-=65536;                 //数据类型转换 Data type conversion
		if(Accel_X>32768) Accel_X-=65536;                //数据类型转换 Data type conversion
		if(Accel_Y>32768) Accel_Y-=65536;                //数据类型转换 Data type conversion
		if(Accel_Z>32768) Accel_Z-=65536;                //数据类型转换 Data type conversion
		Gyro_Balance=-Gyro_X;                            //更新平衡角速度 Update balance angular velocity
		accel_x=Accel_X/1671.84;
		accel_y=Accel_Y/1671.84;
		accel_z=Accel_Z/1671.84;
		gyro_x=Gyro_X/939.8;                              //陀螺仪量程转换 Gyroscope range conversion
		gyro_y=Gyro_Y/939.8;                              //陀螺仪量程转换 Gyroscope range conversion
		if(GET_Angle_Way==2)		  	
		{
			 Pitch= KF_X(accel_y,accel_z,-gyro_x)/PI*180;//卡尔曼滤波 Kalman filtering 
			 Roll = KF_Y(accel_x,accel_z,gyro_y)/PI*180;
		}
		else if(GET_Angle_Way==3) 
		{  
				Accel_Angle_x = atan2(Accel_Y,Accel_Z)*180/PI; //用Accel_Y和accel_y的参数得出的角度是一样的，只是边长不同 The angle obtained using Accel_Y and its parameters is the same, only the side length is different
				Accel_Angle_y = atan2(Accel_X,Accel_Z)*180/PI;
			
			 Pitch = -Complementary_Filter_x(Accel_Angle_x,Gyro_X/16.4);//互补滤波 Complementary filtering
			 Roll = -Complementary_Filter_y(Accel_Angle_y,Gyro_Y/16.4);
		}
		Angle_Balance=Pitch;                              //更新平衡倾角    Update the balance tilt angle
		Gyro_Turn=Gyro_Z;                                 //更新转向角速度  Update steering angular velocity
		Acceleration_Z=Accel_Z;                           //更新Z轴加速度计 Update Z-axis accelerometer
	}

}
