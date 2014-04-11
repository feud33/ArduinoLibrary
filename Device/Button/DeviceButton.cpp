/*
 * DeviceButton.cpp
 *
 *  Created on: 3 mars 2014
 *      Author: FEUD7506
 */
#include "DeviceButton.h"
#include "Arduino.h"

/**
 * constructor
 * @param
 */
DeviceButton::DeviceButton( ) {
	this->pin=13;	// universal pin :-)
	this->setPin(pin);
}

/**
 * constructor
 * @param
 *   pin : the pin used to control the button
 */
DeviceButton::DeviceButton( byte pin ) {
	this->pin=pin;
	this->setPin(pin);
}

DeviceButton::~DeviceButton() {
}

/**
 * pin getter
 * @return
 *   pin : the pin set to control the button
 */
unsigned short DeviceButton::getPin() const {
	return pin;
}

/**
 * pin setter
 * @param
 *   pin : the pin number (INPUT)
 */
void DeviceButton::setPin(byte pin) {
	pinMode(pin, INPUT);
	this->pin=pin;
	digitalWrite(pin, HIGH);	// pull up
}

/**
 * value getter
 * @return
 *   value : true (the button is pressed) otherwise false
 */
bool DeviceButton::getValue() const {
	return ( digitalRead(this->pin) == LOW);
}

/**
 * toString to print the object
 * @param
 *   buffer : the buffer
 */
char *DeviceButton::toString(char *buffer) {
	sprintf(buffer, "[DeviceButton (%u)] pin=%u", (unsigned int)this, this->pin);
	return buffer;
}
