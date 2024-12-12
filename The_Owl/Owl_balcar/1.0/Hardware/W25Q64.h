#ifndef __W25Q64_H
#define __W25Q64_H

#include "allconfig.h"

// 定义各控制参数的起始地址
#define Balance_Kp_ADDRESS   0x00000  // 平衡控制 Kp 参数地址
#define Balance_Kd_ADDRESS   0x00100  // 平衡控制 Kd 参数地址

#define Velocity_Kp_ADDRESS  0x00200  // 速度控制 Kp 参数地址
#define Velocity_Ki_ADDRESS  0x00300  // 速度控制 Ki 参数地址

#define Turn_Kp_ADDRESS      0x00400  // 转向控制 Kp 参数地址
#define Turn_Kd_ADDRESS      0x00500  // 转向控制 Kd 参数地址

void W25Q64_Init(void);
void W25Q64_ReadID(uint8_t *MID, uint16_t *DID);
void W25Q64_PageProgram(uint32_t Address, uint8_t *DataArray, uint16_t Count);
void W25Q64_SectorErase(uint32_t Address);
void W25Q64_ReadData(uint32_t Address, uint8_t *DataArray, uint32_t Count);

#endif
