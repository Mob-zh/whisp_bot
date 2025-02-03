#include "GY906.h"
#include "usart.h"



uint16_t data;			// Data storage (DataH:DataL)
uint8_t DataL=0;			// Low data byte storage
uint8_t DataH=0;			// High data byte storage
uint16_t GY906_ReadMemory(uint8_t slaveAddress, uint8_t command)
{

    
	  slaveAddress <<= 1;	//2-7位表示从机地址
	
	repeat:
		MyI2C_Start();				//Start condition
		MyI2C_SendByte(slaveAddress);//Send SlaveAddress 最低位Wr=0表示接下来写命令
		MyI2C_ReceiveAck();
		MyI2C_SendByte(command);	    //Send command
	  u8 ack = MyI2C_ReceiveAck();
		if(ack == 0){

		}
		else{
			goto repeat;
		}
		MyI2C_Start();					//Repeated Start condition
		MyI2C_SendByte(slaveAddress|1);	//Send SlaveAddress 最低位Rd=1表示接下来读数据
  	MyI2C_ReceiveAck();
		DataL = MyI2C_ReceiveByte();	//Read low data,master must send ACK
		MyI2C_SendAck(ACK);
	
		DataH = MyI2C_ReceiveByte(); //Read high data,master must send ACK
		MyI2C_SendAck(NACK);
	
		MyI2C_Stop();				//Stop condition
		data = (DataH<<8) | DataL;	//data=DataH:DataL
    return data;
}

u8 PEC_Calculation(u8 pec[])
{
    u8 	crc[6];
    u8	BitPosition=47;
    u8	shift;
    u8	i;
    u8	j;
    u8	temp;

    do
    {
        /*Load pattern value 0x000000000107*/
        crc[5]=0;
        crc[4]=0;
        crc[3]=0;
        crc[2]=0;
        crc[1]=0x01;
        crc[0]=0x07;

        /*Set maximum bit position at 47 ( six bytes byte5...byte0,MSbit=47)*/
        BitPosition=47;

        /*Set shift position at 0*/
        shift=0;

        /*Find first "1" in the transmited message beginning from the MSByte byte5*/
        i=5;
        j=0;
        while((pec[i]&(0x80>>j))==0 && i>0)
        {
            BitPosition--;
            if(j<7)
            {
                j++;
            }
            else
            {
                j=0x00;
                i--;
            }
        }/*End of while */

        /*Get shift value for pattern value*/
        shift=BitPosition-8;

        /*Shift pattern value */
        while(shift)
        {
            for(i=5; i<0xFF; i--)
            {
                if((crc[i-1]&0x80) && (i>0))
                {
                    temp=1;
                }
                else
                {
                    temp=0;
                }
                crc[i]<<=1;
                crc[i]+=temp;
            }/*End of for*/
            shift--;
        }/*End of while*/

        /*Exclusive OR between pec and crc*/
        for(i=0; i<=5; i++)
        {
            pec[i] ^=crc[i];
        }/*End of for*/
    }
    while(BitPosition>8); /*End of do-while*/

    return pec[0];
}


float GY906_ReadTemp(void)
{   
	float temp;
	temp = GY906_ReadMemory(SA, RAM_ACCESS|RAM_TOBJ1)*0.02-273.15;
	return temp;
}
