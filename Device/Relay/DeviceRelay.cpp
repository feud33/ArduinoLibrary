/*
 * DeviceRelay.cpp
 *
 *  Created on: 17 april 2014
 *      Author: FEUD7506
 */
#include "DeviceRelay.h"
#include "Arduino.h"
#include "util.h"


/**
 * constructor
 * @param
 */
DeviceRelay::DeviceRelay( ) {
	this->pin=13;	// universal pin :-)
	this->value=DEVICERELAY_OFF;
	this->setPin(pin);
	this->time=0;
}

/**
 * constructor
 * @param
 *   pin : the pin used to control the relay
 */
DeviceRelay::DeviceRelay( byte pin ) {
	this->pin=pin;
	this->value=DEVICERELAY_OFF;
	this->setPin(pin);
	this->time=0;
}

DeviceRelay::~DeviceRelay() {
}

/**
 * pin getter
 * @return
 *   pin : the pin set to control the relay
 */
unsigned short DeviceRelay::getPin() const {
	return pin;
}

/**
 * pin setter
 * @param
 *   pin : the pin number (OUTPUT)
 */
void DeviceRelay::setPin(byte pin) {
	pinMode(pin, OUTPUT);
	this->pin=pin;
	this->value=false;
	this->time=0;
	digitalWrite(pin, LOW); // relay is digital
}

/**
 * value getter
 * @return
 *   value : true if relay is switched on
 */
bool DeviceRelay::getValue() const {
	return value;
}

/**
 * value setter
 * @param
 *   value : true to switched on the relay
 */
void DeviceRelay::setValue(bool value) {
	this->value = value;
	if( value ) digitalWrite(this->getPin(), HIGH);
		else digitalWrite(this->getPin(), LOW);
}

/**
 * set Value to DEVICERELAY_ON, turn the relay on
 * @return true if relay is switched on
 */
bool DeviceRelay::switchOn() {
	if( (this->time) == 0 || ( this->time)+DEVICERELAY_DELAY < millis()) {
		this->setValue(DEVICERELAY_ON);
		return true;
	} else {
		return false;
	}
}

/**
 * set Value to DEVICERELAY_OFF, turn the relay off
 */
bool DeviceRelay::switchOff() {
	if( this->getValue() == DEVICERELAY_ON) {
		this->setValue(false);
		this->time=millis();
		return true;
	} else {
		return false;
	}
}


/**
 * to print the object
 * @param
 *   buffer : the buffer
 */
char *DeviceRelay::toString(char *buffer) {
	sprintf(buffer, "[DeviceRelay (%u)] pin=%u, value=%u, time=%u", (unsigned int)this, this->pin, this->value, (unsigned int)this->time);
	return buffer;
}


/**
 *
 * unitaryTestDeviceRelay is used for non regression unitary tests
 * Serial must be opened
 * @param
 */
void unitaryTestDeviceRelay() {
	byte KO=0, total=0;
	DeviceRelay myRelay1=DeviceRelay(13);
	DeviceRelay myRelay2=DeviceRelay();

	// default 13 init
	CUnit("UT1 (getPin)", (myRelay1.getPin() == 13), &total, &KO);
	CUnit("UT2 (getValue)", (myRelay1.getValue() == DEVICERELAY_OFF), &total, &KO);
	CUnit("UT3 (switchOn)", (myRelay1.switchOn() == true), &total, &KO); // Check if relay is on
	delay(500);
	CUnit("UT4 (switchOff)", (myRelay1.switchOff() == true), &total, &KO); // Check if relay is off
	delay(500);
	CUnit("UT5 (switchOn KO)", (myRelay1.switchOn() == false), &total, &KO); // Check if relay is off, not enough delay
	delay(DEVICERELAY_DELAY);
	CUnit("UT6 (switchOn)", (myRelay1.switchOn() == true), &total, &KO); // Check if relay is off, enough delay

	// default 12 init
	myRelay2.setPin(13);
	CUnit("UT11 (getPin)", (myRelay2.getPin() == 13), &total, &KO);
	CUnit("UT12 (getValue)", (myRelay2.getValue() == DEVICERELAY_OFF), &total, &KO);
	CUnit("UT13 (switchOn)", (myRelay2.switchOn() == true), &total, &KO); // Check if relay is on
	delay(500);
	CUnit("UT14 (switchOff)", (myRelay2.switchOff() == true), &total, &KO); // Check if relay is off
	delay(500);
	CUnit("UT15 (switchOn KO)", (myRelay2.switchOn() == false), &total, &KO); // Check if relay is off, not enough delay
	delay(DEVICERELAY_DELAY);
	CUnit("UT16 (switchOn)", (myRelay2.switchOn() == true), &total, &KO); // Check if relay is off, enough delay

	CUnitFinal(total, KO);
}

