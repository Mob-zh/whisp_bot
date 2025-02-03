#include "Motor.h"
#include "tim.h"

void Set_MotorA(int zhuanxiang,int pwm_val){
	
	if(zhuanxiang==1){
		MotorA_INA(GPIO_PIN_SET);
		MotorA_INB(GPIO_PIN_RESET);
	}
	else{
		MotorA_INA(GPIO_PIN_RESET);
		MotorA_INB(GPIO_PIN_SET);
	}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_1,pwm_val);
}

void Set_MotorB(int zhuanxiang,int pwm_val){
	
	if(zhuanxiang==1){
		MotorB_INA(GPIO_PIN_RESET);
		MotorB_INB(GPIO_PIN_SET);
	}
	else{
		MotorB_INA(GPIO_PIN_SET);
		MotorB_INB(GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim2,TIM_CHANNEL_2,pwm_val);
}

void Set_MotorC(int zhuanxiang,int pwm_val){
	
	if(zhuanxiang==1){
		MotorC_INA(GPIO_PIN_RESET);
		MotorC_INB(GPIO_PIN_SET);
	}
	else{
		MotorC_INA(GPIO_PIN_SET);
		MotorC_INB(GPIO_PIN_RESET);
	}
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,pwm_val);
}
void Set_MotorD(int zhuanxiang,int pwm_val){
	
	if(zhuanxiang==1){
		MotorD_INA(GPIO_PIN_SET);
		MotorD_INB(GPIO_PIN_RESET);
	}
	else{
		MotorD_INA(GPIO_PIN_RESET);
		MotorD_INB(GPIO_PIN_SET);
	}
	__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_2,pwm_val);
}

void Move_forward(int speed){
	
	Set_MotorA(1,speed);
	Set_MotorB(1,speed);
	Set_MotorC(1,speed);
	Set_MotorD(1,speed);

}

void Turn_right(int speed){
	
	Set_MotorA(1,speed);
	Set_MotorB(1,speed);
	Set_MotorC(0,speed);
	Set_MotorD(0,speed);
}

void Turn_left(int speed){
	
	Set_MotorA(0,speed);
	Set_MotorB(0,speed);
	Set_MotorC(1,speed);
	Set_MotorD(1,speed);
}

void Move_back(int speed){
	
	Set_MotorA(0,speed);
	Set_MotorB(0,speed);
	Set_MotorC(0,speed);
	Set_MotorD(0,speed);
	
}

void Car_stop(void){
	
	Set_MotorA(1,0);
	Set_MotorB(1,0);
	Set_MotorC(1,0);
	Set_MotorD(1,0);
	
}

