/*
 * Calendar.cpp
 *
 *  Created on: 3 mars 2014
 *      Author: FEUD7506
 */
#include "Calendar.h"
#include "Arduino.h"
#include "util.h"

/**
 * macro to calculate the number of days since 2000 year
 * @param
 *	y : year
 */
#define PREVIOUS_YEAR_NUM_DAY(y) (((y)*365) + (((y)+3)>>2))


/**
 * macro to determine if the year is not a leap year
 * @param
 *	y : year
 */
#define NOT_LEAPYEAR(y) ((y)&3)


/**
 * macro to determine the day of week
 * @param
 *	TS : timestamp
 */
#define WEEK_DAY(TS) ((((TS / 86400) + 6) % 7)+1);


/**
 * constructor
 * 	default is set to 01/01/00 00:00:00
 * @param
 */
Calendar::Calendar( ) {
	this->timestamp=0;
}


/**
 * constructor
 * @param
 *	timestamp
 */
Calendar::Calendar( const unsigned long timestamp) {
	this->timestamp=timestamp;
}


Calendar::~Calendar() {
}


/**
 * timestamp getter, timestamp is the number of seconds since 2000
 * @return
 *   the timestamp
 */
unsigned long Calendar::getTimestamp() const {
	return this->timestamp;
}


/**
 * timestamp setter, timestamp is the number of seconds since 2000
 * @param
 *   the timestamp
 */
void Calendar::setTimestamp(const unsigned long timestamp) {
	this->timestamp=timestamp;
}


/**
 * set timestamp according to timestamp getter
 * @param
 *   second
 *   minute
 *   hour
 *   day
 *   hour
 *   year
 */
void Calendar::set(const byte second, const byte minute, const byte hour, const byte day, const byte month, const byte year) {
	unsigned int nbDays;

	if( second >59 &&
		minute >59 &&
		hour >23 &&
		day == 0 &&
		day >31 &&
		month == 0 &&
		month >12 &&
		year > 50 ) {
		this->setTimestamp(0);
	} else {
		// previews years number of days
		nbDays = PREVIOUS_YEAR_NUM_DAY(year);

		switch (month) {
			case  2 : nbDays +=  31; break;
			case  3 : nbDays +=  59; break;
			case  4 : nbDays +=  90; break;
			case  5 : nbDays += 120; break;
			case  6 : nbDays += 151; break;
			case  7 : nbDays += 181; break;
			case  8 : nbDays += 212; break;
			case  9 : nbDays += 243; break;
			case 10 : nbDays += 273; break;
			case 11 : nbDays += 304; break;
			case 12 : nbDays += 334; break;
		}

		if ((month < 3) || NOT_LEAPYEAR(year))
		{
			nbDays += day - 1;
		}
		else // after the 29th of feb
		{
			nbDays += day;
		}
		this->setTimestamp(second + minute*60 + (unsigned long)hour*3600 + (unsigned long)nbDays*24*3600);
	}
}


/**
 * to convert the timestamp to Calendar
 * @param
 *   *second : OUT
 *   *minute : OUT
 *   *hour : OUT
 *   *weekDay : OUT 1 is monday
 *   *day : OUT
 *   *hour: OUT
 *   *year : OUT
 */
void Calendar::get(byte *second, byte *minute, byte *hour, byte *weekDay, byte *day, byte *month, byte *year) const {
	unsigned long tmpTS, begin, end;
	byte i;

	tmpTS = this->getTimestamp() / 86400;	// 60 * 60 * 24
	(*year)=(tmpTS - ((tmpTS + 1401) / 1461)) / 365;

	tmpTS = tmpTS + 1 - PREVIOUS_YEAR_NUM_DAY((*year));
	end = 0;
	for (i=1; i<=12; i++) {
		begin = end;
	    switch (i) {
	    	case  2 :
	    		end += 28 + (!NOT_LEAPYEAR((*year)));
	    		break;
	    	case  4 :
	    	case  6 :
	    	case  9 :
	    	case 11 :
	    		end += 30;
	    		break;
	    	default :
	    		end += 31;
	    		break;
	    }
	    if (end >= tmpTS)
	    {
	    	(*month)=i;
	    	(*day)=tmpTS - begin;
	    	i = 12;
		}
	}

	(*weekDay)=WEEK_DAY(this->getTimestamp());

	tmpTS  = timestamp % 86400;
	(*hour)=(byte)(tmpTS / 3600);
	tmpTS  = tmpTS % 3600;
	(*minute)=(byte)(tmpTS / 60);
	(*second)=(byte)(tmpTS % 60);
}


