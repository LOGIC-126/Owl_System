#include "stm32f10x.h"                  // Device header
#include "allconfig.h"

void All_config(void)
{
	NVIC_Config();
	delay_init();
	OLED_I2C_Init();							//oled初始化 OLED initialization
	USB_USARTConfig();
	W25Q64_Init();
	Key1_GPIO_Init();
	Balance_Motor_Init();
	Balance_PWM_Init(2880,0);
	Encoder_Init_TIM3();
	Encoder_Init_TIM4(); 
	delay_ms(300);			
	IIC_MPU6050_Init();							//陀螺仪i2c初始化   Gyroscope I2C initialization
	MPU6050_initialize();						//陀螺仪量程初始化  Gyroscope range initialization
	DMP_Init();                     //DMP初始化    DMP initialization	
	

	
	MPU6050_EXTI_Init();			//一定，他妈的，要放最后，妈的我查了老半天优先级的问题
	printf("----------------------------------------\n");
	printf("Owl Balcar 1.0 Has Been Completed!\n");
	printf("----------------------------------------\n");
	printf("Now You can give it command in < > \n");
	printf("----------------------------------------\n");
}



/*
  ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================
*/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//4级抢占,4级响应。
	
	//外部中断
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;		
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x03;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;		
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				
  	NVIC_Init(&NVIC_InitStructure); 

  
	NVIC_InitStructure.NVIC_IRQChannel = DEBUG_USART_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure); 
	
} 
//------------------End of File----------------------------
