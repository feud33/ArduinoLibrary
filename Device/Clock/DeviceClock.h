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
	bool get(Calendar *time) const;
	bool set(const Calendar &time);
	bool get(byte stackId, Calendar *time) const;
	bool set(byte stackId, const Calendar &time);

	bool Check() const;

private:
	static const byte id=0x68;
	static const byte stackSize=7;
	static const byte clockStackId=0;						// 00h
	static const byte initStackId=stackSize+1;				// 08h
	static const byte lastStackId=initStackId+stackSize;	// 15h
};

#endif /* DEVICECLOCK_H_ */
