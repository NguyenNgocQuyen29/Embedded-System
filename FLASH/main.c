#include "stm32f10x.h"                  // Device header
typedef enum{
	FLASH_ERROR = 0,
	FLASH_NOERROR
}FlashStatus;


void Flash_Unlock(void){
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}
void Flash_Erase(volatile uint32_t startAddr){
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){} //FLASH_SR_BSY
	
	/* Check Unlock sequences */
	if((FLASH->CR&FLASH_CR_LOCK)== FLASH_CR_LOCK){
		Flash_Unlock();
	}
	/* Write FLASH_CR_PER to 1*/
	FLASH->CR |= FLASH_CR_PER;
	/* Write into AR an address within the page to erase*/
	FLASH->AR |= startAddr;
	/* Write FLASH_CR_STRT to 1*/
	FLASH->CR |= FLASH_CR_STRT;
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){} //FLASH_SR_BSY
	/*delete register*/
	FLASH->CR &= ~(uint32_t)FLASH_CR_PER;
	FLASH->CR &= ~(uint32_t)FLASH_CR_STRT;
}
FlashStatus Flash_Write(volatile uint32_t startAddr, uint8_t* bufferWrite, uint32_t len){
	uint32_t count = 0U;
	/*Do dai cua buffer write vao khong the la 0, chieu dai cua buffer cung khong the la 0, truyen 16bit nen cung khong the la so le*/
	if((bufferWrite == 0x00U)||(len == 0U)|| len%2U != 0U){
		return FLASH_ERROR;
	}
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){}
	if((FLASH->CR&FLASH_CR_LOCK) == FLASH_CR_LOCK){
		Flash_Unlock();
	}
	/* Write FLASH_CR_PG to 1*/
	FLASH->CR |= FLASH_CR_PG;
	/* Perform half...*/
	for(count = 0U; count < len/2; count++){
		*(uint16_t*)(startAddr + count*2U) = *(uint16_t*)((uint32_t)bufferWrite + count*2U);
	}	
	while((FLASH->SR&FLASH_SR_BSY) == FLASH_SR_BSY){}
	/* Clear PG bit in the FLASH_CR register*/
		FLASH->CR &= ~(uint32_t)FLASH_CR_PG;
		return FLASH_NOERROR;
}
FlashStatus Flash_Read(volatile uint32_t startAddr, uint8_t* bufferRead, uint32_t len){
	if((startAddr==0x00U)||(len==0U)){
		return FLASH_ERROR;
	}
	do{
	if((startAddr%4U == 0U)&& ((uint32_t)bufferRead%4U==0U) && (len >= 4U)){
		*(uint32_t*)((uint32_t)bufferRead) = *(uint32_t*)(startAddr);
		bufferRead = bufferRead + 4U;
		len = len - 4U;
	}else if((startAddr%2U)==0U && ((uint32_t)bufferRead%2U==0U) && (len >= 2U)){
		*(uint16_t*)((uint32_t)bufferRead) = *(uint16_t*)(startAddr);
		bufferRead = bufferRead + 2U;
		startAddr = startAddr + 2U;
		len = len - 2U;
	}else{
		*(uint8_t*)(bufferRead) = *(uint8_t*)(startAddr);
		bufferRead = bufferRead + 1U;
		startAddr = startAddr + 1U;
		len = len - 1U;
	}
}while(len > 0U);
	return FLASH_NOERROR;
} 
uint8_t dataWrite[8] = {2,9,1,0,2,0,0,2};
int main(){
	uint8_t dataBuffer[8] = {0};
	Flash_Erase((uint32_t)0x08001000);
	if(FLASH_NOERROR != Flash_Write((uint32_t)0x08001000,dataWrite,8)){
		while(1);
	}
	if(FLASH_NOERROR != Flash_Read((uint32_t)0x08001000,dataBuffer,8)){
		while(1);
	}
	Flash_Erase((uint32_t)0x08001000);
	return 0;
}
