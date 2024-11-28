#include "Timer2.h"

void Timer_Init(uint32_t frequency)
{
    // 系统时钟频率（假设为72MHz）
    uint32_t SystemCoreClock = 72000000;

    // 计算预分频值和计数值
    uint16_t prescaler = (SystemCoreClock / 1000000) - 1;  // 预分频值，使得定时器时钟为1MHz
    uint16_t period = (1000000 / frequency) - 1;            // 计数值，使得定时器每1/frequency秒触发一次中断

    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 配置时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = period;           // 计数周期
    TIM_TimeBaseInitStructure.TIM_Prescaler = prescaler;     // 预分频器
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 时钟分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    // 清除定时器更新标志位
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);

    // 开启TIM2的更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // NVIC中断分组配置
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    // NVIC配置
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
	
	// 使能TIM2，定时器开始运行
    TIM_Cmd(TIM2, ENABLE);
}

void Timer_Stop(void)
{
    // 禁用TIM2，定时器停止运行
    TIM_Cmd(TIM2, DISABLE);
}

//复制到调用的地方
/*
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        // 在这里添加您的中断处理代码


        // 清除中断标志位
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
*/
