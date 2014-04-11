/*
 * DeviceLButton.h
 *
 *  Created on: 3 mars 2014
 *      Author: FEUD7506
 */

#ifndef DEVICEBUTTON_H_
#define DEVICEBUTTON_H_
#include "Arduino.h"

class DeviceButton {
  public:
	DeviceButton();
	DeviceButton(byte pin);
	virtual ~DeviceButton();

	unsigned short getPin() const;	 		// pin getter
	void setPin(byte pin);					// set the button pin (INPUT)
  	bool getValue() const;					// is the button pressed ?

  	char *toString(char *buffer);			// logger

  private:
  	byte pin;								// pin number of the arduino board
};

#endif /* DEVICEBUTTON_H_ */
