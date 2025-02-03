/*******************************************************************************
**                       @ Copyright 2014 - 2020                              **
**                           www.diysoon.com                                  **
**                                                                            **
*******************************************************************************/

#include "L610_Init.h"
#include "L610_TCP.h"
#include "main.h"
#include "stdio.h"
#include "string.h"
#include "usart.h"
#include "json2data.h"
#include "stdbool.h"

extern	ParsedData cmd;
char *strx,*extstrx,*Readystrx,*str1,*str2;
extern 	char RxBuffer[200],RxBuffer2[30];
extern int L610_sta;
extern bool celiang_flag;

//���ڽ����ж�
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{  
	
  if(huart == &huart1){
//			HAL_UART_Transmit(huart,(uint8_t*)RxBuffer,Size,10);
			//ATcheck
//			if(L610_sta==-1){
//					strx = strstr((const char*)RxBuffer,(const char*)"SIM READY");
//						if(strx!=NULL){
//							L610_sta = 0;
//							strx = NULL;
//						}
//			}
//			else{
				if(L610_sta == 0){ 
						strx = strstr((const char*)RxBuffer,(const char*)"OK");
						if(strx!=NULL){
							L610_sta = 1;
							strx = NULL;
						}
				}	
				//Get IP
				else if(L610_sta == 1){
						strx = strstr((const char*)RxBuffer,(const char*)"OK");
//						str1 = strstr((const char*)RxBuffer,(const char*)"ERR");
						if(strx!=NULL){
							L610_sta = 2;
							strx = NULL;
						}
				}
				//DISCONNECT
				else if(L610_sta == 2){
						strx = strstr((const char*)RxBuffer,(const char*)"OK");
						str1 = strstr((const char*)RxBuffer,(const char*)"ERR:12");
						if(strx!=NULL||str1!=NULL){
							L610_sta = 3;
							strx = NULL;
						}
				}
				//CONNECT
				else if(L610_sta == 3){
						strx=strstr((const char*)RxBuffer,(const char*)"+HMCON OK");//��������
						str1=strstr((const char*)RxBuffer,(const char*)"+HMCON ERR:1");
						if(strx!=NULL||str1!=NULL){
							L610_sta = 4;
							strx = NULL;
						}
				}
				else if(L610_sta == 4){
						
						strx=strstr((const char*)RxBuffer,(const char*)"AT");//��������
						if(strx!=NULL){
							L610_sta = 5;
							strx = NULL;
						}
				}
				else{
						strx=strstr((const char*)RxBuffer,(const char*)"REC");//��������
						if(strx!=NULL){
							cmd = Commandname_parse_json(cmd,RxBuffer);
							if(strstr(cmd.command_name,"Control")){
									cmd = move_parse_json(cmd,RxBuffer);
							}
							else if(strstr(cmd.command_name,"yizhu")){
									cmd = text_parse_json(cmd,RxBuffer);
							}
							else if(strstr(cmd.command_name,"Box")){
									cmd = Box_parse_json(cmd,RxBuffer);
							}
						}
						
				}
				Clear_Buffer();
				HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t*)RxBuffer,sizeof(RxBuffer));
//			}
  } 
	else if(huart == &huart2){
			
			if(RxBuffer2[0]=='I'){
				celiang_flag = 1;
			}
			HAL_UARTEx_ReceiveToIdle_IT(&huart2,(uint8_t*)RxBuffer2,sizeof(RxBuffer2));

	}
	
	
} 

/*****************************************************************

		ģ�鿪������
		ģ���ϵ��Ĭ���ǹػ�״̬����Ҫ����IO�������ƽ����ģ�鿪��

*******************************************************************/


void OPEN_MOUDLE(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);	

}


/********************************************************

	���Buffer�е�����

*********************************************************/

void Clear_Buffer(void)//��ջ���
{
    uint8_t i;
  //  HAL_UART_Transmit(&huart1,(uint8_t *)RxBuffer,20,20);
    for(i=0;i<sizeof(RxBuffer);i++)
			RxBuffer[i]=0;//����

}



/********************************************************

	ģ���ʼ��������ͨ��ATָ���ģ��������ֽ��г�ʼ����
	��ʼ��������ģ�����ͨ��TCPЭ����������������ݡ�
	�����������Ӫ�̷���IP

*********************************************************/
void L610_HW_init(void)
{	
	if(L610_sta == 0)	AT_check(); 
	else if(L610_sta == 1) Get_IP(); 
	else if(L610_sta == 2) HW_DISConnect();
	else if(L610_sta == 3) HW_Connect_MQTT("117.78.5.125","Whisp_dev1","Hh1578965547");
	else if(L610_sta == 4) HW_HMSUB_0("Whisp_dev1");
	HAL_Delay(100);
}

