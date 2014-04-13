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


/**
 * get : get Clock time
 * @param
 * Calendar address that will contain time
 * @return bool (false : trouble)
 */
bool DeviceClock::get(Calendar *time) const {
	return this->get(this->clockStackId, time);
}


/**
 * set : set the clock time
 * @param
 * Calendar time : time to set
 * @return bool (false : trouble)
 */
bool DeviceClock::set(const Calendar &time) {
	return this->set(this->clockStackId, time);
}


/**
 * get : get the time stored at the stackId address. Please use DeviceClock constant
 * @param
 * stackId : the address to read
 * Calendar *time : pointer to time juste read
 * @return bool (false : trouble)
 */
bool DeviceClock::get(const byte stackId, Calendar *time) const {
	bool readResult;
	byte buffer[this->stackSize];

	readResult=ProtocolI2C::get(this->id, stackId, this->stackSize, buffer);
	if( readResult ) {
		(*time).set(bcd2dec(buffer[0]) & 0b01111111,	// first bit to start clock
					bcd2dec(buffer[1]),			// minute
					bcd2dec(buffer[2]),			// hour
					bcd2dec(buffer[4]),			// day
					bcd2dec(buffer[5]),			// month
					bcd2dec(buffer[6]));		// year
	}
	return readResult;
}


/**
 * set : set the time at the stackId address. Please use DeviceClock constant
 * @param
 * stackId : the address to set
 * Calendar time : the time to set
 * @return bool (false : trouble)
 */
bool DeviceClock::set(const byte stackId, const Calendar &time) {
	byte buffer[this->stackSize];

	time.get( &(buffer[0]), &(buffer[1]), &(buffer[2]), &(buffer[3]), &(buffer[4]), &(buffer[5]), &(buffer[6]));

	buffer[0]=dec2bcd(buffer[0]);
	buffer[1]=dec2bcd(buffer[1]);
	buffer[2]=dec2bcd(buffer[2]);
	buffer[3]=dec2bcd(buffer[3]);
	buffer[4]=dec2bcd(buffer[4]);
	buffer[5]=dec2bcd(buffer[5]);
	buffer[6]=dec2bcd(buffer[6]);

	return ProtocolI2C::set(this->id, stackId, this->stackSize, buffer);
}


/**
 * return true if the I2C protocol is opened and if the clock is running
 * @param
 * @return bool (true : OK)
 */
bool DeviceClock::isOK () const {
	bool isOK=true;
	Calendar time1=Calendar();
	Calendar time2=Calendar();

	isOK=ProtocolI2C::Check(this->id);
	if( isOK) {
		this->get(this->clockStackId, &time1);
		delay(1000);
		this->get(this->clockStackId, &time2);
		isOK=( time2.getTimestamp() > time1.getTimestamp());
	}
	return isOK;
}


/**
 * return true if the battery is OK
 * @param
 * @return bool (true : OK)
 */
bool DeviceClock::isBatteryOK () const {
	Calendar myTime=Calendar();

	this->get(this->ctrlStackId, &myTime);
	return ( myTime.getTimestamp() == this->refTime);
}


/**
 * full reset of the clock.
 * 1. set the clock time
 * 2. if isInit is true, set the initStackId to the current time
 * 3. if battery is KO, set all the other stack to refTime value
 * @param
 *   currentTime : the current time to set
 *   isInit : to set starting time
 * @return bool (true : OK)
 */
bool DeviceClock::reset ( Calendar currentTime, bool isInit) {
	Calendar refTime=Calendar();
	bool isOK, isBatOK;

	refTime.setTimestamp(this->refTime);

	isOK=this->set(this->clockStackId, currentTime.getTimestamp() );
	if( isOK && isInit ) this->set(this->initStackId, currentTime.getTimestamp());

	isBatOK=(isOK && this->isBatteryOK());
	if( isOK && isBatOK ) isOK=this->set(this->ctrlStackId, refTime.getTimestamp());
	if( isOK && isBatOK ) isOK=this->set(this->stackId3, refTime.getTimestamp());
	if( isOK && isBatOK ) isOK=this->set(this->stackId4, refTime.getTimestamp());
	if( isOK && isBatOK ) isOK=this->set(this->stackId5, refTime.getTimestamp());
	if( isOK && isBatOK ) isOK=this->set(this->stackId6, refTime.getTimestamp());
	if( isOK && isBatOK ) isOK=this->set(this->stackId7, refTime.getTimestamp());
	if( isOK && isBatOK ) isOK=this->set(this->stackId8, refTime.getTimestamp());
	return isOK;
}


/**
 *
 * unitaryTestDeviceClock is used for non regression unitary tests
 * Serial must be opened
 * @param
 */
void unitaryTestDeviceClock() {
	byte KO=0, total=0;
	DeviceClock myClock=DeviceClock();
	Calendar time1=Calendar();
	Calendar time2=Calendar();

	// set time
//	time1.setTimestamp(myClock.refTime);
//	myClock.set(time1);

	CUnit("UT1 (Init)", (myClock.isOK() == true), &total, &KO);
	CUnit("UT2 (Battery)", (myClock.isBatteryOK() == true), &total, &KO);

	time1.setTimestamp(myClock.refTime);
	myClock.set(myClock.ctrlStackId, time1);
	myClock.get(myClock.ctrlStackId, &time2);
	CUnit("UT3 (Check refTime)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.reset ( time1, true);
	myClock.get(myClock.clockStackId, &time2);
	CUnit("UT4 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.ctrlStackId, &time2);
	CUnit("UT5 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.stackId3, &time2);
	CUnit("UT6 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.stackId4, &time2);
	CUnit("UT7 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.stackId5, &time2);
	CUnit("UT8 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.stackId6, &time2);
	CUnit("UT9 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.stackId7, &time2);
	CUnit("UT10 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	myClock.get(myClock.stackId8, &time2);
	CUnit("UT11 (Check reset)", (time1.getTimestamp() == time2.getTimestamp() ), &total, &KO);

	CUnitFinal(total, KO);
}

