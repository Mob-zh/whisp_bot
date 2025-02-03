#include "usart_rx.h"
#include "string.h"
#include "usart.h"
#include "L610_TCP.h"

extern char *strx,*extstrx,*Readystrx,*str1,*str2;
extern 	char RxBuffer[200],Rxcouter;
extern uint8_t move;
uint8_t usart1RxBuffer;


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) 
{  /* Prevent unused argument(s) compilation warning */  
  UNUSED(huart);  
  /* NOTE: This function Should not be modified, when the c
 allback is needed,       
  the HAL_UART_TxCpltCallback could be implemented in the u
 ser file   */   
  if(huart->Instance ==USART1){ 
      RxBuffer[Rxcouter++] = usart1RxBuffer;     
      HAL_UART_Receive_IT(&huart1, (uint8_t *)&usart1RxBuffer, 1);    
  } 
} 

