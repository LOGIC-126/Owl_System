#ifndef __SERIAL_H
#define __SERIAL_H

#include "stm32f10x.h"                  // Device header
extern uint8_t SBUS_Packet[];

//SBUS协议data数
#define SBUS_DATA_NUM 25
// 串口2-USART2
#define  SBUS_USART                    USART3
#define  SBUS_USART_CLK                RCC_APB1Periph_USART3
#define  SBUS_USART_APBxClkCmd         RCC_APB1PeriphClockCmd
#define  SBUS_USART_BAUDRATE           100000
#define  SBUS_USART_WordLength			USART_WordLength_9b
#define  SBUS_USART_Parity				USART_Parity_Even
#define  SBUS_USART_StopBits			USART_StopBits_2
// USART GPIO 引脚宏定义
#define  SBUS_USART_GPIO_CLK           (RCC_APB2Periph_GPIOB)
#define  SBUS_USART_GPIO_APBxClkCmd    RCC_APB2PeriphClockCmd
    
#define  SBUS_USART_TX_GPIO_PORT       GPIOB   
#define  SBUS_USART_TX_GPIO_PIN        GPIO_Pin_10
#define  SBUS_USART_RX_GPIO_PORT       GPIOB
#define  SBUS_USART_RX_GPIO_PIN        GPIO_Pin_11
                                       
#define  SBUS_USART_IRQ                USART3_IRQn
#define  SBUS_USART_IRQHandler         USART3_IRQHandler

void SBUS_USARTConfig(void);
uint8_t SBUS_GetRxFlag(void);

#endif
