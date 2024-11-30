

#### **关机数据：**

data1:232 data2:59 data3:223 data4:244 data5:208 data6:199 data7:93 data8:238 data9:210 data10:135 data11:62 data12:0 data13:0 data14:0 data15:0 data16:0 data17:0 data18:0 data19:0 data20:0 data21:0 data22:0 **data23:16** data24:0 data25:15 



#### 开机数据：

data1:180 data2:59 data3:159 data4:127 data5:34 data6:136 data7:62 data8:238 data9:210 data10:135 data11:62 *data12:0 data13:0 data14:0 data15:0 data16:0 data17:0 data18:0 data19:0 data20:0 data21:0 data22:0 **data23:0** data24:0* data25:15 



观察发现 data23 可能是flag
我在怀疑data24与25是否存在，比较我们已经识别了包头包尾了……



#### 此处是之前写的micropython程序：



```python
def sbusread(uart):
    
    bin_data = uart.read(25)

    # 使用列表推导式提取每个字节
    bytes_list = [byte for byte in bin_data]#原始数据表格
    bytes_list_2 = bytes_list[1:12] + [bytes_list[-2]]#因为有部分通道数据为0，故减去
    			   						#flag
    
        
    data_lists ={#字符串列表
            'data1':binarycon(bytes_list_2[0]),#相关联的通道：左测开关CH7 右摇杆左右CH4
            'data2':binarycon(bytes_list_2[1]),#左测开关CH7（？）右摇杆上下CH3注：波动
            'data3':binarycon(bytes_list_2[2]),#左摇杆上下CH1 及其混乱
            'data4':binarycon(bytes_list_2[3]),#左摇杆上下CH1 稳定
            'data5':binarycon(bytes_list_2[4]),#左摇杆左右CH2 左测开关CH7（？）
            'data6':binarycon(bytes_list_2[5]),#左摇杆左右CH2 稳定 左开关CH5
            'data7':binarycon(bytes_list_2[6]),#左开关CH5
            'data8':binarycon(bytes_list_2[7]),#右开关CH6
            'data9':binarycon(bytes_list_2[8]),#右开关CH6
            'data10':binarycon(bytes_list_2[9]),#定值不变？
            'data11':binarycon(bytes_list_2[10]),#定值不变？
            'flags':binarycon(bytes_list_2[11])#连接标志
            }
        
    
    CH_1 = int('0b'+ data_lists['data3'][2:] + data_lists['data2'][:5])#右摇杆上下
    CH_2 = int('0b'+ data_lists['data2'][5:] + data_lists['data1'])#右摇杆左右
    CH_3 = int('0b'+ data_lists['data5'][7:] + data_lists['data4'] + data_lists['data3'][:2])#左摇杆上下
    CH_4 = int('0b'+ data_lists['data6'][4:] + data_lists['data5'][:7])#左摇杆左右
    CH_5 = int('0b'+ data_lists['data7'][1:] + data_lists['data6'][:4])#左开关
    CH_6 = int('0b'+ data_lists['data9'][6:] + data_lists['data8'] + '0')#右开关
    flags = int('0b'+ data_lists['flags'])#电源开关
        
        
    return CH_1,CH_2,CH_3,CH_4,CH_5,CH_6,flags
```

```c
//			printf("data1:%d ",SBUS_Packet[0]);
//			printf("data2:%d ",SBUS_Packet[1]);
//			printf("data3:%d ",SBUS_Packet[2]);
//			printf("data4:%d ",SBUS_Packet[3]);
//			printf("data5:%d ",SBUS_Packet[4]);
//			printf("data6:%d ",SBUS_Packet[5]);
//			printf("data7:%d ",SBUS_Packet[6]);
//			printf("data8:%d ",SBUS_Packet[7]);
//			printf("data9:%d ",SBUS_Packet[8]);
//			printf("data10:%d ",SBUS_Packet[9]);
//			printf("data11:%d ",SBUS_Packet[10]);
//			printf("data12:%d ",SBUS_Packet[11]);
//			printf("data13:%d ",SBUS_Packet[12]);
//			printf("data14:%d ",SBUS_Packet[13]);
//			printf("data15:%d ",SBUS_Packet[14]);
//			printf("data16:%d ",SBUS_Packet[15]);
//			printf("data17:%d ",SBUS_Packet[16]);
//			printf("data18:%d ",SBUS_Packet[17]);
//			printf("data19:%d ",SBUS_Packet[18]);
//			printf("data20:%d ",SBUS_Packet[19]);
//			printf("data21:%d ",SBUS_Packet[20]);
//			printf("data22:%d ",SBUS_Packet[21]);
//			printf("data23: %d \n",SBUS_Packet[22]);
```



#### 最终代码：

```c
// 定义一个函数来解析数据并返回通道值
void parse_channels(uint16_t *CH_1, uint16_t *CH_2, uint16_t *CH_3, 
                    uint16_t *CH_4, uint16_t *CH_5, uint16_t *CH_6, 
                    uint8_t *flags) {
    // 辅助函数：将一个字节转换为二进制表示，并提取指定的位
    #define BINARYCON(byte, start, end) (((byte) >> (7 - (end))) & ((1 << ((end) - (start) + 1)) - 1))

    // 解析各个通道的数据
    *CH_1 = (BINARYCON(SBUS_Packet[2], 2, 7) << 5) | BINARYCON(SBUS_Packet[1], 0, 4); // 右摇杆上下
    *CH_2 = (BINARYCON(SBUS_Packet[1], 5, 7) << 8) | SBUS_Packet[0];                  // 右摇杆左右
    *CH_3 = (BINARYCON(SBUS_Packet[4], 7, 7) << 10) | (SBUS_Packet[3] << 2) | BINARYCON(SBUS_Packet[2], 0, 1); // 左摇杆上下
    *CH_4 = (BINARYCON(SBUS_Packet[5], 4, 7) << 7) | (SBUS_Packet[4] & 0x7F);           // 左摇杆左右
    *CH_5 = (BINARYCON(SBUS_Packet[6], 1, 7) << 4) | BINARYCON(SBUS_Packet[5], 0, 3); // 左开关
    *CH_6 = (BINARYCON(SBUS_Packet[8], 6, 7) << 9) | (SBUS_Packet[7] << 1);           // 右开关
    *flags = SBUS_Packet[11]; // 电源开关

    // 清除宏定义
    #undef BINARYCON
}
```

##### 示例调用代码：

```c
int main(void)
{
	uint16_t CH_1, CH_2, CH_3, CH_4, CH_5, CH_6;
    uint8_t flags;
	USB_USARTConfig();
	SBUS_USARTConfig();
	while(1)
	{	
		if (SBUS_GetRxFlag()==1)
		{
			parse_channels(&CH_1, &CH_2, &CH_3, &CH_4, &CH_5, &CH_6, &flags);
			printf("CH1: %d \n",CH_1);
			Delay_ms(10);
		}
	}	
}

```

