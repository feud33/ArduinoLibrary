/*
 * deviceClock.cpp
 *
 *  Created on: 21 mars 2014
 *      Author: FEUD7506
 */

#include "DeviceClock.h"
#include "Calendar.h"
#include "util.h"

DeviceClock::DeviceClock () : ProtocolI2C() {
}

DeviceClock::~DeviceClock() {
}

bool DeviceClock::Check () const {
	return ProtocolI2C::Check(this->id);
}

bool DeviceClock::get(Calendar *time) const {
	return this->get(this->clockStackId, time);
}


bool DeviceClock::set(const Calendar &time) {
	return this->set(this->clockStackId, time);
}

bool DeviceClock::get(byte stackId, Calendar *time) const {
	bool readResult;
	byte buffer[this->stackSize];

	readResult=ProtocolI2C::get(this->id, this->clockStackId, this->stackSize, buffer);
	if( readResult ) {
//		(*time).setSecond(bcd2dec(buffer[0]) & 0b01111111 );	// first bit to start clock
//		(*time).setMinute(bcd2dec(buffer[1]));
//		(*time).setHour(bcd2dec(buffer[2]));
//		(*time).setWeekDay(bcd2dec(buffer[3]));
//		(*time).setDay(bcd2dec(buffer[4]));
//		(*time).setMonth(bcd2dec(buffer[5]));
//		(*time).setYear(bcd2dec(buffer[6]));
	}
	return readResult;
}


bool DeviceClock::set(byte stackId, const  Calendar &time) {
	byte buffer[this->stackSize];

//	buffer[0]=0b01111111 & dec2bcd(time.getSecond());
//	buffer[1]=dec2bcd(time.getMinute());
//	buffer[2]=dec2bcd(time.getHour());
//	buffer[3]=dec2bcd(time.getWeekDay());
//	buffer[4]=dec2bcd(time.getDay());
//	buffer[5]=dec2bcd(time.getMonth());
//	buffer[6]=dec2bcd(time.getYear());

	return ProtocolI2C::set(this->id, stackId, this->stackSize, buffer);
}
