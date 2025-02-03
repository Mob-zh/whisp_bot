#include "max30102.h"
 
 
 
uint8_t max30102_Bus_Write(uint8_t Register_Address, uint8_t Word_Data)
{
 
	/* ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */
 
	/* ��1��������I2C���������ź� */
	MyI2C_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ */
	MyI2C_SendByte(Register_Address);
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
	/* ��5������ʼд������ */
	MyI2C_SendByte(Word_Data);
 
	/* ��6��������ACK */
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
	return 1;	/* ִ�гɹ� */
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
	return 0;
}
 
 
 
uint8_t max30102_Bus_Read(uint8_t Register_Address)
{
	uint8_t  data;
 
 
	/* ��1��������I2C���������ź� */
	MyI2C_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ�� */
	MyI2C_SendByte((uint8_t)Register_Address);
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
 
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	MyI2C_Start();
 
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
 
	/* ��8��������ACK */
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��9������ȡ���� */
	{
		data = MyI2C_ReceiveByte();	/* ��1���ֽ� */
 
		MyI2C_SendAck(NACK);	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	}
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
	return data;	/* ִ�гɹ� ����dataֵ */
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
	return 0;
}
 
 
void max30102_FIFO_ReadWords(uint8_t Register_Address,uint16_t Word_Data[][2],uint8_t count)
{
	uint8_t i=0;
	uint8_t no = count;
	uint8_t data1, data2;
	/* ��1��������I2C���������ź� */
	MyI2C_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ�� */
	MyI2C_SendByte((uint8_t)Register_Address);
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
 
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	MyI2C_Start();
 
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
 
	/* ��8��������ACK */
	if (MyI2C_ReceiveAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��9������ȡ���� */
	while (no)
	{
		data1 = MyI2C_ReceiveByte();	
		MyI2C_SendAck(ACK);
		data2 = MyI2C_ReceiveByte();
		MyI2C_SendAck(ACK);
		Word_Data[i][0] = (((uint16_t)data1 << 8) | data2);  //
 
		
		data1 = MyI2C_ReceiveByte();	
		MyI2C_SendAck(ACK);
		data2 = MyI2C_ReceiveByte();
		if(1==no)
			MyI2C_SendAck(NACK);	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
		else
			MyI2C_SendAck(ACK);
		Word_Data[i][1] = (((uint16_t)data1 << 8) | data2); 
 
		no--;	
		i++;
	}
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
}
 
void max30102_FIFO_ReadBytes(uint8_t Register_Address,uint8_t* Data)
{	
	uint8_t reg_intr_1 = max30102_Bus_Read(REG_INTR_STATUS_1);
	uint8_t reg_intr_2 =  max30102_Bus_Read(REG_INTR_STATUS_2);
	
	/* ��1��������I2C���������ź� */
	MyI2C_Start();
 
	/* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_WR);	/* �˴���дָ�� */
 
	/* ��3��������ACK */
	if (MyI2C_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��4���������ֽڵ�ַ�� */
	MyI2C_SendByte((uint8_t)Register_Address);
	if (MyI2C_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
	
 
	/* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
	MyI2C_Start();
 
	/* ��7������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
	MyI2C_SendByte(max30102_DV_address | I2C_RD);	/* �˴��Ƕ�ָ�� */
 
	/* ��8��������ACK */
	if (MyI2C_WaitAck() != 0)
	{
		goto cmd_fail;	/* EEPROM������Ӧ�� */
	}
 
	/* ��9������ȡ���� */
	Data[0] = MyI2C_ReceiveByte();	
	MyI2C_SendAck(ACK);
	Data[1] = MyI2C_ReceiveByte();	
	MyI2C_SendAck(ACK);
	Data[2] = MyI2C_ReceiveByte();	
	MyI2C_SendAck(ACK);
	Data[3] = MyI2C_ReceiveByte();
	MyI2C_SendAck(ACK);
	Data[4] = MyI2C_ReceiveByte();	
	MyI2C_SendAck(ACK);
	Data[5] = MyI2C_ReceiveByte();
	MyI2C_SendAck(NACK);
	/* ���1���ֽڶ����CPU����NACK�ź�(����SDA = 1) */
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
 
cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
	/* ����I2C����ֹͣ�ź� */
	MyI2C_Stop();
}
 
void max30102_init(void)
{
 
	repeat:
	
	max30102_reset();
	
	if(max30102_Bus_Write(REG_INTR_ENABLE_1,0xc0)==0)
	{
		printf("1\n");
		
		goto repeat;
	}		                                        // INTR setting
	
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_INTR_ENABLE_2,0x00)==0)
	{
		printf("2\n");
				goto repeat;

	}		
			//������
	sprintf(txBuffer2,"j0.val=10\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_FIFO_WR_PTR,0x00)==0)
	{
		printf("3\n");
				goto repeat;

	}		             
	//FIFO_WR_PTR[4:0]
			//������
	sprintf(txBuffer2,"j0.val=20\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_OVF_COUNTER,0x00)==0)  	//OVF_COUNTER[4:0]
	{
		printf("4\n");
				goto repeat;

		
	}		
				//������
	sprintf(txBuffer2,"j0.val=30\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_FIFO_RD_PTR,0x00)==0)  	//FIFO_RD_PTR[4:0]
	{
		printf("5\n");
				goto repeat;

	}		
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_FIFO_CONFIG,0x0f)==0)  	//sample avg = 1, fifo rollover=false, fifo almost full = 17
	{
		printf("6\n");
				goto repeat;

	}		
		//������
	sprintf(txBuffer2,"j0.val=40\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

	HAL_Delay(10);
	if(max30102_Bus_Write(REG_MODE_CONFIG,0x03)==0)  	//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	{
		printf("7\n");
				goto repeat;

	}		
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_SPO2_CONFIG,0x27)==0)  	// SPO2_ADC range = 4096nA, SPO2 sample rate (100 Hz), LED pulseWidth (400uS) 
	{
		printf("8\n");
				goto repeat;

	}		
	//������
	sprintf(txBuffer2,"j0.val=50\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

  HAL_Delay(10);	
	if(max30102_Bus_Write(REG_LED1_PA,0x24)==0)   	//Choose value for ~ 7mA for LED1
	{
		printf("9\n");
				goto repeat;

	}		
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_LED2_PA,0x24)==0)   	// Choose value for ~ 7mA for LED2
	{
		printf("10\n");
				goto repeat;

	}		
	
	HAL_Delay(10);
	if(max30102_Bus_Write(REG_PILOT_PA,0x7f)==0)   	// Choose value for ~ 25mA for Pilot LED
	{
		printf("11\n");
				goto repeat;

	}		
	HAL_Delay(10);					
	
	//������
	sprintf(txBuffer2,"j0.val=60\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

	
	printf("\r\n MAX30102  init  \r\n");	
	for(int i=0;i<ir_buffer_length;i++)
	{
		while(MAX_INT_read==GPIO_PIN_SET); //�ȴ��ж�������Ӧ��Ĭ��Ϊ��,�������������
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,TempData);
		red_buffer[i]=((TempData[0]&0x03)<<16) | (TempData[1]<<8) | (TempData[2]); //ǰ��λ�������HR
		ir_buffer[i]=((TempData[3]&0x03)<<16) | (TempData[4]<<8) | (TempData[5]); //����λ�������BO
		if(red_min>red_buffer[i]) red_min=red_buffer[i];  //���µ�ǰ��Сֵ
		if(red_max<red_buffer[i]) red_max=red_buffer[i];  //���µ�ǰ���ֵ
	}
	maxim_heart_rate_and_oxygen_saturation(ir_buffer,ir_buffer_length,red_buffer,&pn_SpO2_value,&SpO2_valid,&pn_hr_value,&hr_valid);
	sprintf(txBuffer2,"j0.val=80\xff\xff\xff");
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
	HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

}
 
