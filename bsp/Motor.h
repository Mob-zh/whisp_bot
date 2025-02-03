#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "main.h"

#define MotorA_INA(x) HAL_GPIO_WritePin(GPIOH,GPIO_PIN_4,x) 
#define MotorA_INB(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,x) 
#define MotorB_INA(x) HAL_GPIO_WritePin(GPIOB,GPIO_PIN_11,x) 
#define MotorB_INB(x) HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,x) 
#define MotorC_INA(x) HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,x) 
#define MotorC_INB(x) HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,x) 
#define MotorD_INA(x) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,x) 
#define MotorD_INB(x) HAL_GPIO_WritePin(GPIOC,GPIO_PIN_5,x) 

//控制单个车轮转向
void Set_MotorA(int zhuanxiang,int pwm_val);
void Set_MotorB(int zhuanxiang,int pwm_val);
void Set_MotorC(int zhuanxiang,int pwm_val);
void Set_MotorD(int zhuanxiang,int pwm_val);


//前进控制
void Move_forward(int speed);
void Turn_right(int speed);
void Turn_left(int speed);
void Move_back(int speed);
void Car_stop(void);



#endif
