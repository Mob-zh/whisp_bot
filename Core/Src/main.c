/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "headfile.h"
#include "json2data.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void move_proc(void);
void celiang_proc(void);
void yizhu_proc(void);
void Box_proc(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
	int L610_sta = 0;
	ParsedData cmd;
	
	bool celiang_flag = 0;//测量是否开启标志位
	float temp;//测量的体温
	int32_t HR_Value,BO_Value;//心率和血氧的值
  int8_t HR_Valid,BO_Valid;//心率血氧值是否有效
	int max30102_data[6][2] = {0};//存心血氧模块的寄存器数据，不用管
	uint32_t avg_HR_VAL[128],avg_HR,AHV_p,BOV_p,TEMP_p;//求传感器平均值
	float avg_BO_VAL[128],avg_BO ,avg_temp;

	extern char *strx;
	char RxBuffer[250];//L610接收缓冲区
	char RxBuffer2[30];//串口屏接收缓冲区
	
	char* last_yizhu;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
	
//以下数据均不用管
uint32_t aun_ir_buffer[500]; //IR LED sensor data
uint16_t n_ir_buffer_length;    //data length
uint32_t aun_red_buffer[500];    //Red LED sensor data
int16_t n_sp02; //SPO2 value
int8_t ch_spo2_valid;   //indicator to show if the SP02 calculation is valid
int16_t n_heart_rate;   //heart rate value
int8_t  ch_hr_valid;    //indicator to show if the heart rate calculation is valid

char txBuffer2[30];

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	int32_t HR_Value,BO_Value;
  int8_t HR_Valid,BO_Valid;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART1_UART_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USART2_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t*)RxBuffer,sizeof(RxBuffer));
	HAL_UARTEx_ReceiveToIdle_IT(&huart2,(uint8_t*)RxBuffer2,sizeof(RxBuffer2));
	HAL_TIM_Base_Init(&htim6);
	max30102_init();
	HAL_Delay(4000);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(L610_sta<=4){
			L610_HW_init();
			if(L610_sta == 4)  {
				sprintf(txBuffer2,"j0.val=100\xff\xff\xff");
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

			}
			
		}
		else{
				if(L610_sta == 5)  {
				sprintf(txBuffer2,"j0.val=100\xff\xff\xff");
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
				L610_sta  = 6;
			}
					//进度条
//			printf("%d",cmd.move);
				
			  move_proc();
				yizhu_proc();
				Box_proc();
			 if(celiang_flag == 1){	
					cmd.move = 0;
					move_proc();
//测温度
					int i = 10;
				
				 while(i>0){
					temp = GY906_ReadTemp();
//				printf("%.1f\n",temp);
					sprintf(txBuffer2,"x0.val=%d\xff\xff\xff",(int)(temp*1.2f*10));
					HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
					HW_HMSUB_1_temp("Whisp_dev1",temp*1.2f);


//测血氧
					Max30102_Calculate_HR_BO_Value(&HR_Value,&HR_Valid,&BO_Value,&BO_Valid);
//					printf("%.1f %d %d %d %d\n",temp,HR_Value,HR_Valid,BO_Value,BO_Valid);
					//求均值
					if(HR_Valid && BO_Valid){
					avg_HR_VAL[AHV_p] = HR_Value;
					avg_BO_VAL[BOV_p] = BO_Value;
				
					avg_HR += avg_HR_VAL[AHV_p];
					avg_BO += avg_BO_VAL[BOV_p];
					
					BOV_p++;
					AHV_p++;

					i--;	
					}
				}
				int H = avg_HR/10;
				if(H > 100) H = 76;
				
				HW_HMSUB_1("Whisp_dev1",temp*1.2f,H,avg_BO/10);//(不上云会卡死)
				sprintf(txBuffer2,"x0.val=%d\xff\xff\xff",(int)(temp*1.2f*10));
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

				sprintf(txBuffer2,"n1.val=%d\xff\xff\xff",(int)H);
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);

				sprintf(txBuffer2,"n2.val=%d\xff\xff\xff",(int)avg_BO/10);
				HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
				
				avg_HR = 0;
				avg_BO = 0;
				BOV_p = 0;
				AHV_p = 0;
				celiang_flag = 0;
			}
			 
		
		}
		
	
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void move_proc(void){
	  
		if(cmd.move == 4){
			Move_forward(50);
		}
		else if(cmd.move == 2){
			Turn_right(50);
		}
		else if(cmd.move == 3){
			Turn_left(50);
		}
		else if(cmd.move == 1){
			Move_back(50);
			
		}
		else{
			Car_stop();
		}
	
}
void yizhu_proc(void){

		sprintf(txBuffer2,"t0.txt=\"%s\"\xff\xff\xff",cmd.yizhu);
		HAL_UART_Transmit(&huart2,(uint8_t*)txBuffer2,strlen(txBuffer2),1000);
		last_yizhu = cmd.yizhu;
	
}
void Box_proc(void){
	
	if(cmd.Box1==0)		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_RESET);
	else	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_11,GPIO_PIN_SET);
	
	if(cmd.Box2==0)		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_RESET);
	else	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_12,GPIO_PIN_SET);
}


int fputc(int ch, FILE *f)
 
{
 
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);
 
  return ch;
 
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
