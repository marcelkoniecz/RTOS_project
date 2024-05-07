/*
 * crc8.h
 *
 *  Created on: Apr 19, 2022
 *      Author: marcel
 */

#ifndef INC_CRC8_H_
#define INC_CRC8_H_

#define POLYNOMIAL 0x161

uint16_t CRCSingleByte(uint16_t data);
uint16_t CalculateCRC8(uint8_t *ptr, uint32_t len);
void printValues(BMA220 acc);

#endif /* INC_CRC8_H_ */
