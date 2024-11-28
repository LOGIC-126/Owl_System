#ifndef __BSP_OLED_I2C_H_
#define __BSP_OLED_I2C_H_

#include "allconfig.h"


void OLED_I2C_Init(void);



//IIC所有操作函数
void OLED_IIC_Init(void);                  //初始化IIC的IO口				 
int OLED_IIC_Start(void);                  //发送IIC开始信号
void OLED_IIC_Stop(void);                  //发送IIC停止信号
void OLED_IIC_Send_Byte(uint8_t txd);           //IIC发送一个字节
uint8_t OLED_IIC_Read_Byte(unsigned char ack);  //IIC读取一个字节
int OLED_IIC_Wait_Ack(void);               //IIC等待ACK信号
void OLED_IIC_Ack(void);                   //IIC发送ACK信号
void OLED_IIC_NAck(void);                  //IIC不发送ACK信号

void OLED_IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t OLED_IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 
unsigned char OLED_I2C_Readkey(unsigned char I2C_Addr);

unsigned char OLED_I2C_ReadOneByte(unsigned char I2C_Addr,unsigned char addr);
unsigned char OLED_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data);
uint8_t OLED_IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t OLED_IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
uint8_t OLED_IICwriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t OLED_IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

int OLED_i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int OLED_i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


#endif

