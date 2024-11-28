#include "bsp_oled_i2c.h"


//IO方向设置
#define OLED_SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)8<<4;}
#define OLED_SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)3<<4;}

//IO操作函数	 
#define OLED_IIC_SCL    PBout(8) //SCL
#define OLED_IIC_SDA    PBout(9) //SDA	 
#define OLED_READ_SDA   PBin(9)  //输入SDA 

// 等待引脚的时间，可根据芯片时钟修改，只要符合通讯要求即可。
#define DELAY_FOR_COUNT      10


static void Delay_For_Pin(vu8 nCount) 
{
    uint8_t i = 0;
    for(; nCount != 0; nCount--)
    {
        for (i = 0; i < DELAY_FOR_COUNT; i++); 
    }
}


void OLED_I2C_Init(void)
{	
		OLED_IIC_Init();
		OLED_Init();//oled初始化
	
		OLED_Draw_Line("oled init success!", 1, true, true);
}


void OLED_IIC_Init(void)
{
    RCC->APB2ENR |= 1 << 3;   //先使能外设IO PORTB时钟
    GPIOB->CRH &= 0XFFFFFF00; //PB 8/9 推挽输出
    GPIOB->CRH |= 0X00000033;
}

/**
 * @Brief: 产生IIC起始信号
 * @Note:
 * @Parm: void
 * @Retval: void
 */
int OLED_IIC_Start(void)
{
    OLED_SDA_OUT(); //sda线输出
    OLED_IIC_SDA = 1;
    if (!OLED_READ_SDA)
        return 0;
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SDA = 0; //START:when CLK is high,DATA change form high to low
    if (OLED_READ_SDA)
        return 0;
    Delay_For_Pin(2);
    OLED_IIC_SCL = 0; //钳住I2C总线，准备发送或接收数据
    return 1;
}

/**
 * @Brief: 产生IIC停止信号
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_Stop(void)
{
    OLED_SDA_OUT(); //sda线输出
    OLED_IIC_SCL = 0;
    OLED_IIC_SDA = 0; //STOP:when CLK is high DATA change form low to high
    Delay_For_Pin(2);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SDA = 1; //发送I2C总线结束信号
    Delay_For_Pin(2);
}

/**
 * @Brief: 等待应答信号到来
 * @Note:
 * @Parm:
 * @Retval: 1:接收应答成功 | 0:接收应答失败
 */
int OLED_IIC_Wait_Ack(void)
{
    uint8_t ucErrTime = 0;
    OLED_SDA_IN(); //SDA设置为输入
    OLED_IIC_SDA = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    while (OLED_READ_SDA)
    {
        ucErrTime++;
        if (ucErrTime > 50)
        {
            OLED_IIC_Stop();
            return 0;
        }
        Delay_For_Pin(1);
    }
    OLED_IIC_SCL = 0; //时钟输出0
    return 1;
}

/**
 * @Brief: 产生ACK应答
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_Ack(void)
{
    OLED_IIC_SCL = 0;
    OLED_SDA_OUT();
    OLED_IIC_SDA = 0;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 0;
}

/**
 * @Brief: 产生NACK应答
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_NAck(void)
{
    OLED_IIC_SCL = 0;
    OLED_SDA_OUT();
    OLED_IIC_SDA = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 1;
    Delay_For_Pin(1);
    OLED_IIC_SCL = 0;
}

/**
 * @Brief: IIC发送一个字节
 * @Note:
 * @Parm: void
 * @Retval: void
 */
void OLED_IIC_Send_Byte(uint8_t txd)
{
    uint8_t t;
    OLED_SDA_OUT();
    OLED_IIC_SCL = 0; //拉低时钟开始数据传输
    for (t = 0; t < 8; t++)
    {
        OLED_IIC_SDA = (txd & 0x80) >> 7;
        txd <<= 1;
        Delay_For_Pin(1);
        OLED_IIC_SCL = 1;
        Delay_For_Pin(1);
        OLED_IIC_SCL = 0;
        Delay_For_Pin(1);
    }
}


