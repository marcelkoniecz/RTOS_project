/*
 * bma220.h
 *
 *  Created on: 6 Apr 2022
 *      Author: marcel
 */

#ifndef INC_BMA220_H_
#define INC_BMA220_H_

#include "stm32f3xx_hal.h"

#define BMA220_I2C_ADDR (0x0A<<1)

#define CHIP_ID 	0xDD
#define REVISION_ID 0x00

#define G2_MES		0x00
#define G4_MES		0x01
#define G8_MES		0x02
#define G16_MES		0x03

#define BMA220_REG_CHIP_ID 		0x00
#define BMA220_REG_REV_ID 		0x02
#define BMA220_REG_ACC_X 		0x04
#define BMA220_REG_ACC_Y 		0x06
#define BMA220_REG_ACC_Z		0x08

typedef struct{

	I2C_HandleTypeDef i2cHandle;

	//Acceleration data
	int8_t accData[3];
    float calcData[3];
}BMA220;

uint8_t BMA220_Init(BMA220 *dev, I2C_HandleTypeDef *i2cHandle);

HAL_StatusTypeDef BMA220_ReadAcc(BMA220 *dev);
HAL_StatusTypeDef BMA220_CalcAcc(BMA220 *dev);
void BMA220_CreateString(BMA220 *dev, char *str);

HAL_StatusTypeDef BMA220_ReadRegister(BMA220 *dev, uint8_t reg, uint8_t *data);
HAL_StatusTypeDef BMA220_ReadRegisters(BMA220 *dev, uint8_t reg, uint8_t *data,uint8_t lenght);
HAL_StatusTypeDef BMA220_WriteRegister(BMA220 *dev, uint8_t reg, uint8_t *data);

#endif /* INC_BMA220_H_ */
