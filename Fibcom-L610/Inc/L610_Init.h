/*******************************************************************************
**                       @ Copyright 2014 - 2019                              **
**                           www.diysoon.com                                  **
**                                                                            **
*******************************************************************************/


#ifndef L610_INIT_H
#define L610_INIT_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"


/* function declaration -----------------------------------------------------*/


extern void OPEN_MOUDLE(void);
extern void WAIT_OPEN(void);
extern void Clear_Buffer(void);
extern void L610_HW_init(void);
extern void AT_check(void);
extern void CSQ_check(void);
extern void ATI_check(void);
extern void CPIN_check(void);
extern void GPRS_check(void);
extern void EPS_check(void);
extern void Hex_set(void);

extern void Fibcom_L610_init_pre(void);
extern void Deactive_PDP(void);

//连接华为云
void Get_IP(void);
void HW_Connect_MQTT(const char* Ac_address,const char* dev_id,const char* dev_password);
void HW_HMSUB_0(const char* dev_id);
void HW_HMSUB_1(const char* dev_id,float TEMP,int HR_Avg,float BO);
void HW_DISConnect(void);
void HW_HMSUB_1_temp(const char* dev_id,float TEMP);
#ifdef __cplusplus
}
#endif
#endif /*L610_Init_H */
