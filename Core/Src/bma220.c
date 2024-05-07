/*
 * bma220.c
 *
 *  Created on: 6 Apr 2022
 *      Author: marcel
 */
#include "bma220.h"
#include "i2c.h"
#include <string.h>
#include <stdio.h>

uint8_t BMA220_Init(BMA220 *dev, I2C_HandleTypeDef *i2cHandle){
	dev->i2cHandle=hi2c1;

	dev->accData[0]=0;
	dev->accData[1]=0;
	dev->accData[2]=0;
	//dev->accData[0]=0.3f;
	//dev->accData[1]=0.3f;
	//dev->accData[2]=0.3f;

	uint8_t errNum=0; //Liczba bledow
	HAL_StatusTypeDef status;

	uint8_t regData;

	//Checking chip id
	status=BMA220_ReadRegister(dev,BMA220_REG_CHIP_ID,&regData);
	errNum+=(status!=HAL_OK);

	if(regData!=CHIP_ID)
		return 255;

	status =BMA220_ReadRegister(dev,BMA220_REG_REV_ID,&regData);
	errNum+=(status!=HAL_OK);

	if(regData!=REVISION_ID)
		return 255;

	//Filter

	regData=0x3;
	status =BMA220_ReadRegister(dev,0x20,&regData);
	errNum+=(status!=HAL_OK);

	//Setting 4g

	regData=G4_MES;
	status =BMA220_ReadRegister(dev,0x22,&regData);
	errNum+=(status!=HAL_OK);

	return errNum;
}

HAL_StatusTypeDef BMA220_CalcAcc(BMA220 *dev){
    dev->calcData[0] = dev->accData[0]*0.0625*9.81;
    dev->calcData[1] = dev->accData[1]*0.0625*9.81;
    dev->calcData[2] = dev->accData[2]*0.0625*9.81;
    return HAL_OK;
}

void BMA220_CreateString(BMA220 *dev, char *str){
     uint8_t *FChar = "X "; //First char
      uint8_t *EChar = " \n"; //End chars

      static uint8_t help[30];
//
      strcpy(str, FChar); //Copy "X " to bufor
      sprintf(help,"%.3f %.3f %.3f \n",acc.calcData[0],acc.calcData[1],acc.calcData[2]);
      strcat(str, help); 	//Copy message to bufor
////      CRC8 = CalculateCRC8(buf, strlen(buf));
////      sprintf(help, " %X", CRC8);
////      strcat(buf, help);
      strcat(str, EChar); //Copy end chars to buf
}

HAL_StatusTypeDef BMA220_ReadAcc(BMA220 *dev){

	int8_t data;
	uint8_t status;
	uint8_t errNum;
	status=BMA220_ReadRegister(dev,BMA220_REG_ACC_X,&data);
	errNum+=(status!=HAL_OK);
//	dev->accData[0]=data*0.0625*9.81;
	dev->accData[0]=(data>>2);
	//dev->accData[0]=data;
	status=BMA220_ReadRegister(dev,BMA220_REG_ACC_Y,&data);
	errNum+=(status!=HAL_OK);
//	dev->accData[1]=data*0.0625*9.81;
	dev->accData[1]=(data>>2);
	//dev->accData[1]=data;
	status=BMA220_ReadRegister(dev,BMA220_REG_ACC_Z,&data);
	errNum+=(status!=HAL_OK);
//	dev->accData[2]=data*0.0625*9.81;
	dev->accData[2]=(data>>2);
	//dev->accData[2]=data;
	return errNum;

}


HAL_StatusTypeDef BMA220_ReadRegister(BMA220 *dev, uint8_t reg, uint8_t *data){
	return HAL_I2C_Mem_Read(&hi2c1, BMA220_I2C_ADDR,reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}

HAL_StatusTypeDef BMA220_ReadRegisters(BMA220 *dev, uint8_t reg, uint8_t *data, uint8_t lenght){
	return HAL_I2C_Mem_Read(&hi2c1, BMA220_I2C_ADDR,reg, I2C_MEMADD_SIZE_8BIT, data, lenght, HAL_MAX_DELAY);
}

HAL_StatusTypeDef BMA220_WriteRegister(BMA220 *dev, uint8_t reg, uint8_t *data){
	return HAL_I2C_Mem_Write(&hi2c1, BMA220_I2C_ADDR,reg, I2C_MEMADD_SIZE_8BIT, data, 1, HAL_MAX_DELAY);
}