/**
 * @Brief: I2C写数据函数
 * @Note:
 * @Parm: addr:I2C地址 | reg:寄存器 | len:数据长度 | data:数据指针
 * @Retval: 0:停止 | 1:连续写
 */
int OLED_i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data)
{
    int i;
    if (!OLED_IIC_Start())
        return 1;
    OLED_IIC_Send_Byte(addr << 1);
    if (!OLED_IIC_Wait_Ack())
    {
        OLED_IIC_Stop();
        return 1;
    }
    OLED_IIC_Send_Byte(reg);
    OLED_IIC_Wait_Ack();
    for (i = 0; i < len; i++)
    {
        OLED_IIC_Send_Byte(data[i]);
        if (!OLED_IIC_Wait_Ack())
        {
            OLED_IIC_Stop();
            return 0;
        }
    }
    OLED_IIC_Stop();
    return 0;
}

/**
 * @Brief: I2C读函数
 * @Note:
 * @Parm: 参数同写类似
 * @Retval:
 */
int OLED_i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf)
{
    if (!OLED_IIC_Start())
        return 1;
    OLED_IIC_Send_Byte(addr << 1);
    if (!OLED_IIC_Wait_Ack())
    {
        OLED_IIC_Stop();
        return 1;
    }
    OLED_IIC_Send_Byte(reg);
    OLED_IIC_Wait_Ack();
    OLED_IIC_Start();
    OLED_IIC_Send_Byte((addr << 1) + 1);
    OLED_IIC_Wait_Ack();
    while (len)
    {
        if (len == 1)
            *buf = OLED_IIC_Read_Byte(0);
        else
            *buf = OLED_IIC_Read_Byte(1);
        buf++;
        len--;
    }
    OLED_IIC_Stop();
    return 0;
}

/**
 * @Brief: 读1个字节，ack=1时，发送ACK，ack=0，发送nACK
 * @Note:
 * @Parm:
 * @Retval:
 */
uint8_t OLED_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i, receive = 0;
    OLED_SDA_IN(); //SDA设置为输入
    for (i = 0; i < 8; i++)
    {
        OLED_IIC_SCL = 0;
        Delay_For_Pin(2);
        OLED_IIC_SCL = 1;
        receive <<= 1;
        if (OLED_READ_SDA)
            receive++;
        Delay_For_Pin(2);
    }
    if (ack)
        OLED_IIC_Ack(); //发送ACK
    else
        OLED_IIC_NAck(); //发送nACK
    return receive;
}

/**
 * @Brief: 读取指定设备 指定寄存器的一个值
 * @Note: 
 * @Parm: I2C_Addr  目标设备地址 | addr     寄存器地址
 * @Retval: 
 */
unsigned char OLED_I2C_ReadOneByte(unsigned char I2C_Addr, unsigned char addr)
{
    unsigned char res = 0;

    OLED_IIC_Start();
    OLED_IIC_Send_Byte(I2C_Addr); //发送写命令
    res++;
    OLED_IIC_Wait_Ack();
    OLED_IIC_Send_Byte(addr);
    res++; //发送地址
    OLED_IIC_Wait_Ack();
	
    OLED_IIC_Start();
    OLED_IIC_Send_Byte(I2C_Addr + 1);
    res++; //进入接收模式
    OLED_IIC_Wait_Ack();
    res = OLED_IIC_Read_Byte(0);
    OLED_IIC_Stop(); //产生一个停止条件

    return res;
}

/**
 * @Brief: 读取指定设备 指定寄存器的 length个值
 * @Note: 
 * @Parm: dev  目标设备地址 | reg   寄存器地址 | length 要读的字节数 | *data  读出的数据将要存放的指针
 * @Retval: 读出来的字节数量
 */
