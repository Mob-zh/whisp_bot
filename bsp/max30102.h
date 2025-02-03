#ifndef _MAX30102_H_
#define _MAX30102_H_
#include "main.h"
#include "MyI2C.h"
#include "stdbool.h"
#include "stdio.h"
#include "max30102_algorithm.h"
#include "usart.h"
#include "string.h"
#define u8 uint8_t
#define u16 uint16_t
#define ACK	 0 //应答
#define	NACK 1 //无应答
#define max30102_DV_address 0xAE
#define I2C_WRITE_ADDR 0xAE
#define I2C_READ_ADDR 0xAF

#define MAX_INT_GPIO_Port GPIOB //改
#define MAX_INT_Pin GPIO_PIN_9  //改
#define MAX_INT_H          HAL_GPIO_WritePin(MAX_INT_GPIO_Port, MAX_INT_Pin, GPIO_PIN_SET)
#define MAX_INT_read       HAL_GPIO_ReadPin(MAX_INT_GPIO_Port,MAX_INT_Pin)

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */
#define i2c_wr	0		/* 写控制bit */
#define i2c_rd	1		/* 读控制bit */

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF

extern char txBuffer2[30];


void max30102_init(void);  
void max30102_reset(void);
u8 max30102_Bus_Write(u8 Register_Address, u8 Word_Data);
u8 max30102_Bus_Read(u8 Register_Address);
void max30102_FIFO_ReadWords(uint8_t Register_Address,uint16_t  Word_Data[][2],uint8_t count);
void max30102_FIFO_ReadBytes(uint8_t Register_Address,uint8_t* Data);
 
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data);
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data);
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led);

void Max30102_Calculate_HR_BO_Value(int32_t* HR_Value,int8_t* HR_Valid,int32_t* BO_Value,int8_t* BO_Valid);


#endif
