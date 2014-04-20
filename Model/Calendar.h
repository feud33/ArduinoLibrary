/*
 * Calendar.h
 *
 *  Created on: 3 mars 2014
 *      Author: FEUD7506
 */

#ifndef CALENDAR_H_
#define CALENDAR_H_
#include "Arduino.h"

class Calendar {
  public:
	Calendar();
	Calendar(const unsigned long timestamp);
	virtual ~Calendar();

	unsigned long getTimestamp() const;			// to get the number of seconds since 01/01/2000
  	void setTimestamp(const unsigned long timestamp);	// to set the timestamp

  	void set(const byte second, const byte minute, const byte hour, const byte day, const byte month, const byte year);
  	void get(byte *second, byte *minute, byte *hour, byte *weekDay, byte *day, byte *month, byte *year) const;
  	void get(const int timeZone, const bool summerTime, byte *second, byte *minute, byte *hour, byte *weekDay, byte *day, byte *month, byte *year) const;
  	byte getSecond() const;
  	byte getMinute() const;
  	byte getHour() const;
  	byte getWeekDay() const;
  	byte getDay() const;
  	byte getMonth() const;
  	byte getYear() const;
  	void setSecond(byte second);
  	void setMinute(byte minute);
  	void setHour(byte hour);
  	void setDay(byte day);
  	void setMonth(byte month);
  	void setYear(byte year);
  	char *toString(char *buffer) const;			// logger, 1 is sunday

  	void addSecond(const long second);			// to add seconds to the current object, can be greater than 60 and <0
  	void addMinute(const long minute);			// to add minutes to the current object, can be greater than 60 and <0
  	void addHour(const long hour);				// to add hours to the current object, can be greater than 24 and <0
  	void addDay(const long day);				// to add days to the current object, 30 days is not necessarily a month

  private:
	unsigned long timestamp;
};


void unitaryTestCalendar();

#endif /* CELENDAR_H_ */

