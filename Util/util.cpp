/*
 * util.h
 *
 *  Created on: 21 mars 2014
 *      Author: FEUD7506
 */

#ifndef UTIL_H_
#define UTIL_H_
#include "Arduino.h"

byte bcd2dec(byte bcd) {
  return ((bcd / 16 * 10) + (bcd % 16));
}

byte dec2bcd(byte dec) {
  return ((dec / 10 * 16) + (dec % 10));
}

char *byteToBinary (byte b, char *buffer) {
	int i=8;
    buffer[i--]=0;
	for(;i>=0;i--) {
    	buffer[i] = '0' + (b & 1);
    	b >>= 1;
    };

	return buffer;
}

void CUnit(const char *useCase, const bool result, byte *total, byte *KO) {
	Serial.print(useCase);
	(*total)++;
	if( result) {
		Serial.println(" : passed");
	} else {
		Serial.println(" : failure");
		(*KO)++;
	}
}

void CUnitFinal(const byte total, const byte KO) {
	Serial.print("Tests run : ");
	Serial.print(total);
	Serial.print(", failures : ");
	Serial.println(KO);
}

#endif /* UTIL_H_ */
