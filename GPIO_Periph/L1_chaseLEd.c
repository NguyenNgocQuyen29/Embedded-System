#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

//1. Config Rcc
void RCC_Config(){
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}
//2. Config GPIO
void GPIO_Config(){
	GPIO_InitTypeDef config;
	config.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	config.GPIO_Mode = GPIO_Mode_Out_PP;
	config.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &config);
}
//3. delay
void delay(__IO uint32_t timedelay){ 
	for(int i=0; i<timedelay; i++){}
}
//4. nháy duoi 
void chaseLed(){
	uint16_t val = 0x0001; //PC0: sáng 0000 0000 0000 0001
	GPIO_Write(GPIOC, val); 
	//delay(500);
	//3 lan
	for(int i = 0; i<4;i++){
		for(int j = 0; j < 5; j++){
			val <<= 1; //dich trai
			GPIO_Write(GPIOC, val);//ghi gia tri vao
			delay(500);
		}
	}
}

int main(){
	RCC_Config();
	GPIO_Config();
	chaseLed();
	while(1){
		
	}
	return 0;
}
