/*
 * crc8.c
 *
 *  Created on: Apr 19, 2022
 *      Author: marcel
 */
#include <stdio.h>
#include <string.h>
#include "bma220.h"
#include "crc8.h"
#include "usart.h"

uint16_t CRCSingleByte(uint16_t data) {
	uint16_t poly = (POLYNOMIAL << 7);
	for (uint8_t i = 0; i < 8; i++) {
		if ((data & 0x8000) != 0)
			data ^= poly;
		data <<= 1;
	}
	return data;
}

uint16_t CalculateCRC8(uint8_t *ptr, uint32_t len) {
	uint16_t data = ptr[0] << 8;

	for (int i = 1; i < strlen(ptr); i++) {
		data |= ptr[i];
		data = CRCSingleByte(data);
	}
	data = CRCSingleByte(data);
	return (data >> 8);
}

void printValues(BMA220 acc) {
	uint8_t *FChar = "X "; //First char
	uint8_t *EChar = " \n"; //End chars
	uint16_t CRC8 = 0;
	static uint8_t buf[50];
	static uint8_t help[25];

	strcpy(buf, FChar); //Copy "X " to bufor
	sprintf(help,"%d %d %d",acc.accData[0],acc.accData[1],acc.accData[2]);
	strcat(buf, help); 	//Copy message to bufor
	CRC8 = CalculateCRC8(buf, strlen(buf));
	sprintf(help, " %X", CRC8);
	strcat(buf, help);
	strcat(buf, EChar); //Copy end chars to buf

	HAL_UART_Transmit(&huart2, buf, strlen(buf), 1000);
}
