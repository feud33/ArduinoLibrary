/*
 * DeviceLed.h
 *
 *  Created on: 3 mars 2014
 *      Author: FEUD7506
 */

#ifndef DEVICELED_H_
#define DEVICELED_H_
#include "Arduino.h"

#define DEVICELED_ON true
#define DEVICELED_OFF false

class DeviceLed {
  public:
	DeviceLed();
	DeviceLed(byte pin);
	virtual ~DeviceLed();

	unsigned short getPin() const; 			// pin getter
	void setPin(byte pin);					// set the led pin (OUTPUT)
  	bool getValue() const;					// to get the value previously set
  	void setValue(bool value);				// should be DEVICELED_ON or DEVICELED_OFF

  	void switchOn();						// set to DEVICELED_ON
  	void switchOff();						// set to DEVICELED_OFF
  	void switchValue();						// Switch ON to OFF or OFF to ON
  	char *toString(char *buffer);			// logger

  private:
  	byte pin;								// pin number of the arduino board
  	bool value;								// pin value, should be HIGH or LOW
};

#endif /* DEVICELED_H_ */

