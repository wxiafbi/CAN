#include "ATH10.h"
#include "myiic.h"
#include "main.h"

/*-------------------------------------------------*/
/*?????????��AHT10��????��?????��??                    */
/*??  ??????                                       */
/*��?????????                                       */
/*-------------------------------------------------*/
void AHT10_Reset(void)
{
	i2c_Start();                    //IIC????????
	i2c_SendByte(0x70);            //��????��?????��+????����	   	
	i2c_WaitAck();                 //????????
    i2c_SendByte(0xBA);            //��????????��?? 
	i2c_WaitAck();	                //???????? 
    i2c_Stop();                     //????????
}
/*-------------------------------------------------*/
/*?????????��AHT10��??????????????????��??            */
/*??  ??????                                       */
/*��?????????                                       */
/*-------------------------------------------------*/
void AHT10_ReadData(void) 
{
	i2c_Start();                    //IIC????????
	i2c_SendByte(0x70);            //��????��?????��+????����	   	
	i2c_WaitAck();                 //????????
    i2c_SendByte(0xAC);            //��????��??
	i2c_WaitAck();	                //???????? 
	i2c_SendByte(0x33);            //��???????	   	
	i2c_WaitAck();                 //????????
    i2c_SendByte(0x00);            //��????��?????????????�� 
	i2c_WaitAck();	                //???????? 
    i2c_Stop();                     //????????
}
/*-------------------------------------------------*/
/*????????????AHT10??��????????��                    */
/*??  ??????                                       */
/*��????????????��??                                 */
/*-------------------------------------------------*/
char AHT10_ReadStatus(void)
{
	char Status;
	
	i2c_Start();                    //IIC????????
	i2c_SendByte(0x71);            //��????��?????��+????����
	i2c_WaitAck();	                //???????? 
	Status = IIC_Read_Byte(0);      //????��???????????��???ACK???? 
    i2c_Stop();                     //????????

	return Status;                  //��???��????????��????
}
/*-------------------------------------------------*/
/*?????????��???�����????????��??????��??��?             */
/*??  ??????                                       */
/*��???????0???�� 1?���?                              */
/*-------------------------------------------------*/
char AHT10_Read_CalEnable(void)  
{
	char val = 0;
 
	val = AHT10_ReadStatus();  //????��????????��
	if((val & 0x68)==0x08)     //?????�����????????��???????��????��
		return 0;              //???����???0
	else  
		return 1;              //?���?��???1
 }
/*-------------------------------------------------*/
/*??????????????AHT10                              */
/*??  ??????                                       */
/*��?????????                                       */
/*-------------------------------------------------*/
char AHT10_Init(void)   
{	   
	char times=0;
	
	i2c_Start();                    //IIC????????
	i2c_SendByte(0x70);            //��????��?????��+????����	   	
	i2c_WaitAck();                 //????????
    i2c_SendByte(0xE1);            //��??????????��?? 
	i2c_WaitAck();	                //???????? 
	i2c_SendByte(0x08);            //��???????	   	
	i2c_WaitAck();                 //????????
    i2c_SendByte(0x00);            //��???????
	i2c_WaitAck();	                //???????? 
    i2c_Stop();                     //????????
	HAL_Delay(500);                  //???��
	
	while(AHT10_Read_CalEnable()==1)   //?��???�����????????��??????��??��? 
	{   
		AHT10_Reset();                 //????AHT10
		HAL_Delay(100);                 //???��

		i2c_SendByte(0x70);           //��????��?????��+????����	   	
		i2c_WaitAck();                //????????
		i2c_SendByte(0xE1);           //��??????????��?? 
		i2c_WaitAck();	               //???????? 
		i2c_SendByte(0x08);           //��???????	   	
		i2c_WaitAck();                //????????
		i2c_SendByte(0x00);           //��???????
		i2c_WaitAck();	               //???????? 
		i2c_Stop();                    //????????

		times++;	                   //????????+1
		HAL_Delay(500);                 //???��
		if(times>=10)return 1;         //???????��????10?????���?��???1
	}
	return 0;                          //???����???0
}
/*-------------------------------------------------*/
/*???????????????��????                             */
/*??  ????temp ��???????????                        */
/*??  ????humi ��???????????                        */
/*��?????????????????                               */
/*-------------------------------------------------*/
void AHT10_Data(double *temp, double *humi)
{				  		
	int     tdata=0;
	char    Redata[6];
		
	AHT10_ReadData();                 //??????
	HAL_Delay(500);                    //????	
	i2c_Start();                      //IIC????????
	i2c_SendByte(0x71);              //��????��?????��+????����				   
	i2c_WaitAck();	                  //????????
	Redata[0]=IIC_Read_Byte(1);       //????��?????????��???ACK???? 
	Redata[1]=IIC_Read_Byte(1);       //????��?????????��???ACK???? 
	Redata[2]=IIC_Read_Byte(1);       //????��?????????��???ACK???? 
	Redata[3]=IIC_Read_Byte(1);       //????��?????????��???ACK???? 
	Redata[4]=IIC_Read_Byte(1);       //????��?????????��???ACK????
	Redata[5]=IIC_Read_Byte(0);       //????��???????????��???ACK???? 	
    i2c_Stop();                       //????????	
	HAL_Delay(5);                      //????
	
	tdata = (tdata|Redata[1])<<8;
	tdata = (tdata|Redata[2])<<8;
	tdata = (tdata|Redata[3]);
	tdata = (tdata >>4);
	*humi =  tdata*100.0/1024.0/1024.0;
	
	tdata = 0;
	tdata = (tdata|Redata[3])<<8;
	tdata = (tdata|Redata[4])<<8;
	tdata = (tdata|Redata[5]);
	tdata =  tdata&0xfffff;
	*temp =  tdata*200.0/1024.0/1024.0-50.0;
}


