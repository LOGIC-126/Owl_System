#include "Key1.h"

void Key1_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK, ENABLE);
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; 
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
}

void EXTI9_5_IRQHandler(void)
{
     if(KEY_INT==0)     
    {   
        EXTI->PR=1<<8; // 清除中断标志位
        Stop_Flag = !Stop_Flag;
    }
}

