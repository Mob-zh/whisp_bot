#include "MyI2C.h"




void i2c_delay_us(uint32_t time)
{
    uint32_t cnt = time*35; 
		uint32_t i = 0;
		for(i=0;i<cnt;i++)__NOP();
}

void MyI2C_W_SCL(uint8_t BitValue)
{
	HAL_GPIO_WritePin(I2C_PORT,I2C_SCL,(GPIO_PinState)BitValue);//置高电平
//	HAL_Delay(2);
	i2c_delay_us(5);
}

void MyI2C_W_SDA(uint8_t BitValue)
{
	HAL_GPIO_WritePin(I2C_PORT,I2C_SDA,(GPIO_PinState)BitValue);//置高电平
//		HAL_Delay(2);

	i2c_delay_us(5);
}

uint8_t MyI2C_R_SDA(void)
{
	uint8_t x = HAL_GPIO_ReadPin(I2C_PORT,I2C_SDA);
//		HAL_Delay(2);

	i2c_delay_us(5);
	return x;

}

bool MyI2C_Start(void)
{
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	if(!SDA_read) return FALSE;
	MyI2C_W_SDA(0);
	if(SDA_read) return FALSE;
	MyI2C_W_SCL(0);
	return TRUE;
}

void MyI2C_Stop(void)
{
	MyI2C_W_SCL(0);
	MyI2C_W_SDA(0);
	MyI2C_W_SCL(1);
	MyI2C_W_SDA(1);
}

void MyI2C_SendBit(uint8_t bit_out)
{

		
    if(bit_out==0)
    {
        MyI2C_W_SDA(0);
    }
    else
    {
        MyI2C_W_SDA(1);
    }
    MyI2C_W_SCL(1);					// Set SCL line
    MyI2C_W_SCL(0);					// Clear SCL line
		
}

void MyI2C_SendByte(uint8_t Byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SDA(Byte & (0x80 >> i));
		MyI2C_W_SCL(1);
		MyI2C_W_SCL(0);
	}
}

uint8_t MyI2C_ReceiveByte()
{
	uint8_t i, Byte = 0x00;
	MyI2C_W_SDA(1);
	for (i = 0; i < 8; i ++)
	{
		MyI2C_W_SCL(1);
		if (MyI2C_R_SDA() == 1){Byte |= (0x80 >> i);}
		MyI2C_W_SCL(0);
	}
	return Byte;
}

void MyI2C_SendAck(uint8_t AckBit)
{
	MyI2C_W_SDA(AckBit);
	MyI2C_W_SCL(1);
	MyI2C_W_SCL(0);
}

uint8_t MyI2C_ReceiveAck(void)
{
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}

uint8_t MyI2C_WaitAck(void)
{ 
	uint8_t AckBit;
	MyI2C_W_SDA(1);
	MyI2C_W_SCL(1);
	AckBit = MyI2C_R_SDA();
	MyI2C_W_SCL(0);
	return AckBit;
}

/**
  * @brief Write a Byte to the device
  * @param uint8_t WriteAddr,uint8_t WriteData
  * @retval bool FALSE: 0
  *              TRUE : 1
  */
bool I2C_Write(uint8_t dev,uint8_t WriteAddr,uint8_t WriteData) 
{ 
	MyI2C_Start();  
	MyI2C_SendByte(dev); //Send write cmd   
	MyI2C_WaitAck();	   
	MyI2C_SendByte(WriteAddr); //Send addr                                                    
	MyI2C_WaitAck(); 	 										  		   
	MyI2C_SendByte(WriteData); //Send data                                             						   
	MyI2C_WaitAck();  		    	   
	MyI2C_Stop();		   	//iic stop
	return TRUE;
} 
/********************************************************************/
bool I2C_Write2(uint8_t WriteAddr,uint8_t WriteData)   
{ 
    if (!MyI2C_Start()) return FALSE; 
    MyI2C_SendByte(0x10);//设置器件地址+段地址  
    if (!MyI2C_WaitAck()) 
		{ 
			MyI2C_Stop();  
			return FALSE; 
		} 
    MyI2C_SendByte(WriteAddr);   //设置段内地址       
		MyI2C_WaitAck(); 
   
		MyI2C_SendByte(WriteData); 
		MyI2C_WaitAck(); 
		MyI2C_Stop(); 
 
		return TRUE; 
} 
/************************************************************************/
/**
  * @brief Read a byte from the device
  * @param uint8_t WriteAddr
  * @retval temp  Return the read byte 
  */      
