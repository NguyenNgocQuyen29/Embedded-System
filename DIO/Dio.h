/*
* File: Dio.h
* Author: Nguyen Thi Ngoc Quyen
* Date: 31/05/2024
* Description: Specification of DIO Driver
*/

#ifndef DIO_H
#define DIO_H
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Device:StdPeriph Drivers:GPIO
/* Type Imported from Std_Types.h file*/
typedef struct{
	uint16_t vendorID;
	uint16_t moduleID;
	uint8_t sw_major_version;
	uint8_t sw_minor_version;
	uint8_t sw_patch_version;
}Std_VersionInfoType;

/*
* Define all the channels for Autosar
* Channels will be numbered from 0 to the max number of pin.
* For example: #define DIO_CHANNEL_0 	(Dio_ChannelType)0 //GPIOA, Pin 0
ChannelId DIO_CHANNEL_0 to 15 will be in GPIOA,the pin from ChannelId 16 to 31 will be in GPIOB and so on...
*/
#define DIO_CHANNEL_0 	(Dio_ChannelType)0 //GPIOA,Pin 0
#define DIO_CHANNEL_1 	(Dio_ChannelType)1 
#define DIO_CHANNEL_2 	(Dio_ChannelType)2 
#define DIO_CHANNEL_3 	(Dio_ChannelType)3 
#define DIO_CHANNEL_4 	(Dio_ChannelType)4 
#define DIO_CHANNEL_5 	(Dio_ChannelType)5 
#define DIO_CHANNEL_6 	(Dio_ChannelType)6 
#define DIO_CHANNEL_7 	(Dio_ChannelType)7 
#define DIO_CHANNEL_8 	(Dio_ChannelType)8 
#define DIO_CHANNEL_9 	(Dio_ChannelType)9 
#define DIO_CHANNEL_10 	(Dio_ChannelType)10 
#define DIO_CHANNEL_11 	(Dio_ChannelType)11
#define DIO_CHANNEL_12 	(Dio_ChannelType)12
#define DIO_CHANNEL_13 	(Dio_ChannelType)13 
#define DIO_CHANNEL_14 	(Dio_ChannelType)14 
#define DIO_CHANNEL_15 	(Dio_ChannelType)15 //GPIOA, Pin 15

#define DIO_CHANNEL_16 	(Dio_ChannelType)16 //GPIOB,Pin 0
#define DIO_CHANNEL_17 	(Dio_ChannelType)17 
#define DIO_CHANNEL_18 	(Dio_ChannelType)18 
#define DIO_CHANNEL_19 	(Dio_ChannelType)19 
#define DIO_CHANNEL_20 	(Dio_ChannelType)20 
#define DIO_CHANNEL_21 	(Dio_ChannelType)21 
#define DIO_CHANNEL_22 	(Dio_ChannelType)22 
#define DIO_CHANNEL_23 	(Dio_ChannelType)23 
#define DIO_CHANNEL_24 	(Dio_ChannelType)24 
#define DIO_CHANNEL_25 	(Dio_ChannelType)25
#define DIO_CHANNEL_26 	(Dio_ChannelType)26 
#define DIO_CHANNEL_27 	(Dio_ChannelType)27
#define DIO_CHANNEL_28 	(Dio_ChannelType)28
#define DIO_CHANNEL_29 	(Dio_ChannelType)29 
#define DIO_CHANNEL_30 	(Dio_ChannelType)30 
#define DIO_CHANNEL_31 	(Dio_ChannelType)31 //GPIOB, Pin 15

/* 
* Câc ham ben stm32 thi truyen vao port va pin nhung ben autosar chi la channel thoi nen minh dung 2 ham sau day de tinh port va bit
*/
#define DIO_GET_PORT_FROM_CHANNEL_ID(_channelId) (_channelId/16)
#define DIO_GET_BIT_FROM_CHANNEL_ID(_channelId)  (1 << (_channelId%16)

#define Dio_ChannelType    uint8_t
#define Dio_PortType       uint16_t
#define Dio_PortLevelType  uint16_t
#define Dio_LevelType      uint8_t

#define STD_LOW    (uint8_t)0x00
#define STD_HIGH   (uint8_t)0x01

/* Define ypur modu;e's version information*/
#define DIO_MODULE_ID           (123)
#define DIO_VENDOR_ID           (456)
#define DIO_SW_MAJOR_VERSION    (1)
#define DIO_SW_MINOR_VERSION    (2)
#define DIO_SW_PATCH_VERSION    (3)

typedef  GPIO_TypeDef* GPIO_TypeDefPtr;

typedef struct{
	uint16_t mask;
	uint8_t offset;
	Dio_PortType port;
}Dio_ChannelGroupType;

Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId);
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId);
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId);
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo);
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level);


#endif