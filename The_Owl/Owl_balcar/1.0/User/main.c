#include "allconfig.h"

uint8_t GET_Angle_Way=3;                             
//获取角度的算法，1：四元数  2：卡尔曼  3：互补滤波  //Algorithm for obtaining angles, 1: Quaternion 2: Kalman 3: Complementary filtering

float Angle_Balance,Gyro_Balance,Gyro_Turn;     		
//平衡倾角 平衡陀螺仪 转向陀螺仪 //Balance tilt angle balance gyroscope steering gyroscope

int Motor_Left,Motor_Right;                 	  		
//电机PWM变量 //Motor PWM variable

int Temperature;                                		
//温度变量 		//Temperature variable

float Acceleration_Z;                           		
//Z轴加速度计  //Z-axis accelerometer

int Mid_Angle = 4;                          						
//机械中值  //Mechanical median

float Move_X,Move_Z; 
//Move_X:前进速度  Move_Z：转向速度  //Move_X: Forward speed Move_Z: Steering speed

u8 Stop_Flag = 1; 
//0:开始 1:停止  //0: Start 1: Stop

u8 MPU6050_Flag = 1;
//0:开始 1:停止  //0: Start 1: Stop

int Costime = 0;
//计时变量

extern char Command[10];
//指令字符串

extern uint8_t ComFlag;
//指令接收标志


int8_t datapacket[CTP_LONGTH];

int main(void)
{
	All_config();
	Get_W25Q64_PID();
	while(1)
	{	
		if (Costime == 10){
			datapacket[0] = Angle_Balance;
			datapacket[1] = Gyro_Balance;
			datapacket[2] = Gyro_Turn;
//			printf("<debug>Balance_Kp: %f \n",Balance_Kp);
			if(MPU6050_Flag == 0)
				USB_UsartSendpacket(DEBUG_USART,datapacket,CTP_LONGTH);
			Costime = 0;
		}

	}	

	
}


void EXTI15_10_IRQHandler(void)
{
	int Encoder_Left,Encoder_Right;             					//左右编码器的脉冲计数
	int Balance_Pwm,Velocity_Pwm,Turn_Pwm;		  					//平衡环PWM变量，速度环PWM变量，转向环PWM变
	
  // 检查是否发生中断事件
  if(MPU6050_INT==0)		
	{   
		EXTI->PR=1<<12;                           					//清除中断标志位 
//		angle_flag = 1;
		Costime +=1;
		
		Get_Angle(GET_Angle_Way);                     			//更新姿态，5ms一次，更高的采样频率可以改善卡尔曼滤波和互补滤波的效果 Updating the posture once every 5ms, a higher sampling frequency can improve the effectiveness of Kalman filtering and complementary filtering
		Encoder_Left=Read_Encoder(MOTOR_ID_ML);            					//读取左轮编码器的值，前进为正，后退为负 Read the value of the left wheel encoder, forward is positive, backward is negative
		Encoder_Right=-Read_Encoder(MOTOR_ID_MR);           					//读取右轮编码器的值，前进为正，后退为负 Read the value of the right wheel encoder, forward is positive, backward is negative
		
				
		Balance_Pwm=Balance_PD(Angle_Balance,Gyro_Balance);    //平衡PID控制 Gyro_Balance平衡角速度极性：前倾为正，后倾为负 Balance PID control gyro balance angular velocity polarity: forward tilt is positive, backward tilt is negative
		Velocity_Pwm=Velocity_PI(Encoder_Left,Encoder_Right);  //速度环PID控制	记住，速度反馈是正反馈 Speed loop PID control. Remember, speed feedback is positive feedback
				
		Turn_Pwm=Turn_PD(Gyro_Turn);														//转向环PID控制    Steering loop PID control
		
		
		Motor_Left=Balance_Pwm+Velocity_Pwm+Turn_Pwm;       //计算左轮电机最终PWM Calculate the final PWM of the left wheel motor
		Motor_Right=Balance_Pwm+Velocity_Pwm-Turn_Pwm;      //计算右轮电机最终PWM Calculate the final PWM of the right wheel motor
																												//PWM值正数使小车前进，负数使小车后退  Positive PWM values cause the car to move forward, negative values cause the car to move backward
																																				
		//滤掉死区 Filter out dead zones
		Motor_Left = PWM_Ignore(Motor_Left);
		Motor_Right = PWM_Ignore(Motor_Right);
		
		//PWM限幅 PWM limiting
		Motor_Left=PWM_Limit(Motor_Left,2600,-2600); //25khz->2592 
		Motor_Right=PWM_Limit(Motor_Right,2600,-2600);		

		if(Turn_Off(Angle_Balance,battery)==0)			//如果不存在异常
		{
			if (Stop_Flag == 0)
				Set_Pwm(Motor_Left,Motor_Right);         					//赋值给PWM寄存器 
		}	
		
		
		 		
   }
	
}