void max30102_reset(void)
{
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
	max30102_Bus_Write(REG_MODE_CONFIG,0x40);
}
 
void maxim_max30102_write_reg(uint8_t uch_addr, uint8_t uch_data)
{
	IIC_Write_One_Byte(I2C_WRITE_ADDR,uch_addr,uch_data);
}
 
void maxim_max30102_read_reg(uint8_t uch_addr, uint8_t *puch_data)
{
 
	IIC_Read_One_Byte(I2C_WRITE_ADDR,uch_addr,puch_data);
}
 
void maxim_max30102_read_fifo(uint32_t *pun_red_led, uint32_t *pun_ir_led)
{
	uint32_t un_temp;
	unsigned char uch_temp;
	char ach_i2c_data[6];
	*pun_red_led=0;
	*pun_ir_led=0;
 
  
  //read and clear status register
  maxim_max30102_read_reg(REG_INTR_STATUS_1, &uch_temp);
  maxim_max30102_read_reg(REG_INTR_STATUS_2, &uch_temp);
  
  IIC_ReadBytes(I2C_WRITE_ADDR,REG_FIFO_DATA,(uint8_t *)ach_i2c_data,6);
  
  un_temp=(unsigned char) ach_i2c_data[0];
  un_temp<<=16;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[1];
  un_temp<<=8;
  *pun_red_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[2];
  *pun_red_led+=un_temp;
  
  un_temp=(unsigned char) ach_i2c_data[3];
  un_temp<<=16;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[4];
  un_temp<<=8;
  *pun_ir_led+=un_temp;
  un_temp=(unsigned char) ach_i2c_data[5];
  *pun_ir_led+=un_temp;
  *pun_red_led&=0x03FFFF;  //Mask MSB [23:18]
  *pun_ir_led&=0x03FFFF;  //Mask MSB [23:18]
}

