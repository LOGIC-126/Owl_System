#include "beep.h"

u32 beep_time = 0;

void init_beep(void)
{
	//初始化蜂鸣器的引脚
  GPIO_InitTypeDef GPIO_InitStructure;
	/*开启外设时钟*/
	RCC_APB2PeriphClockCmd(BEEP_RCC, ENABLE); 
	/*选择要控制的引脚*/															   
	GPIO_InitStructure.GPIO_Pin = BEEP_PIN;	
	/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   
	/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz; 
	/*调用库函数，初始化BEEP_PORT*/
	GPIO_Init(BEEP_PORT, &GPIO_InitStructure);
	
	BEEP_BEEP = 0;

}

//蜂鸣器打开时长
//beep_time ：1ms为单位
void open_beep(u32 beep_time) //10ms减一次
{
	beep_time = beep_time/10;
}

