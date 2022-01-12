#ifndef _AHT10_H__
#define _AHT10_H__
#include "stm32f1xx_hal.h"



/*****************º¯ÊýÉùÃ÷******************/
void AHT10_Reset(void);
void AHT10_ReadData(void);
char AHT10_ReadStatus(void);
char AHT10_Read_CalEnable(void); 
char AHT10_Init(void);
void AHT10_Data(double *temp, double *humi);

#endif
