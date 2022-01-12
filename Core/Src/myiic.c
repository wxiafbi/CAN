//i2c.c
#include "myiic.h"
#include "main.h"
 
static void i2c_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 40; i++);
}
//开始信号
void i2c_Start(void)
{
	// 当SCL高电平时，SDA出现一个下跳沿表示I2C总线启动信号 
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}
 
//停止信号
void i2c_Stop(void)
{
	// 当SCL高电平时，SDA出现一个上跳沿表示I2C总线停止信号 
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
	i2c_Delay();
}
//发送一个字节
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;
	
	// 先发送字节的高位bit7 
	for (i = 0; i < 8; i++)
	{
		if (_ucByte & 0x80)
		{
			I2C_SDA_1();
		}
		else
		{
			I2C_SDA_0();
		}
		i2c_Delay();
		I2C_SCL_1();
		i2c_Delay();
		I2C_SCL_0();
		if (i == 7)
		{
			 I2C_SDA_1(); 	// 释放总线
		}
		_ucByte <<= 1;		// 左移一个bit 
		i2c_Delay();
	}
}
//读取一个字节
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	
	/* 读到第1个bit为数据的bit7 */
	value = 0;
	for (i = 0; i < 8; i++)
	{
		value <<= 1;
		I2C_SCL_1();
		i2c_Delay();
		if (I2C_SDA_READ())
		{
			value++;
		}
		I2C_SCL_0();
		i2c_Delay();
	}
	return value;
}
 
//等待应答信号
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
 
	I2C_SDA_1();	/* CPU释放SDA总线 */
//	i2c_Delay();
	I2C_SCL_1();	/* CPU驱动SCL = 1, 此时器件会返回ACK应答 */
	i2c_Delay();
 
	if (I2C_SDA_READ())	/* CPU读取SDA口线状态 */
	{
		re = 1;
	}
	else
	{
		re = 0;
	}
 
	I2C_SCL_0();
	i2c_Delay();
	return re;
}
//应答信号
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU驱动SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU产生1个时钟 */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU释放SDA总线 */
}
 
 
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
 
	if (I2C_SDA_READ() && I2C_SCL_READ())
	{
		i2c_Start();		/* 发送启动信号 */
 
		/* 发送设备地址+读写控制bit（0 = w， 1 = r) bit7 先传 */
		i2c_SendByte(_Address | 0);
		ucAck = i2c_WaitAck();	/* 检测设备的ACK应答 */
 
		i2c_Stop();			/* 发送停止信号 */
 
		return ucAck;
	}
	return 1;	/* I2C总线异常 */
}
 
 
void  Device_WriteData(uint8_t DeciveAddr,uint8_t *Data,int size)
{
    int count=size;
		uint8_t *pData=Data;
		//起始信号
    i2c_Start();    
    //发送器件地址                         
    i2c_SendByte(DeciveAddr);       
    //等待应答
    i2c_WaitAck();                          
    while(count--)
		{
			//发送数据
			i2c_SendByte(*pData++);                     
			//等待应答
			i2c_WaitAck();   
		}			 
                       
    //结束信号
    i2c_Stop();     
}