#include "Sbus.h"

uint8_t SBUS_Packet[SBUS_DATA_NUM];
uint8_t RxFlag;

/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = SBUS_USART_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  无
  * @retval 无
  */
void SBUS_USARTConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	// 打开串口GPIO的时钟
	SBUS_USART_GPIO_APBxClkCmd(SBUS_USART_GPIO_CLK, ENABLE);
	
	// 打开串口外设的时钟
	SBUS_USART_APBxClkCmd(SBUS_USART_CLK, ENABLE);

	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = SBUS_USART_TX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SBUS_USART_TX_GPIO_PORT, &GPIO_InitStructure);

  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = SBUS_USART_RX_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SBUS_USART_RX_GPIO_PORT, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = SBUS_USART_BAUDRATE;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = SBUS_USART_WordLength;
	// 配置停止位
	USART_InitStructure.USART_StopBits = SBUS_USART_StopBits;
	// 配置校验位
	USART_InitStructure.USART_Parity = SBUS_USART_Parity ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(SBUS_USART, &USART_InitStructure);
	
	// 串口中断优先级配置
	NVIC_Configuration();
	
	// 使能串口接收中断
	USART_ITConfig(SBUS_USART, USART_IT_RXNE, ENABLE);	
	
	// 使能串口
	USART_Cmd(SBUS_USART, ENABLE);	    
}
/**
  * 函    数：获取串口接收数据包标志位
  * 参    数：无
  * 返 回 值：串口接收数据包标志位，范围：0~1，接收到数据包后，标志位置1，读取后标志位自动清零
  */
uint8_t SBUS_GetRxFlag(void)
{
	if (RxFlag == 1)			//如果标志位为1
	{
		RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}
/**
  * 函    数：USART2中断函数
  * 参    数：无
  * 返 回 值：无
  * 注意事项：此函数为中断函数，无需调用，中断触发后自动执行
  *           函数名为预留的指定名称，可以从启动文件复制
  *           请确保函数名正确，不能有任何差异，否则中断函数将不能进入
  */
void SBUS_USART_IRQHandler(void)
{
	static uint8_t RxState = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket = 0;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(SBUS_USART, USART_IT_RXNE) == SET)		//判断是否是USART2的接收事件触发的中断
	{
		uint8_t RxData = USART_ReceiveData(SBUS_USART);				//读取数据寄存器，存放在接收的数据变量
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
		if (RxState == 0)
		{
			if (RxData == 0x0F)			//如果数据确实是包头
			{
				RxState = 1;			//置下一个状态
				pRxPacket = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState == 1)
		{
			SBUS_Packet[pRxPacket] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket ++;				//数据包的位置自增
			if (pRxPacket >= SBUS_DATA_NUM)
			{
				RxState = 2;			//置下一个状态
			}
		}
		/*当前状态为2，接收数据包包尾*/
		else if (RxState == 2)
		{
			if (RxData == 0x00)			//如果数据确实是包尾部
			{
				RxState = 0;			//状态归0
				RxFlag = 1;		//接收数据包标志位置1，成功接收一个数据包
			}
		}
		
		USART_ClearITPendingBit(SBUS_USART, USART_IT_RXNE);		//清除标志位
	}
}

/**
  * 函    数：通道值解析函数
  * 参    数：CH通道值指针
  * 返 回 值：CH通道值
  */
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
					
