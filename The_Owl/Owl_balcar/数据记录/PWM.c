#include "stm32f10x.h"                  // Device header
#include "PWM.h"

// 计算预分频器和周期值
void CalculatePWMParameters(uint32_t SystemCoreClock, float Frequency, uint16_t* Prescaler, uint16_t* Period)
{
    // 计算预分频器值
    *Prescaler = (SystemCoreClock / (Frequency * 65536)) - 1; // 65536 是 TIM1 的最大计数值
    if (*Prescaler > 65535) {
        *Prescaler = 65535; // 预分频器的最大值
    }

    // 计算周期值
    *Period = (SystemCoreClock / (Frequency * (*Prescaler + 1))) - 1;
    if (*Period > 65535) {
        *Period = 65535; // 周期的最大值
    }
}

// PWM初始化函数
void PWM_Init(The_GPIO_Config gpioConfig,float Frequency)
{
    // 开启TIM1和指定GPIO的时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_GPIOA, ENABLE);
	
    // 初始化GPIO
	GPIO_Config(gpioConfig);
	
	// 计算预分频器和周期值
    uint16_t Prescaler, Period;
    CalculatePWMParameters(SystemCoreClock, Frequency, &Prescaler, &Period);

    // 配置TIM1时基单元
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period = Period;          // 计数周期，这里设置为1000个计数周期
    TIM_TimeBaseInitStructure.TIM_Prescaler = Prescaler;      // 预分频器，假设系统时钟为72MHz，则TIM1时钟频率为10kHz
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);

    // 配置输出比较通道1
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  // 初始脉冲宽度为0
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    // 使能TIM1
    TIM_Cmd(TIM1, ENABLE);
}

// 设置PWM占空比函数
void PWM_SetCompare1(uint16_t DutyCycle)
{
    uint16_t compareValue = (uint16_t)((float)DutyCycle / 1000 * (TIM1->ARR + 1));
    TIM_SetCompare1(TIM1, compareValue);
}



