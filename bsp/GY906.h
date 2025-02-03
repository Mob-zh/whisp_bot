#ifndef _GY906_H_
#define _GY906_H_

#include "main.h"
#include "gpio.h"
#include "MyI2C.h"
#define u8 uint8_t
#define ACK	 0 //应答
#define	NACK 1 //无应答
#define SA				0x00 //Slave address 单个MLX90614时地址为0x00,多个时地址默认为0x5a
#define RAM_ACCESS		0x00 //RAM access command RAM存取命令
#define EEPROM_ACCESS	0x20 //EEPROM access command EEPROM存取命令
#define RAM_TOBJ1		0x07 //To1 address in the eeprom 目标1温度,检测到的红外温度 -70.01 ~ 382.19度


uint16_t GY906_ReadMemory(uint8_t slaveAddress, uint8_t command);
u8 PEC_Calculation(u8 pec[]);
float GY906_ReadTemp(void);







#endif


