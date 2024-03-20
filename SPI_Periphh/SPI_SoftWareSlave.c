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

	  GPIO_Config.GPIO_Pin = SPI_MISO;
    GPIO_Config.GPIO_Mode = GPIO_Mode_Out_PP; // MISO là OUTPUT
    GPIO_Config.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SPI_GPIO, &GPIO_Config);
	
    GPIO_Config.GPIO_Pin = SPI_SCK | SPI_MOSI | SPI_SS;
    GPIO_Config.GPIO_Mode = GPIO_Mode_IN_FLOATING; // SCK, MOSI, SS là INPUT
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

uint8_t SPI_Slave_Receive(void){
	uint8_t dataReceive =0x00;//0b11000000
	uint8_t temp = 0x00, i=0;
	while(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SS));
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK));
	for(i=0; i<8;i++){ 
		if(GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK)){
		while (GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK)) 
			temp = GPIO_ReadInputDataBit(SPI_GPIO, SPI_MOSI);
		dataReceive=dataReceive<<1;
		dataReceive=dataReceive|temp;
  }
	while(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SCK));
	}
	return dataReceive;
}



uint8_t DATA;
int main(){
	RCC_Config();
	GPIO_config();
	TIM_Config();
	while(1){
	   //neu nhu doc trang thai cua chan cs xuong 0 thi moi doc data
     //uint8_t DataTrans[] = {3, 7, 6, 4, 71, 23, 11, 14};
       if(!GPIO_ReadInputDataBit(SPI_GPIO, SPI_SS)){
			  DATA = SPI_Slave_Receive();
		 }
	}
	return 0;
}
