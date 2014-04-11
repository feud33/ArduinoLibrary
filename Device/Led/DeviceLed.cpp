/*
 * DeviceLed.cpp
 *
 *  Created on: 3 mars 2014
 *      Author: FEUD7506
 */
#include "DeviceLed.h"
#include "Arduino.h"


/**
 * constructor
 * @param
 */
DeviceLed::DeviceLed( ) {
	this->pin=13;	// universal pin :-)
	this->value=DEVICELED_OFF;
	this->setPin(pin);
}

/**
 * constructor
 * @param
 *   pin : the pin used to control the led
 */
DeviceLed::DeviceLed( byte pin ) {
	this->pin=pin;
	this->value=DEVICELED_OFF;
	this->setPin(pin);
}

DeviceLed::~DeviceLed() {
}

/**
 * pin getter
 * @return
 *   pin : the pin set to control the led
 */
unsigned short DeviceLed::getPin() const {
	return pin;
}

/**
 * pin setter
 * @param
 *   pin : the pin number (OUTPUT)
 */
void DeviceLed::setPin(byte pin) {
	pinMode(pin, OUTPUT);
	this->pin=pin;
	this->value=false;
	digitalWrite(pin, LOW); // Led is digital
}

/**
 * value getter
 * @return
 *   value : true if led is switched on
 */
bool DeviceLed::getValue() const {
	return value;
}

/**
 * value setter
 * @param
 *   value : true to switched on the led
 */
void DeviceLed::setValue(bool value) {
	this->value = value;
	if( value ) digitalWrite(this->getPin(), HIGH);
		else digitalWrite(this->getPin(), LOW);
}

/**
 * set Value to DEVICELED_ON, turn the led on
 */
void DeviceLed::switchOn() {
	this->setValue(true);
}

/**
 * set Value to DEVICELED_OFF, turn the led off
 */
void DeviceLed::switchOff() {
	this->setValue(false);
}


/**
 * toggle the value from HIGH to LOW (or LOW to HIGH)
 */
void DeviceLed::switchValue() {
	if( this->getValue()) this->setValue(false);
	else this->setValue(true);
}




/**
 * to print the object
 * @param
 *   buffer : the buffer
 */
char *DeviceLed::toString(char *buffer) {
	sprintf(buffer, "[DeviceLed (%u)] pin=%u, value=%u", (unsigned int)this, this->pin, this->value);
	return buffer;
}