uint8_t OLED_OLED_IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{
    uint8_t count = 0;

    OLED_IIC_Start();
    OLED_IIC_Send_Byte(dev); //发送写命令
    OLED_IIC_Wait_Ack();
    OLED_IIC_Send_Byte(reg); //发送地址
    OLED_IIC_Wait_Ack();
    OLED_IIC_Start();
    OLED_IIC_Send_Byte(dev + 1); //进入接收模式
    OLED_IIC_Wait_Ack();

    for (count = 0; count < length; count++)
    {

        if (count != length - 1)
            data[count] = OLED_IIC_Read_Byte(1); //带ACK的读数据
        else
            data[count] = OLED_IIC_Read_Byte(0); //最后一个字节NACK
    }
    OLED_IIC_Stop(); //产生一个停止条件
    return count;
}

/**
 * @Brief: 将多个字节写入指定设备 指定寄存器
 * @Note: 
 * @Parm: dev  目标设备地址 | reg   寄存器地址 | length 要写的字节数 | *data  将要写的数据的首地址
 * @Retval: 返回是否成功
 */
uint8_t OLED_OLED_IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data)
{

    uint8_t count = 0;
    OLED_IIC_Start();
    OLED_IIC_Send_Byte(dev); //发送写命令
    OLED_IIC_Wait_Ack();
    OLED_IIC_Send_Byte(reg); //发送地址
    OLED_IIC_Wait_Ack();
    for (count = 0; count < length; count++)
    {
        OLED_IIC_Send_Byte(data[count]);
        OLED_IIC_Wait_Ack();
    }
    OLED_IIC_Stop(); //产生一个停止条件

    return 1; //status == 0;
}

/**
 * @Brief: 读取指定设备 指定寄存器的一个值
 * @Note: 
 * @Parm: dev  目标设备地址 | reg    寄存器地址 | *data  读出的数据将要存放的地址
 * @Retval: 1
 */
uint8_t OLED_IICreadByte(uint8_t dev, uint8_t reg, uint8_t *data)
{
    *data = OLED_I2C_ReadOneByte(dev, reg);
    return 1;
}

/**
 * @Brief: 写入指定设备 指定寄存器一个字节
 * @Note: 
 * @Parm: dev  目标设备地址 | reg    寄存器地址 | data  将要写入的字节
 * @Retval: 1
 */
unsigned char OLED_IICwriteByte(unsigned char dev, unsigned char reg, unsigned char data)
{
    return OLED_OLED_IICwriteBytes(dev, reg, 1, &data);
}

/**
 * @Brief: 读 修改 写 指定设备 指定寄存器一个字节 中的多个位
 * @Note: 
 * @Parm: dev  目标设备地址 | reg    寄存器地址 | bitStart  目标字节的起始位 | length   位长度 | data    存放改变目标字节位的值
 * @Retval: 1:成功 | 0:失败
 */
uint8_t OLED_OLED_IICwriteBits(uint8_t dev, uint8_t reg, uint8_t bitStart, uint8_t length, uint8_t data)
{

    uint8_t b;
    if (OLED_IICreadByte(dev, reg, &b) != 0)
    {
        uint8_t mask = (0xFF << (bitStart + 1)) | 0xFF >> ((8 - bitStart) + length - 1);
        data <<= (8 - length);
        data >>= (7 - bitStart);
        b &= mask;
        b |= data;
        return OLED_IICwriteByte(dev, reg, b);
    }
    else
    {
        return 0;
    }
}

/**
 * @Brief: 读 修改 写 指定设备 指定寄存器一个字节 中的1个位
 * @Note: 
 * @Parm: dev  目标设备地址 | reg    寄存器地址 | bitNum  要修改目标字节的bitNum位 | data  为0 时，目标位将被清0 否则将被置位
 * @Retval: 1:成功 | 0:失败
 */
uint8_t OLED_IICwriteBit(uint8_t dev, uint8_t reg, uint8_t bitNum, uint8_t data)
{
    uint8_t b;
    OLED_IICreadByte(dev, reg, &b);
    b = (data != 0) ? (b | (1 << bitNum)) : (b & ~(1 << bitNum));
    return OLED_IICwriteByte(dev, reg, b);
}

