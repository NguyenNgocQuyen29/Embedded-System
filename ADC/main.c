#include "stm32f10x_adc.h"              // Device:StdPeriph Drivers:ADC
#include "stm32f10x_rcc.h"              // Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Device:StdPeriph Drivers:GPIO
#include "stm32f10x_tim.h"              // Device:StdPeriph Drivers:TIM
#include "stm32f10x.h"                  // Device header
#include "kalman.h"



void delay_us(uint32_t delay){
	TIM_SetCounter(TIM2,0);
	while(TIM_GetCounter(TIM2)<delay){}
}
void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM_Config;
	TIM_Config.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_Config.TIM_Period = 20000-1;
	TIM_Config.TIM_Prescaler = 72-1;
	
	TIM_TimeBaseInit(TIM2, &TIM_Config);
	TIM_Cmd(TIM2, ENABLE);
}
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1|RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
void GPIO_Config(){
	GPIO_InitTypeDef GPIO_Config;
	GPIO_Config.GPIO_Pin = GPIO_Pin_0;
	GPIO_Config.GPIO_Mode = GPIO_Mode_AIN; //analog input
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_Config);
}
void ADC_Config(){
	ADC_InitTypeDef ADC_Config;
	ADC_Config.ADC_Mode = ADC_Mode_Independent; //chuyen doi doc lap
	ADC_Config.ADC_NbrOfChannel = 1;//su duung bao nhieu kenh
	ADC_Config.ADC_ContinuousConvMode = ENABLE; //doc du lieu lien tuc
	ADC_Config.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_Config.ADC_DataAlign = ADC_DataAlign_Right; //12bit, nhung thanh ghi 16 bit lan nen minh chon can le trai hay phai
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
	ADC_Init(ADC1, &ADC_Config);

	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE); //start convert
}
uint16_t val,valUpdate;
float angle;
int main(){
	RCC_Config();
	TIM_Config();
	GPIO_Config();
	ADC_Config();
	SimpleKalmanFilter(1, 2, 0.001);
	while(1){
			val = ADC_GetConversionValue(ADC1); //get value tren kenh ADC
			valUpdate = (uint16_t)updateEstimate((float)val);
		  angle = (float)(valUpdate*180.0/4095.0);   //convert from adc to angle
		  delay_us(1000);

	}
}

