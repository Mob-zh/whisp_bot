#ifndef __MYI2C_H
#define __MYI2C_H
#include "main.h"
#include "stdbool.h"
#define TRUE 1
#define FALSE 0
#define I2C_PORT	  GPIOB      //PB端口(端口和下面的两个针脚可自定义)
#define I2C_SCL		GPIO_PIN_6 //PB6：SCL
#define I2C_SDA		GPIO_PIN_7 //PB7：SDA

#define SCL_read           HAL_GPIO_ReadPin(I2C_PORT,I2C_SCL) 
#define SDA_read           HAL_GPIO_ReadPin(I2C_PORT,I2C_SDA)

void i2c_delay_us(uint32_t time);
void MyI2C_W_SCL(uint8_t BitValue);
void MyI2C_W_SDA(uint8_t BitValue);
uint8_t MyI2C_R_SDA(void);
bool MyI2C_Start(void);
void MyI2C_Stop(void);
void MyI2C_SendBit(uint8_t bit_out);
void MyI2C_SendByte(uint8_t Byte);
uint8_t MyI2C_ReceiveByte(void);
void MyI2C_SendAck(uint8_t AckBit);
uint8_t MyI2C_ReceiveAck(void);

uint8_t MyI2C_WaitAck(void);

bool I2C_Write(uint8_t dev,uint8_t WriteAddr,uint8_t WriteData) ;

bool I2C_Write2(uint8_t WriteAddr,uint8_t WriteData);

uint8_t I2C_Read(uint8_t dev,uint8_t WriteAddr) ;

//读出1串数据          
uint8_t I2C_Read2(uint8_t WriteAddr) ;

void I2C_DevRead(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *rbuf);

void I2C_DevWrite(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *wbuf);
 
//==========================================================
 
void IIC_WriteBytes(uint8_t WriteAddr,uint8_t* data,uint8_t dataLength);
 
void IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr,uint8_t* data,uint8_t dataLength);
 
void IIC_Read_One_Byte(uint8_t daddr,uint8_t addr,uint8_t* data);
 
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
#endif
