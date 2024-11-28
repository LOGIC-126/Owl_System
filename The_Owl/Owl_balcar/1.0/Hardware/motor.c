#include "motor.h"

float Velocity_Left,Velocity_Right;	//车轮速度(mm/s)
uint8_t angle_max = 40; 

/**************************************************************************
Function: Absolute value function 
Input   : a：Number to be converted
Output  : unsigned int
函数功能：绝对值函数
入口参数：a：需要计算绝对值的数
返回  值：无符号整型
**************************************************************************/	
int myabs(int a)
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a; 
	return temp;
}

/**************************************************************************
Function: Initialize Motor Interface
Input   : none
Output  : none
函数功能：初始化电机接口
入口参数：无
返回  值：无
**************************************************************************/
void Balance_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //使能PC端口时钟 Enable PC port clock
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;	//端口配置 port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;      //推挽输出 Push pull output
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;     //50M
	GPIO_Init(GPIOC, &GPIO_InitStructure);					      //根据设定参数初始化GPIOC Initialize GPOC according to the set parameters
}



/**************************************************************************
Function: Initialize PWM to drive motor
Input   : arr：Auto reload value；psc：Prescaler coefficient
Output  : none
函数功能：初始化PWM，用于驱动电机 
入口参数：arr：自动重装值；psc：预分频系数
返回  值：无
**************************************************************************/
void Balance_PWM_Init(u16 arr,u16 psc)//BalanceCar_PWM_Init(2880,0); 
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
	TIM_DeInit(TIM8);
	TIM_TimeBaseStructure.TIM_Period = arr - 1; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	 Set the value of the automatic reload register cycle for the next update event loading activity
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值  不分频 设置用来作为TIMx时钟频率除数的预分频值  不分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim Set clock division: TDTS=Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式 TIM Up Counting Mode
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位  Initialize the time base unit of TIMx based on the parameters specified in TIM_TimeBaseInitStruct

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式1  Select Timer Mode: TIM Pulse Width Modulation Mode 1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能  Compare output enablement
	TIM_OCInitStructure.TIM_Pulse = 0;                            //设置待装入捕获比较寄存器的脉冲值  Set the pulse value to be loaded into the capture comparison register
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //输出极性:TIM输出比较极性高  Output polarity: TIM output has a relatively high polarity
	
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //根据TIM_OCInitStruct中指定的参数初始化外设TIMx  Initialize peripheral TIMx based on the parameters specified in TIM_SICInitStruct


	TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE 主输出使能  Main output enable

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1预装载使能	  CH1 pre load enable
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH2预装载使能    CH2 pre load enable
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH3预装载使能	  CH3 pre load enable
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4预装载使能    CH4 pre load enable
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //使能TIMx在ARR上的预装载寄存器   Enable TIMx pre loaded registers on ARR
	
	
	/* TIM8 enable counter */
	TIM_Cmd(TIM8, ENABLE);                   //使能定时器8	Enable Timer 8

}

//过滤死区 Filter dead zone
int PWM_Ignore(int pulse)
{
	if (pulse > 0) return pulse + MOTOR_IGNORE_PULSE;
  if (pulse < 0) return pulse - MOTOR_IGNORE_PULSE;
	return pulse;
}

/**************************************************************************
Function: Assign to PWM register
Input   : motor_left：Left wheel PWM；motor_right：Right wheel PWM
Output  : none
函数功能：赋值给PWM寄存器
入口参数：左轮PWM、右轮PWM
返回  值：无
**************************************************************************/
void Set_Pwm(int motor_left,int motor_right)
{
	if(motor_left == 0)//停车
	{
		L_PWMA = 0;
		L_PWMB = 0;
	}
	if(motor_right == 0)
	{
		R_PWMA = 0;
		R_PWMB = 0;
	}
	
	//左轮
  if(motor_left>0)	 //前进   
	{
		L_PWMB = myabs(motor_left);
		L_PWMA = 0;
	}		
	else
	{
		L_PWMB = 0;
		L_PWMA = myabs(motor_left);
	}
	
	//右轮
	if(motor_right>0) //前进
	{
		R_PWMA = myabs(motor_right);
		R_PWMB = 0;
	}
	else //后退
	{
		R_PWMA = 0;
		R_PWMB = myabs(motor_right);	
	}

}


/**************************************************************************
Function: PWM limiting range
Input   : IN：Input  max：Maximum value  min：Minimum value
Output  : Output
函数功能：限制PWM赋值 
入口参数：IN：输入参数  max：限幅最大值  min：限幅最小值
返回  值：限幅后的值
**************************************************************************/
int PWM_Limit(int IN,int max,int min)
{
	int OUT = IN;
	if(OUT>max) OUT = max;
	if(OUT<min) OUT = min;
	return OUT;
}


/**************************************************************************
Function: Encoder reading is converted to speed (mm/s)
Input   : none
Output  : none
函数功能：编码器读数转换为速度（mm/s）
入口参数：无
返回  值：无
**************************************************************************/
void Get_Velocity_Form_Encoder(int encoder_left,int encoder_right)
{ 	
	float Rotation_Speed_L,Rotation_Speed_R;						//电机转速  转速=编码器读数（5ms每次）*读取频率/倍频数/减速比/编码器精度 //Motor speed=Encoder reading (5ms each time) * Reading frequency/harmonics/reduction ratio/Encoder accuracy
	Rotation_Speed_L = encoder_left*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
	Velocity_Left = Rotation_Speed_L*PI*Diameter_67/10;		//求出编码器速度=转速*周长 /10换成cm //Calculate the encoder speed=rotational speed * circumference/10 and convert it to cm
	Rotation_Speed_R = encoder_right*Control_Frequency/EncoderMultiples/Reduction_Ratio/Encoder_precision;
	Velocity_Right = Rotation_Speed_R*PI*Diameter_67/10;		//求出编码器速度=转速*周长 /10换成cm //Calculate the encoder speed=rotational speed * circumference/10 and convert it to cm
	
}



/**************************************************************************
Function: If abnormal, turn off the motor
Input   : angle：Car inclination；voltage：Voltage
Output  : 1：abnormal；0：normal
函数功能：异常关闭电机		
入口参数：angle：小车倾角；voltage：电压
返回  值：1：异常  0：正常
**************************************************************************/	
uint8_t Turn_Off(float angle, float voltage)
{
	u8 temp;
	if(angle<-40||angle>angle_max || battery<9.6 || Stop_Flag==1)//电池电压低于10V关闭电机 || battery<9.6V
	{	                                                 //倾角大于40度关闭电机 //Turn off the motor if the inclination angle is greater than 40 degrees
		temp=1;                                          
		L_PWMA = 0;
		L_PWMB = 0;
		R_PWMA = 0;
		R_PWMB = 0;
	}
	else
		temp=0;
	return temp;			
}





