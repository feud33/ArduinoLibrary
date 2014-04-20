/*
 * DeviceRelay.h
 *
 *  Created on: 17 april 2014
 *      Author: FEUD7506
 */

#ifndef DEVICERELAY_H_
#define DEVICERELAY_H_
#include "Arduino.h"

#define DEVICERELAY_ON true
#define DEVICERELAY_OFF false
#define DEVICERELAY_DELAY 10000

class DeviceRelay {
  public:
	DeviceRelay();
	DeviceRelay(byte pin);
	virtual ~DeviceRelay();

	unsigned short getPin() const; 			// pin getter
	void setPin(byte pin);					// set the relay pin (OUTPUT)
  	bool getValue() const;					// to get the value previously set
  	void setValue(bool value);				// should be DEVICERELAY_ON or DEVICERELAY_OFF

  	bool switchOn();						// set to DEVICERELAY_ON
  	bool switchOff();						// set to DEVICERELAY_OFF
  	char *toString(char *buffer);			// logger

  private:
  	byte pin;								// pin number of the arduino board
  	bool value;								// pin value, should be HIGH or LOW
  	unsigned long time;						//
};

void unitaryTestDeviceRelay();

#endif /* DEVICERELAY_H_ */

