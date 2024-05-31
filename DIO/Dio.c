/*
* File: Dio.c
* Author: Nguyen Thi Ngoc Quyen
* Date: 31/05/2024
* Description: Specification of DIO Driver
*/

#include "Dio.h"
const GPIO_TypeDefPtr GPIO_ports[] = {GPIOA, GPIOB, GPIOC, GPIOD, GPIOE, GPIOD};
/*
* Function: Dio_ReadPort
* Description: returns the level of all channels of that port
* input:
* 	PortId - id of DIO Port
* Output:
*		Dio_PortLevelType - level of all channels of that port
*/
Dio_PortLevelType Dio_ReadPort(Dio_PortType PortId){
	Dio_LevelType level = 0x0000;
	level = GPIO_ReadInputData(GPIO_ports[PortId]);
	return level;
}
/*
* Function: Dio_WritePort
* Description: Service to set a value oSf the port
* Input:
* 	PortId - Id of DIO Port
*		Level - Value to be written
* Output:
*		none
*/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level){
	GPIO_Write(GPIO_ports[PortId], Level);
}
/*
* Function: Dio_ReadChannel
* Description: Return the value of the specified DIO channel.
* Input:
* 	ChannelId - ID of DIO channel
* Output:
*		return Dio_Level_Type 
					STD_HIGH: The physical level of the corresponding Pin is STD_HIGH
					STD_LOW: The physical level of he corresponding Pin is STD_LOW
*/
Dio_LevelType Dio_ReadChannel (Dio_ChannelType ChannelId){
	Dio_LevelType level;
	Dio_PortLevelType portVal = Dio_ReadPort(DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId));
	Dio_PortLevelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);
	if((portVal && bit != STD_LOW)){
		level = STD_HIGH;
	}else{
		level = STD_LOW;
	}
	return level;
}
void Dio_WriteChannel (Dio_ChannelType ChannelId, Dio_LevelType Level){
	Dio_PortType port = DIO_GET_PORT_FROM_CHANNEL_ID(ChannelId);
	Dio_ChannelType bit = DIO_GET_BIT_FROM_CHANNEL_ID(ChannelId);
	
	Dio_PortLevelType portVal = Dio_ReadPort(port);
	if(Level == STD_HIGH){
		portVal |= bit;
	}else{
		portVal &= ~bit;
	}
	Dio_WritePort(port, portVal);
}
/*
* Function: Dio_FlipChannel
* Description: Service to flip the level of a channel and return the level of the channel after flip
* Input:
* 	ChannelId - ID of DIO channel.
* Output:
*		return Dio_Level_Type 
					STD_HIGH: The physical level of the corresponding Pin is STD_HIGH
					STD_LOW: The physical level of he corresponding Pin is STD_LOW
*/
Dio_LevelType Dio_FlipChannel(Dio_ChannelType ChannelId){
	Dio_LevelType level;
	level = Dio_ReadChannel(ChannelId);
	if(level == STD_HIGH){
		level = STD_LOW;
	}else{
		level = STD_HIGH;
	}
	Dio_WriteChannel(ChannelId, level);
}
/*
* Function: Dio_GetVersionInfo
* Description: Service to get the version information of this module
* Input:
* 	VersionInfo - Pointer to where to store the version information of this module.
* Output:
*		none
*/
void Dio_GetVersionInfo(Std_VersionInfoType* VersionInfo){
	if(VersionInfo != 0){
		VersionInfo->moduleID = DIO_MODULE_ID;
		VersionInfo->vendorID = DIO_VENDOR_ID;
		VersionInfo->sw_major_version = DIO_SW_MAJOR_VERSION;
		VersionInfo->sw_minor_version = DIO_SW_MINOR_VERSION;
		VersionInfo->sw_patch_version = DIO_SW_PATCH_VERSION;
	}else{}
}