/**
 * to convert the number of seconds since 2000 to a Calendar object according to timezone and summerTime
 * @param
 *   timestamp : number of seconds since 01/01/2000
 *   timeZone :
 */
void Calendar::get(const int timeZone, const bool summerTime, byte *second, byte *minute, byte *hour, byte *weekDay, byte *day, byte *month, byte *year) const {
	  unsigned long tmpTS;
	  byte LastSunday;
	  short tmpTZ;

	  tmpTZ = timeZone;

	  if (summerTime)
	  {
	    this->get(second, minute, hour, weekDay, day, month, year);
	    if ((*month > 3) && (*month < 10))
	    {
	      tmpTZ++;
	    }
	    if (*month == 3)
	    {
	      LastSunday = 31 - ((((((byte) *year) * 5) >> 2) + 5) % 7);
	      if ((*day > LastSunday) || ((*day == LastSunday) && (*hour)))
	      {
	        tmpTZ++;
	      }
	    }
	    if (*month == 10)
	    {
	      LastSunday = 31 - ((((((byte) *year) * 5) >> 2) + 2) % 7);
	      if ((*day < LastSunday) || ((*day == LastSunday) && (!(*hour))))
	      {
	        tmpTZ++;
	      }
	    }
	  }

	  if (tmpTZ & 128) //negatif
	  {
	    tmpTZ = (short) 0 - tmpTZ;
	    tmpTS = (unsigned long) tmpTZ * 3600;
	    if (tmpTS < this->getTimestamp())
	    {
	      tmpTS = this->getTimestamp() - tmpTS;
	    }
	    else
	    {
	      tmpTS = this->getTimestamp();
	    }
	  }
	  else
	  {
	    tmpTS = this->getTimestamp() + (long) tmpTZ * 3600;
	  }

	  Calendar myTmpCal=Calendar(tmpTS);
	  myTmpCal.get(second, minute, hour, weekDay, day, month, year);
//	  (*weekDay) = WEEK_DAY(tmpTS);
}


/**
 * to display the  object
 * @param
 *   *buffer : the buffer to store the information to display
 *
 * @return : same as *buffer
 */
char *Calendar::toString(char *buffer) const {
	byte day, month, year, weekDay, hour, minute, second;

	this->get(&second, &minute, &hour, &weekDay, &day, &month, &year);
	sprintf(buffer, "%d/%d/%d (%d) %d:%d:%d", day, month, year, weekDay, hour, minute, second);
	return buffer;
}



/**
 * to get seconds.
 * @return
 *   second : number of second in the object.
 */
byte Calendar::getSecond() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return second;
}



/**
 * to get minutes.
 * @return
 *   minute : number of second in the object.
 */
byte Calendar::getMinute() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return minute;
}


/**
 * to get hour.
 * @return
 *   hour : number of second in the object.
 */
byte Calendar::getHour() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return hour;
}



/**
 * to get weekDay.
 * @return
 *   weekDay : number of second in the object.
 */
byte Calendar::getWeekDay() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return weekDay;
}


/**
 * to get Day.
 * @return
 *   Day : number of second in the object.
 */
byte Calendar::getDay() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return day;
}


/**
 * to get month.
 * @return
 *   month : number of second in the object.
 */
byte Calendar::getMonth() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return month;
}


/**
 * to get Year.
 * @return
 *   year : number of second in the object.
 */
byte Calendar::getYear() const {
	byte second, hour, minute, weekDay, day, month, year;

	this->get(&second, &hour, &minute, &weekDay, &day, &month, &year);
	return year;
}


/**
 * to set seconds.
 * @param
 *   second to set.
 */
void Calendar::setSecond(byte second) {
	byte mySecond, myHour, myMinute, myWeekDay, myDay, myMonth, myYear;

	this->get(&mySecond, &myMinute, &myHour, &myWeekDay, &myDay, &myMonth, &myYear);
	this->set(second, myMinute, myHour, myDay, myMonth, myYear);
}



/**
 * to set minutes.
 * @param
 *   minutes to set.
 */
void Calendar::setMinute(byte minute) {
	byte mySecond, myHour, myMinute, myWeekDay, myDay, myMonth, myYear;

	this->get(&mySecond, &myMinute, &myHour, &myWeekDay, &myDay, &myMonth, &myYear);
	this->set(mySecond, minute, myHour, myDay, myMonth, myYear);
}



