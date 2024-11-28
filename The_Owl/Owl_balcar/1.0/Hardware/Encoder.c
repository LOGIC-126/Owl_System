#include "Encoder.h"

/**************************************************************************
Function: Initialize TIM3 to encoder interface mode
Input   : none
Output  : none
函数功能：把TIM3初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM3(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
	TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);//使能定时器3的时钟 Enable the clock of Timer 3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能PA端口时钟  Enable PA port clock
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置  port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入  Floating input
	GPIO_Init(GPIOA, &GPIO_InitStructure);					      //根据设定参数初始化GPIOA  Init GPIOA
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0; // 预分频器  Prescaler
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值 Set the automatic reset value of the counter
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频 Choose clock division: no division
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数    TIM up counting
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3 Use encoder mode 3

	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;	//滤波10 Filter 10
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位 Clear the update flag of TIM
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM3,0);
	TIM_Cmd(TIM3, ENABLE); 
}


/**************************************************************************
Function: Initialize TIM4 to encoder interface mode
Input   : none
Output  : none
函数功能：把TIM4初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM4(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;  
    TIM_ICInitTypeDef TIM_ICInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//使能定时器4的时钟  Enable the clock of Timer 4
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能PB端口时钟    Enable PB port clock
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;	//端口配置   			port configuration
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入   			Floating input
	GPIO_Init(GPIOB, &GPIO_InitStructure);					      //根据设定参数初始化GPIOB  Init GPIOB
  
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器  Prescaler
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD; //设定计数器自动重装值 Set the automatic reset value of the counter
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频 Choose clock division: no division
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数   TIM up counting
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3 Use encoder mode 3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);//清除TIM的更新标志位  Clear the update flag of TIM
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//Reset counter
	TIM_SetCounter(TIM4,0);
	TIM_Cmd(TIM4, ENABLE); 
}


/**************************************************************************
Function: Read encoder count per unit time
Input   : TIMX：Timer
Output  : none
函数功能：单位时间读取编码器计数
入口参数：TIMX：定时器
返回  值：速度值
**************************************************************************/
int Read_Encoder(Motor_ID MYTIMX)
{
   int Encoder_TIM;    
   switch(MYTIMX)
	 {
		 case MOTOR_ID_ML:  Encoder_TIM= (short)TIM3 -> CNT;  TIM3 -> CNT=0;break;	
		 case MOTOR_ID_MR:  Encoder_TIM= (short)TIM4 -> CNT;  TIM4 -> CNT=0;break;	
		 default: Encoder_TIM=0;
	 }
		return Encoder_TIM;
}


/**************************************************************************
Function: TIM4 interrupt service function
Input   : none
Output  : none
函数功能：TIM4中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//溢出中断 Overflow interrupt
	{    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//清除中断标志位 	Clear interrupt flag bit    
}
/**************************************************************************
Function: TIM2 interrupt service function
Input   : none
Output  : none
函数功能：TIM2中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM3_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断 Overflow interrupt
	{    				   				     	    	
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	   Clear interrupt flag bit   
}