void Max30102_Calculate_HR_BO_Value(int32_t* HR_Value,int8_t* HR_Valid,int32_t* BO_Value,int8_t* BO_Valid)
{
	for(int i=100;i<500;i++)  //�������е�100~500����ֵ��ǰŲ��0~400
	{
		red_buffer[i-100]=red_buffer[i];
		ir_buffer[i-100]=ir_buffer[i];
		if(red_min>red_buffer[i]) red_min=red_buffer[i];  //���µ�ǰ��Сֵ
		if(red_max<red_buffer[i]) red_max=red_buffer[i];  //���µ�ǰ���ֵ
	}
	for(int i=400;i<500;i++)  //ʵ��ֻȡ100������ֵ������
	{
		prev_data=red_buffer[i-1];
		while(MAX_INT_read==1); //�ȴ��ж�������Ӧ��Ĭ��Ϊ��,�������������
		max30102_FIFO_ReadBytes(REG_FIFO_DATA,TempData);
		red_buffer[i]=((TempData[0]&0x03)<<16) | (TempData[1]<<8) | (TempData[2]); //ǰ��λ�������HR
		ir_buffer[i]=((TempData[3]&0x03)<<16) | (TempData[4]<<8) | (TempData[5]); //����λ�������BO
		if(red_buffer[i]>prev_data)
		{    //���ʹ�ʽ��|��һ�ε�ֵ-��ǰֵ| / (���ֵ-��Сֵ) * 255
			f_temp=(float)(red_buffer[i]-prev_data)/(red_max-red_min)*255;
			n_brightness-=(int)f_temp;
			if(n_brightness<0) n_brightness=0;
		}
		else
		{
			f_temp=(float)(prev_data-red_buffer[i])/(red_max-red_min)*255;
			n_brightness+=(int)f_temp;
			if(n_brightness>255) n_brightness=255;
		}
		*HR_Value=pn_hr_value;
		*HR_Valid=hr_valid;
		*BO_Value=pn_SpO2_value;
		*BO_Valid=SpO2_valid;
	}
		maxim_heart_rate_and_oxygen_saturation(ir_buffer,ir_buffer_length,red_buffer,&pn_SpO2_value,&SpO2_valid,&pn_hr_value,&hr_valid);

}

