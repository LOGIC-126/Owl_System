#ifndef __SPI_H
#define __SPI_H

#include "stm32f10x.h"                  // Device header

#define SPI_SS_PORT GPIOB
#define SPI_SS_PIN  GPIO_Pin_12

#define SPI_SCK_PORT GPIOB
#define SPI_SCK_PIN  GPIO_Pin_13

#define SPI_MOSI_PORT GPIOB
#define SPI_MOSI_PIN  GPIO_Pin_15

#define SPI_MISO_PORT GPIOB
#define SPI_MISO_PIN  GPIO_Pin_14

void The_SPI_Init(void);
void SPI_Start(void);
void SPI_Stop(void);
uint8_t SPI_SwapByte(uint8_t ByteSend);

#endif
