//i2c.c
#include "myiic.h"
#include "main.h"
 
static void i2c_Delay(void)
{
	uint8_t i;
	for (i = 0; i < 40; i++);
}
//��ʼ�ź�
void i2c_Start(void)
{
	// ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C���������ź� 
	I2C_SDA_1();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_0();
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
}
 
//ֹͣ�ź�
void i2c_Stop(void)
{
	// ��SCL�ߵ�ƽʱ��SDA����һ�������ر�ʾI2C����ֹͣ�ź� 
	I2C_SDA_0();
	I2C_SCL_1();
	i2c_Delay();
	I2C_SDA_1();
	i2c_Delay();
}
//����һ���ֽ�
void i2c_SendByte(uint8_t _ucByte)
{
	uint8_t i;
	
	// �ȷ����ֽڵĸ�λbit7 
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
			 I2C_SDA_1(); 	// �ͷ�����
		}
		_ucByte <<= 1;		// ����һ��bit 
		i2c_Delay();
	}
}
//��ȡһ���ֽ�
uint8_t i2c_ReadByte(void)
{
	uint8_t i;
	uint8_t value;
	
	/* ������1��bitΪ���ݵ�bit7 */
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
 
//�ȴ�Ӧ���ź�
uint8_t i2c_WaitAck(void)
{
	uint8_t re;
 
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
//	i2c_Delay();
	I2C_SCL_1();	/* CPU����SCL = 1, ��ʱ�����᷵��ACKӦ�� */
	i2c_Delay();
 
	if (I2C_SDA_READ())	/* CPU��ȡSDA����״̬ */
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
//Ӧ���ź�
void i2c_Ack(void)
{
	I2C_SDA_0();	/* CPU����SDA = 0 */
	i2c_Delay();
	I2C_SCL_1();	/* CPU����1��ʱ�� */
	i2c_Delay();
	I2C_SCL_0();
	i2c_Delay();
	I2C_SDA_1();	/* CPU�ͷ�SDA���� */
}
 
 
uint8_t i2c_CheckDevice(uint8_t _Address)
{
	uint8_t ucAck;
 
	if (I2C_SDA_READ() && I2C_SCL_READ())
	{
		i2c_Start();		/* ���������ź� */
 
		/* �����豸��ַ+��д����bit��0 = w�� 1 = r) bit7 �ȴ� */
		i2c_SendByte(_Address | 0);
		ucAck = i2c_WaitAck();	/* ����豸��ACKӦ�� */
 
		i2c_Stop();			/* ����ֹͣ�ź� */
 
		return ucAck;
	}
	return 1;	/* I2C�����쳣 */
}
 
 
void  Device_WriteData(uint8_t DeciveAddr,uint8_t *Data,int size)
{
    int count=size;
		uint8_t *pData=Data;
		//��ʼ�ź�
    i2c_Start();    
    //����������ַ                         
    i2c_SendByte(DeciveAddr);       
    //�ȴ�Ӧ��
    i2c_WaitAck();                          
    while(count--)
		{
			//��������
			i2c_SendByte(*pData++);                     
			//�ȴ�Ӧ��
			i2c_WaitAck();   
		}			 
                       
    //�����ź�
    i2c_Stop();     
}