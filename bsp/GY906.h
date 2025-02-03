#ifndef _GY906_H_
#define _GY906_H_

#include "main.h"
#include "gpio.h"
#include "MyI2C.h"
#define u8 uint8_t
#define ACK	 0 //Ӧ��
#define	NACK 1 //��Ӧ��
#define SA				0x00 //Slave address ����MLX90614ʱ��ַΪ0x00,���ʱ��ַĬ��Ϊ0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM��ȡ����
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM��ȡ����
#define RAM_TOBJ1		0x07 //To1 address in the eeprom Ŀ��1�¶�,��⵽�ĺ����¶� -70.01 ~ 382.19��


uint16_t GY906_ReadMemory(uint8_t slaveAddress, uint8_t command);
u8 PEC_Calculation(u8 pec[]);
float GY906_ReadTemp(void);







#endif


