#include "stm32f10x.h"                  // Device header
#include "stm32f10x_rcc.h"              // Device:StdPeriph Drivers:RCC
#include "stm32f10x_gpio.h"             // Device:StdPeriph Drivers:GPIO

#define SPI_SCK       GPIO_Pin_0
#define SPI_MISO      GPIO_Pin_1
#define SPI_MOSI      GPIO_Pin_2
#define SPI_SS        GPIO_Pin_3
#define SPI_GPIO      GPIOA
#define SPI_RCC       RCC_APB2Periph_GPIOA



//1. Fuction Delay
void delay_ms(int time){
	TIM_SetCounter(TIM2,0);
	  while(TIM_GetCounter(TIM2)<time*10){}
}
//2. RCC
void RCC_Config(){
	RCC_APB2PeriphClockCmd(SPI_RCC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}
//3. Cau hinh timer
void TIM_Config(){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	TIM_TimeBaseInitStruct.TIM_Prescaler = 7200-1;
	TIM_TimeBaseInitStruct.TIM_Period = 0xFFFF-1;
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
	TIM_Cmd(TIM2, ENABLE);
}
//4.Cau hinh GPIO
void GPIO_config(){
	GPIO_InitTypeDef GPIO_Config;
	GPIO_Config.GPIO_Pin = SPI_SCK|SPI_MOSI|SPI_SS;
	GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_Config);
	
	GPIO_Config.GPIO_Pin = SPI_MISO;
	GPIO_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(SPI_GPIO, &GPIO_Config);
}
//5. Clock
void clock(){
	GPIO_WriteBit(SPI_GPIO, SPI_SCK, Bit_SET);
	delay_ms(4);
	GPIO_WriteBit(SPI_GPIO, SPI_SCK, Bit_RESET);
	delay_ms(4);
}
//6. Khoi tao SPI
void SPI_Initt(){
	//tang thai mac dinh cua SS la 1
	GPIO_WriteBit(SPI_GPIO,SPI_SCK, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO,SPI_MOSI, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO,SPI_MISO, Bit_RESET);
	GPIO_WriteBit(SPI_GPIO,SPI_SS, Bit_SET);
}

//7. Truyen du lieu: Keo CS = 0, (truyen bit, dich bit, clock), keo CS = 1
void SPI_Transmit(uint8_t data){
	uint8_t tempdata;
	uint8_t mask = 0x80;
	//keo chan CS xuong 0
	GPIO_WriteBit(SPI_GPIO, SPI_SS, Bit_RESET);
	delay_ms(1);
	//truyen du lieu
	for(int i = 0;i<8;i++){
		tempdata = data & mask;  //truyen bit cao nhat
		if(tempdata) //neu bit cao nhat la 1 thi truyen 1
		{
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI, Bit_SET);
			delay_ms(1);
		}else
		{
			GPIO_WriteBit(SPI_GPIO, SPI_MOSI, Bit_RESET);
			delay_ms(1);
		}
		data<<=1; //dich 1 bit
		clock();     
	}
	//keo ve 1
	GPIO_WriteBit(SPI_GPIO, SPI_SS, Bit_SET);
	delay_ms(1);	
}
uint8_t DataTrans[] = {3, 7, 6, 4, 71, 23, 11, 14};
int main(){
	RCC_Config();
	GPIO_config();
	TIM_Config();
	SPI_Initt();
	while(1){
		for(int i = 0; i< 8; i++){
			SPI_Transmit(DataTrans[i]);
			delay_ms(500);
		}
	}
	return 0;
}
