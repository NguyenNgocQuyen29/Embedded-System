#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


//1. Config RCC
void RCC_Config(){
	//turn on clock for portA,C
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}
//2 Config GPIO
void GPIO_Config(){
	GPIO_InitTypeDef config;
	//button A
	config.GPIO_Pin = GPIO_Pin_0;
	config.GPIO_Mode = GPIO_Mode_IPD; //input pull down: default status of button is 0
	config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &config);
	
	//led PC13
	config.GPIO_Pin = GPIO_Pin_13;
	config.GPIO_Mode = GPIO_Mode_Out_PP; //output push pull
	config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &config);
	
}
//3. delay 
void delay(__IO uint32_t timedelay){ 
	for(int i=0; i<timedelay; i++){}
}

int main(){
	RCC_Config();
	GPIO_Config();
	while(1){
		//trang thai mac dinh cua nut nhat la 0
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)){ //read status of PA0
			while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0))//wait den khi trang thai cua PA0 la 0
				if(GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13)){ //doc trang thai cua PC13 neu no dang sang thi tat
				  GPIO_ResetBits(GPIOC, GPIO_Pin_13);} 
				else{
					GPIO_SetBits(GPIOC, GPIO_Pin_13); //neu no dang tat thi sang 
				}
			}
		}
	return 0;
}