/**
 * to set hour.
 * @param
 *   hour to set.
 */
void Calendar::setHour(byte hour) {
	byte mySecond, myHour, myMinute, myWeekDay, myDay, myMonth, myYear;

	this->get(&mySecond, &myMinute, &myHour, &myWeekDay, &myDay, &myMonth, &myYear);
	this->set(mySecond, myMinute, hour, myDay, myMonth, myYear);
}



/**
 * to set days.
 * @param
 *   days to set.
 */
void Calendar::setDay(byte day) {
	byte mySecond, myHour, myMinute, myWeekDay, myDay, myMonth, myYear;

	this->get(&mySecond, &myMinute, &myHour, &myWeekDay, &myDay, &myMonth, &myYear);
	this->set(mySecond, myMinute, myHour, day, myMonth, myYear);
}



/**
 * to set monthes.
 * @param
 *   monthes to set.
 */
void Calendar::setMonth(byte month) {
	byte mySecond, myHour, myMinute, myWeekDay, myDay, myMonth, myYear;

	this->get(&mySecond, &myMinute, &myHour, &myWeekDay, &myDay, &myMonth, &myYear);
	this->set(mySecond, myMinute, myHour, myDay, month, myYear);
}



/**
 * to set year.
 * @param
 *   second to year.
 */
void Calendar::setYear(byte year) {
	byte mySecond, myHour, myMinute, myWeekDay, myDay, myMonth, myYear;

	this->get(&mySecond, &myMinute, &myHour, &myWeekDay, &myDay, &myMonth, &myYear);
	this->set(mySecond, myMinute, myHour, myDay, myMonth, year);
}



/**
 * to add seconds. Can be negative. If resulting timestamp <0, set to 0
 * @param
 *   second : number of second to add.
 */
void Calendar::addSecond(const long second) {
	long timestamp;

	timestamp=this->getTimestamp()+second;
	if( timestamp < 0 ) timestamp=0;
	this->setTimestamp(timestamp);
}


/**
 * to add minutes. Can be negative. If resulting timestamp <0, set to 0
 * @param
 *   minute : number of minute to add.
 */
void Calendar::addMinute(const long  minute) {
	this->addSecond(minute*60);
}


/**
 * to add hours.  Can be negative. If resulting timestamp <0, set to 0
 * @param
 *   hour : number of minute to add.
 */
void Calendar::addHour(const long  hour) {
	this->addSecond(hour*60*60);
}


/**
 * to add days. Can be negative. If resulting timestamp <0, set to 0
 * @param
 *   day : number of minute to add.
 */
void Calendar::addDay(const long day) {
	this->addSecond(day*60*60*24);

}


/**
 *
 * unitaryTestCalendar is used for non regression unitary tests
 * Serial must be opened
 * @param
 */
