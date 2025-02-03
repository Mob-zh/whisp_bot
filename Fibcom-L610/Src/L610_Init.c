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

//串口接收中断
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
						strx=strstr((const char*)RxBuffer,(const char*)"+HMCON OK");//返回正常
						str1=strstr((const char*)RxBuffer,(const char*)"+HMCON ERR:1");
						if(strx!=NULL||str1!=NULL){
							L610_sta = 4;
							strx = NULL;
						}
				}
				else if(L610_sta == 4){
						
						strx=strstr((const char*)RxBuffer,(const char*)"AT");//返回正常
						if(strx!=NULL){
							L610_sta = 5;
							strx = NULL;
						}
				}
				else{
						strx=strstr((const char*)RxBuffer,(const char*)"REC");//返回正常
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

		模块开机函数
		模块上电后默认是关机状态，需要控制IO口输出电平控制模块开机

*******************************************************************/


void OPEN_MOUDLE(void)
{
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_RESET);	

}


/********************************************************

	清空Buffer中的数据

*********************************************************/

void Clear_Buffer(void)//清空缓存
{
    uint8_t i;
  //  HAL_UART_Transmit(&huart1,(uint8_t *)RxBuffer,20,20);
    for(i=0;i<sizeof(RxBuffer);i++)
			RxBuffer[i]=0;//缓存

}



/********************************************************

	模块初始化函数，通过AT指令对模块各个部分进行初始化，
	初始化结束后模块可以通过TCP协议向服务器发送数据。
	最后是请求运营商分配IP

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

	检测模块AT指令状态，回复OK可以正常发送AT指令，结束后
	清除buffer。
	对应AT指令：AT

*******************************************************/

void AT_check(void)
{
	printf("AT\r\n"); 
}

/***************************************************

	查询模块固件版本号，便于问题分析，本步骤不强制
	对应AT指令：ATI

****************************************************/

void ATI_check(void)
{
	printf("ATI\r\n"); //检查模块的版本号
}
/****************************************************

	检查SIM卡状态，如果SIM卡状态不正常，模块就无法正常建立无线通信
	对应AT指令 ：AT+CPIN?

*****************************************************/

void CPIN_check(void)
{
	printf("AT+CPIN?\r\n");//检查SIM卡是否在位,卡的缺口朝外放置 
}


/******************************************************

  查询GPRS服务是否可用,检查模块网络是否正常
	对应AT指令：AT+CGREG?	

*******************************************************/

void GPRS_check(void)
{
	printf("AT+CGREG?\r\n");
}

/******************************************************

  查询EPS服务是否可用,检查模块网络是否正常
	对应AT指令：AT+CEREG?	

*******************************************************/

void EPS_check(void)
{
	printf("AT+CEREG?\r\n");
}


/******************************************************************

	设置当模块收到服务器的数据时，上报数据的的格式。默认 0，掉电保存。
	对应AT指令：AT+GTSET="IPRFMT",0
	
******************************************************************/

void Hex_set(void)
{
	
	printf("AT+GTSET=\"IPRFMT\",0\r\n");
	
}

/******************************************************************

	获取Ip地址
	
******************************************************************/



void Get_IP(void)
{
	printf("AT+MIPCALL=1\r\n");
  
//	printf("AT+MIPCALL?\r\n");

}

/******************************************************************
	MQTT连接华为云
	参数一：接入地址
	参数二：设备id
	参数三：密钥
******************************************************************/

void HW_Connect_MQTT(const char* Ac_address,const char* dev_id,const char* dev_password){
	
	printf("AT+HMCON=0,60,\"%s\",\"1883\",\"%s\",\"%s\",0\r\n",Ac_address,dev_id,dev_password);
}

/******************************************************************
	订阅华为云上报属性
******************************************************************/

void HW_HMSUB_0(const char* dev_id){
	
	printf("AT+HMSUB=0,\"$oc/devices/%s/sys/properties/report\"\r\n",dev_id);	

}

/******************************************************************
	计算字符串长度，不包括/符号
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
	上报属性
******************************************************************/

void HW_HMSUB_1(const char* dev_id,float TEMP,int HR_Avg,float BO){
	
	
	char json_data[124];
	sprintf(json_data,"{\\\"services\\\":[{\\\"service_id\\\":\\\"mcu_data\\\",\\\"properties\\\":{\\\"Temp\\\":%.1f,\\\"HR\\\":%d,\\\"BO\\\":%.1f}}]}",TEMP,HR_Avg,BO);
	printf("AT+HMPUB=1,\"$oc/devices/%s/sys/properties/report\",%d,\"%s\"\r\n",dev_id,Mystrlen(json_data),json_data);


}

/******************************************************************
	上报温度属性
******************************************************************/

void HW_HMSUB_1_temp(const char* dev_id,float TEMP){
	
	
	char json_data[124];
	sprintf(json_data,"{\\\"services\\\":[{\\\"service_id\\\":\\\"mcu_data\\\",\\\"properties\\\":{\\\"Temp\\\":%.1f}}]}",TEMP);
	printf("AT+HMPUB=1,\"$oc/devices/%s/sys/properties/report\",%d,\"%s\"\r\n",dev_id,Mystrlen(json_data),json_data);
}

/******************************************************************
	断开华为云
******************************************************************/

void HW_DISConnect(void){
	
	printf("AT+HMDIS\r\n");
}