uint8_t I2C_Read(uint8_t dev,uint8_t WriteAddr) 
{ 
	uint8_t temp=0;		  	    																 
	MyI2C_Start();  
	MyI2C_SendByte(dev);  //Send write cmd                                      	   
	MyI2C_WaitAck(); 
	MyI2C_SendByte(WriteAddr); //Send addr                        
	MyI2C_WaitAck();	
 
	MyI2C_Start();  	 	   
	MyI2C_SendByte(dev|1);  //Send read cmd                                               
	MyI2C_WaitAck();	 
	temp=MyI2C_ReceiveByte();			   
	MyI2C_Stop();                   	    
	return temp;
} 
/*********************************************************************************/
//读出1串数据          
uint8_t I2C_Read2(uint8_t WriteAddr) 
{ 
	uint8_t tempDat=0;	
	if (!MyI2C_Start()) return FALSE; 
  MyI2C_SendByte(0x77);//设置器件地址+段地址  
 
  if (!MyI2C_WaitAck())  
	{ 
		MyI2C_Stop();  
		return FALSE; 
	} 
    MyI2C_SendByte(WriteAddr);   //设置低起始地址       
    MyI2C_WaitAck(); 
    MyI2C_Start(); 
    MyI2C_SendByte(0x77 | 0x01); 
    MyI2C_WaitAck(); 
    tempDat = MyI2C_ReceiveByte();  
    MyI2C_Stop(); 
 
		return tempDat; 
} 
/**
  * @brief Read continuously
  * @param uint8_t devaddr  device addr
  *        uint8_t addr     Start addr 
  *        uint8_t len      read data length
  *        uint8_t *rbuf    read data buf 
  * @retval None
  */ 
void I2C_DevRead(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *rbuf)
{
	int i=0;
	MyI2C_Start();  
	MyI2C_SendByte(devaddr);  
	if(!MyI2C_WaitAck())
	{
	  MyI2C_Stop();
		return ;
	}		
	MyI2C_SendByte(addr);  //address ++ 
	if(!MyI2C_WaitAck())
	{
	  MyI2C_Stop();
		return ;
	}	
	MyI2C_Start();
	MyI2C_SendByte(devaddr|0x01);  	
	if(!MyI2C_WaitAck())
	{
	  MyI2C_Stop();
		return ;
	}		
	for(i=0; i<len; i++)
	{
 
		if(i==len-1)
		{
			rbuf[i]=MyI2C_ReceiveByte();  //The last byte does not answer
		}
		else
			rbuf[i]=MyI2C_ReceiveByte();
			MyI2C_SendAck(1);
	}
	MyI2C_Stop( );	
}
 
/**
  * @brief Write continuously
  * @param uint8_t devaddr  device addr
  *        uint8_t addr     Start addr 
  *        uint8_t len      read data length
  *        uint8_t *rbuf    read data buf 
  * @retval None
  */ 
void I2C_DevWrite(uint8_t devaddr,uint8_t addr,uint8_t len,uint8_t *wbuf)
{
	int i=0;
	MyI2C_Start();  
	MyI2C_SendByte(devaddr);  	
	MyI2C_WaitAck();	
	MyI2C_SendByte(addr);  //address ++ 
	MyI2C_WaitAck();	
	for(i=0; i<len; i++)
	{
		MyI2C_SendByte(wbuf[i]);  
		MyI2C_WaitAck();		
	}
	MyI2C_Stop( );	
}
 
//==========================================================
 
void IIC_WriteBytes(uint8_t WriteAddr,uint8_t* data,uint8_t dataLength)
{		
	uint8_t i;	
    MyI2C_Start();  
 
	MyI2C_SendByte(WriteAddr);	    //发送写命令
	MyI2C_WaitAck();
	
	for(i=0;i<dataLength;i++)
	{
		MyI2C_SendByte(data[i]);
		MyI2C_WaitAck();
	}				    	   
    MyI2C_Stop();//产生一个停止条件 
	i2c_delay_us(10); 
}
 
void IIC_ReadBytes(uint8_t deviceAddr, uint8_t writeAddr,uint8_t* data,uint8_t dataLength)
{		
	uint8_t i;	
    MyI2C_Start();  
 
	MyI2C_SendByte(deviceAddr);	    //发送写命令
	MyI2C_WaitAck();
	MyI2C_SendByte(writeAddr);
	MyI2C_WaitAck();
	MyI2C_SendByte(deviceAddr|0X01);//进入接收模式			   
	MyI2C_WaitAck();
	
	for(i=0;i<dataLength-1;i++)
	{
		data[i] = MyI2C_ReceiveByte();
		MyI2C_SendAck(1);
	}		
	data[dataLength-1] = MyI2C_ReceiveByte();	
    MyI2C_Stop();//产生一个停止条件 
	i2c_delay_us(10);	 
}
 
void IIC_Read_One_Byte(uint8_t daddr,uint8_t addr,uint8_t* data)
{				  	  	    																 
    MyI2C_Start();  
	
	MyI2C_SendByte(daddr);	   //发送写命令
	MyI2C_WaitAck();
	MyI2C_SendByte(addr);//发送地址
	MyI2C_WaitAck();		 
	MyI2C_Start();  	 	   
	MyI2C_SendByte(daddr|0X01);//进入接收模式			   
	MyI2C_WaitAck();	 
	*data = MyI2C_ReceiveByte();		   
	MyI2C_Stop();//产生一个停止条件	    
}
 
void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data)
{				   	  	    																 
    MyI2C_Start();  
	
	MyI2C_SendByte(daddr);	    //发送写命令
	MyI2C_WaitAck();
	MyI2C_SendByte(addr);//发送地址
	MyI2C_WaitAck();	   	 										  		   
	MyI2C_SendByte(data);     //发送字节							   
	MyI2C_WaitAck();  		    	   
  MyI2C_Stop();//产生一个停止条件 
	i2c_delay_us(10);
}