void unitaryTestCalendar() {
	byte KO=0, total=0;
	Calendar myCalendar=Calendar();
	char buffer[100];

	CUnit("UT1 (Init)", (myCalendar.getTimestamp() == 0), &total, &KO);
	CUnit("UT2 (Init)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 0:0:0") == 0), &total, &KO);

	myCalendar.addSecond(1);
	CUnit("UT3 (Add 1 second)", (myCalendar.getTimestamp() == 1), &total, &KO);
	CUnit("UT4 (Add 1 second)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 0:0:1") == 0), &total, &KO);

	myCalendar.addSecond(60);
	CUnit("UT5 (Add 60 second)", (myCalendar.getTimestamp() == 61), &total, &KO);
	CUnit("UT6 (Add 60 second)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 0:1:1") == 0), &total, &KO);

	myCalendar.addMinute(1);
	CUnit("UT7 (Add 1 minute)", (myCalendar.getTimestamp() == 121), &total, &KO);
	CUnit("UT8 (Add 1 minute)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 0:2:1") == 0), &total, &KO);

	myCalendar.addMinute(60);
	CUnit("UT9 (Add 60 minute)", (myCalendar.getTimestamp() == 3721), &total, &KO);
	CUnit("UT10 (Add 60 minute)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 1:2:1") == 0), &total, &KO);

	myCalendar.addHour(1);
	CUnit("UT11 (Add 1 hour)", (myCalendar.getTimestamp() == 7321), &total, &KO);
	CUnit("UT12 (Add 1 hour)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 2:2:1") == 0), &total, &KO);

	myCalendar.addHour(7);
	CUnit("UT13 (Add 7 hour)", (myCalendar.getTimestamp() == 32521), &total, &KO);
	CUnit("UT14 (Add 7 hour)", (strcmp(myCalendar.toString(buffer), "1/1/0 (7) 9:2:1") == 0), &total, &KO);

	myCalendar.addHour(20);
	CUnit("UT15 (Add 20 hour)", (myCalendar.getTimestamp() == 104521), &total, &KO);
	CUnit("UT16 (Add 20 hour)", (strcmp(myCalendar.toString(buffer), "2/1/0 (1) 5:2:1") == 0), &total, &KO);

	myCalendar.addDay(1);
	CUnit("UT17 (Add 1 day)", (myCalendar.getTimestamp() == 190921), &total, &KO);
	CUnit("UT18 (Add 1 day)", (strcmp(myCalendar.toString(buffer), "3/1/0 (2) 5:2:1") == 0), &total, &KO);

	myCalendar.set(0,0,0,28,2,0);
	CUnit("UT21 (set to 28/2/0)", (myCalendar.getTimestamp() == 5011200), &total, &KO);
	CUnit("UT22 (set to 28/2/0)", (strcmp(myCalendar.toString(buffer), "28/2/0 (2) 0:0:0") == 0), &total, &KO);

	myCalendar.addDay(1);
	CUnit("UT24 (Add 1 day)", (strcmp(myCalendar.toString(buffer), "29/2/0 (3) 0:0:0") == 0), &total, &KO);

	myCalendar.addDay(1);
	CUnit("UT25 (Add 1 day)", (strcmp(myCalendar.toString(buffer), "1/3/0 (4) 0:0:0") == 0), &total, &KO);

	myCalendar.set(0,0,0,1,1,14);
	CUnit("UT30 (Init)", (myCalendar.getTimestamp() == 441849600), &total, &KO);
	CUnit("UT31 (Init)", (strcmp(myCalendar.toString(buffer), "1/1/14 (4) 0:0:0") == 0), &total, &KO);

	myCalendar.set(0,0,0,5,4,14);
	CUnit("UT33 (+31 days)", (strcmp(myCalendar.toString(buffer), "5/4/14 (7) 0:0:0") == 0), &total, &KO);

	myCalendar.set(0,0,0,5,4,14);
	CUnit("UT40 (getSecond)", (myCalendar.getSecond() == 0), &total, &KO);
	CUnit("UT41 (getMinute)", (myCalendar.getMinute() == 0), &total, &KO);
	CUnit("UT42 (getHour)", (myCalendar.getHour() == 0), &total, &KO);
	CUnit("UT43 (getWeekDay)", (myCalendar.getWeekDay() == 7), &total, &KO);
	CUnit("UT44 (getDay)", (myCalendar.getDay() == 5), &total, &KO);
	CUnit("UT45 (getMonth)", (myCalendar.getMonth() == 4), &total, &KO);
	CUnit("UT46 (getYear)", (myCalendar.getYear() == 14), &total, &KO);

	myCalendar.set(0,0,0,5,4,14);
	myCalendar.setSecond(1);
	CUnit("UT50 (setSecond)", (strcmp(myCalendar.toString(buffer), "5/4/14 (7) 0:0:1") == 0), &total, &KO);

	myCalendar.setMinute(1);
	CUnit("UT51 (setMinute)", (strcmp(myCalendar.toString(buffer), "5/4/14 (7) 0:1:1") == 0), &total, &KO);

	myCalendar.setHour(1);
	CUnit("UT52 (setHour)", (strcmp(myCalendar.toString(buffer), "5/4/14 (7) 1:1:1") == 0), &total, &KO);

	myCalendar.setDay(6);
	CUnit("UT53 (setDay)", (strcmp(myCalendar.toString(buffer), "6/4/14 (1) 1:1:1") == 0), &total, &KO);

	myCalendar.setMonth(6);
	CUnit("UT54 (setMonth)", (strcmp(myCalendar.toString(buffer), "6/6/14 (6) 1:1:1") == 0), &total, &KO);

	myCalendar.setYear(15);
	CUnit("UT55 (setYear)", (strcmp(myCalendar.toString(buffer), "6/6/15 (7) 1:1:1") == 0), &total, &KO);

	CUnitFinal(total, KO);
}

