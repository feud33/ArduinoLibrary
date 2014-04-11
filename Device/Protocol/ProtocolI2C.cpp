/*
 * ProtocolI2C.cpp
 *
 *  Created on: 21 mars 2014
 *      Author: FEUD7506
 */

#include "ProtocolI2C.h"
#include "Wire.h"
#include "Arduino.h"
#include "util.h"

bool ProtocolI2C::init=false;

ProtocolI2C::ProtocolI2C() {
	if( this->init == false ) {
		this->init=true;
	    Wire.begin();
	}
}

ProtocolI2C::~ProtocolI2C() {
}

bool ProtocolI2C::Check(byte I2CAddress) const {
	Wire.beginTransmission(I2CAddress);
	return (Wire.endTransmission() == 0);
}

bool ProtocolI2C::isInit() const {
	return this->init;
}

bool ProtocolI2C::get(byte I2CAddress, byte subAddress, byte length, byte *buffer) const {
	byte nbRead=0;

	// Offset
	Wire.beginTransmission(I2CAddress);
	Wire.write(subAddress);
	Wire.endTransmission();

	// Read
	Wire.requestFrom(I2CAddress, length);
	while(Wire.available()) buffer[nbRead++] = Wire.read();
	return (nbRead == length );
}

bool ProtocolI2C::set(byte I2CAddress, byte subAddress, byte length, byte *buffer) {
	byte i;

	Wire.beginTransmission(I2CAddress);
	Wire.write(subAddress);				// Offset
	i=Wire.write(buffer, length);		// write
	Wire.endTransmission();

	return (i == length );
}

