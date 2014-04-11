/*
 * ProtocolI2C.h
 *
 *  Created on: 21 mars 2014
 *      Author: FEUD7506
 */

#ifndef PROTOCOLI2C_H_
#define PROTOCOLI2C_H_
#include "Arduino.h"

class ProtocolI2C {
public:
	ProtocolI2C();
	virtual ~ProtocolI2C();
	bool Check(byte I2CAddress) const;
	bool isInit() const;

	bool get(byte I2CAddress, byte subAdress, byte length, byte *buffer) const;
	bool set(byte I2CAddress, byte subAdress, byte length, byte *buffer);

private:
	static bool init;
};

#endif /* PROTOCOLI2C_H_ */