/*******************************************************

	���ģ��ATָ��״̬���ظ�OK������������ATָ�������
	���buffer��
	��ӦATָ�AT

*******************************************************/

void AT_check(void)
{
	printf("AT\r\n"); 
}

/***************************************************

	��ѯģ��̼��汾�ţ�������������������費ǿ��
	��ӦATָ�ATI

****************************************************/

void ATI_check(void)
{
	printf("ATI\r\n"); //���ģ��İ汾��
}
/****************************************************

	���SIM��״̬�����SIM��״̬��������ģ����޷�������������ͨ��
	��ӦATָ�� ��AT+CPIN?

*****************************************************/

void CPIN_check(void)
{
	printf("AT+CPIN?\r\n");//���SIM���Ƿ���λ,����ȱ�ڳ������ 
}


/******************************************************

  ��ѯGPRS�����Ƿ����,���ģ�������Ƿ�����
	��ӦATָ�AT+CGREG?	

*******************************************************/

void GPRS_check(void)
{
	printf("AT+CGREG?\r\n");
}

/******************************************************

  ��ѯEPS�����Ƿ����,���ģ�������Ƿ�����
	��ӦATָ�AT+CEREG?	

*******************************************************/

void EPS_check(void)
{
	printf("AT+CEREG?\r\n");
}


/******************************************************************

	���õ�ģ���յ�������������ʱ���ϱ����ݵĵĸ�ʽ��Ĭ�� 0�����籣�档
	��ӦATָ�AT+GTSET="IPRFMT",0
	
******************************************************************/

void Hex_set(void)
{
	
	printf("AT+GTSET=\"IPRFMT\",0\r\n");
	
}

/******************************************************************

	��ȡIp��ַ
	
******************************************************************/



void Get_IP(void)
{
	printf("AT+MIPCALL=1\r\n");
  
//	printf("AT+MIPCALL?\r\n");

}

/******************************************************************
	MQTT���ӻ�Ϊ��
	����һ�������ַ
	���������豸id
	����������Կ
******************************************************************/

void HW_Connect_MQTT(const char* Ac_address,const char* dev_id,const char* dev_password){
	
	printf("AT+HMCON=0,60,\"%s\",\"1883\",\"%s\",\"%s\",0\r\n",Ac_address,dev_id,dev_password);
}

/******************************************************************
	���Ļ�Ϊ���ϱ�����
******************************************************************/

void HW_HMSUB_0(const char* dev_id){
	
	printf("AT+HMSUB=0,\"$oc/devices/%s/sys/properties/report\"\r\n",dev_id);	

}

/******************************************************************
	�����ַ������ȣ�������/����
******************************************************************/

int Mystrlen(const char* str){
	
	int cnt = 0;
	int p = 0;
	while(str[p]!='\0'){
		if(str[p]=='\\'){
			p++;
		}
		else{
			cnt++;
			p++;
		}
	}
	return cnt;
}


/******************************************************************
	�ϱ�����
******************************************************************/

void HW_HMSUB_1(const char* dev_id,float TEMP,int HR_Avg,float BO){
	
	
	char json_data[124];
	sprintf(json_data,"{\\\"services\\\":[{\\\"service_id\\\":\\\"mcu_data\\\",\\\"properties\\\":{\\\"Temp\\\":%.1f,\\\"HR\\\":%d,\\\"BO\\\":%.1f}}]}",TEMP,HR_Avg,BO);
	printf("AT+HMPUB=1,\"$oc/devices/%s/sys/properties/report\",%d,\"%s\"\r\n",dev_id,Mystrlen(json_data),json_data);


}

/******************************************************************
	�ϱ��¶�����
******************************************************************/

void HW_HMSUB_1_temp(const char* dev_id,float TEMP){
	
	
	char json_data[124];
	sprintf(json_data,"{\\\"services\\\":[{\\\"service_id\\\":\\\"mcu_data\\\",\\\"properties\\\":{\\\"Temp\\\":%.1f}}]}",TEMP);
	printf("AT+HMPUB=1,\"$oc/devices/%s/sys/properties/report\",%d,\"%s\"\r\n",dev_id,Mystrlen(json_data),json_data);
}

/******************************************************************
	�Ͽ���Ϊ��
******************************************************************/

void HW_DISConnect(void){
	
	printf("AT+HMDIS\r\n");
}

