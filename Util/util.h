/*
 * util.h
 *
 *  Created on: 21 mars 2014
 *      Author: FEUD7506
 */

#ifndef UTIL_H_
#define UTIL_H_
#include "Arduino.h"

byte bcd2dec(byte bcd);
byte dec2bcd(byte dec);
char *byteToBinary  (byte i, char *buffer);
void CUnit(const char *useCase, const bool result, byte *total, byte *KO);
void CUnitFinal(const  byte total, const byte KO);

#endif /* UTIL_H_ */
