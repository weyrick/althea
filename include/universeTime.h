/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for universeTime class. implements a global system of time for the world.

	CHANGE HISTORY:
	


 */                   
                     

/* LOCAL INCLUDES */
#include <stdio.h>
#include <iostream.h>
#include <time.h>
#include "defs.h"
#include "String.h"

#ifndef AUNIVERSETIME_H
#define AUNIVERSETIME_H

/* 

  a class to implement a system of time for the universe.

   our time system is internally base one - ie, seconds go from 1 - SECONDS_PER_MINUTE,
   minutes go from 1 - MINUTES_PER_HOUR, etc.

*/

class universeTimeClass
{
	
public:
	
	// constructor sets up time values for the first time (yes, it's the begining of time :)
	universeTimeClass(void) {
		
		second 			= 1;
		minute 			= 1;
		hour 			= 1;
		dayOfMonth		= 1;
		dayOfYear 		= 1;
		dayOfWeek 		= 1;
		dayOfSeason 	= 1;
		weekOfMonth 	= 1;
		weekOfYear 		= 1;
		month 			= 1;
		year 			= 1;
		season 			= 1;
        totalSeconds 	= 1;
		
		nextPulseTime	= time(NULL) + 1;
		
	}
	
	// we're receiving a time slice - see if it's time to pulse the clock (keep this inline)
	bool timeSlice(void) {
		if (time(NULL) >= nextPulseTime) {
			nextPulseTime = time(NULL) + 1;
			pulse();
		}
		return true;
	}
	
	// get the current universe timestamp. format: YYYYDDHHMMSS 
	String stringStamp(void) {
		
		String tStamp;
		char buf[30];
		
		snprintf(buf, 30, "%04d%03d%02d%02d%02d", year, dayOfYear, hour, minute, second);
		tStamp = buf;
		
		return tStamp;
	}
	
    // get the current timestamp, base on seconds since the clock started
    unsigned long timeStamp(void) { return totalSeconds; }

private:
	
	// a pulse to the time system is the smallest incremental change - we use seconds. all other values
	//    will be updated / wrapped accordingly (keep this inline)
	void pulse(void) {
		
		
		// seconds ALWAYS gets incremented
		second++;
        totalSeconds++;
		
		if (second > SECONDS_PER_MINUTE) {
			
			// minute change
			minute++;
			second = 1;	
			
		}
		
		if (minute > MINUTES_PER_HOUR) {
			
			// hour change
			hour++;
			minute=1;
			
		}
		
		if (hour > HOURS_PER_DAY) {
			
			// day change
			dayOfYear++;
			dayOfWeek++;
			dayOfSeason++;
			dayOfMonth++;
			hour=1;
			
		}
		
		if (dayOfSeason > DAYS_PER_SEASON) {
			
			// season change
			season++;
			dayOfSeason=1;
			
		}
		
		if (season > SEASONS_PER_YEAR) {
			
			// wrap season?
			season = 1;
			dayOfSeason=1;
			
		}
		
		if (dayOfWeek > DAYS_PER_WEEK) {
			
			// week change
			weekOfYear++;
			weekOfMonth++;
			dayOfWeek=1;
			
		}
		
		if (weekOfMonth > WEEKS_PER_MONTH) {
			
			// month change
			month++;
			weekOfMonth=1;
			dayOfMonth=1;
			
		}
		
		if (month > MONTHS_PER_YEAR) {
			
			// year change
			year++;
			month=1;
			dayOfYear=1;
			weekOfYear=1;
			
		}	
		
	}	
	
	// print a string of the current universe time
	void debugTime(void) 
	{
		
		printf("%d:%d:%d %d/%d/%d - week: %d, day: %d, season: %d\n", hour, minute, second,
			month, dayOfMonth, year, weekOfYear, dayOfYear, season);	
		
	}	
	
	
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char dayOfWeek;
	unsigned int dayOfYear;
	unsigned char dayOfMonth;
	unsigned char weekOfMonth; 
	unsigned int weekOfYear;
	unsigned char month;
	unsigned int year;
	unsigned char season;
	unsigned int dayOfSeason;

    unsigned long totalSeconds;   // seconds since the world began
	
	time_t nextPulseTime;
	
};

// global universe time class instantiated in main.cpp
extern universeTimeClass universeTime;

#endif

