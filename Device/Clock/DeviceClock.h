/*
 * deviceClock.h
 *
 *  Created on: 21 mars 2014
 *      Author: FEUD7506
 */

#ifndef DEVICECLOCK_H_
#define DEVICECLOCK_H_
#include "Protocol/ProtocolI2C.h"
#include "Arduino.h"
#include "Calendar.h"


class DeviceClock : public ProtocolI2C {
public:
	DeviceClock();
	virtual ~DeviceClock();
	bool get(Calendar *time) const;						// get clock Calendar
	bool set(const Calendar &time);						// set clock Calendar
	bool get(const byte stackId, Calendar *time) const;	// get the desired stack
	bool set(const byte stackId, const Calendar &time);	// set the desired stack

	bool isOK() const;									// true if clock responding
	bool isBatteryOK() const ;							// true if battery is OK
	bool reset(Calendar currentTime, bool isInit);		// if battery is'nt OK, reset all the stacks

	static const unsigned long refTime=451932360;			// 27/04/14 16:46:00
	static const byte stackSize=7;
	static const byte clockStackId=0;						// 00h
	static const byte ctrlStackId=stackSize+1;				// 08h
	static const byte initStackId=ctrlStackId+stackSize;	// 08h
	static const byte stackId3=initStackId+stackSize;	// 15h
	static const byte stackId4=stackId3+stackSize;	// 15h
	static const byte stackId5=stackId4+stackSize;	// 15h
	static const byte stackId6=stackId5+stackSize;	// 15h
	static const byte stackId7=stackId6+stackSize;	// 15h
	static const byte stackId8=stackId7+stackSize;	// 15h


private:
	static const byte id=0x68;
};

void unitaryTestDeviceClock();

#endif /* DEVICECLOCK_H_ */
