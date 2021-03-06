// Author: Jason Tennyson
// File: TimeStamp.cpp
// Date: 11/2/10
//
// This file contains the class function definitions for the timeStamp
// class. These functions grab two time stamps, and do some math to
// calculate the time difference and print them nicely.

#include "TimeStamp.h"

using namespace std;

// This function accounts for rollover on all time measurements.
// This is necessary for if we take the difference in time stamps
// and end up with a negative number.
void timeStamp::rolloverCorrection(void)
{
	// Account for the microsecond count wrapping around.
	if(totalUsecs < 0)
	{
		totalUsecs += 1000000;
		totalSecs--;
	}

	// Account for the second count wrapping around.
	if(totalSecs < 0)
	{
		totalSecs += 60;
		totalMins--;
	}

	// Account for the minute count wrapping around.
	if(totalMins < 0)
	{
		totalMins += 60;
		totalHrs--;
	}

	// Account for the hour count wrapping around.
	if(totalHrs < 0)
	{
		totalHrs += 24;
	}
}

// This function is a way to print the end time result without having
// to print every time value, like hours and minutes, when they may not
// be necessary.
void timeStamp::smartTimePrint(void)
{
	// If there are hours to print, print them.
	if(totalHrs > 0)
	{
		cout << totalHrs << " hr";
	}

	// If hours have been printed, put in a comma, otherwise, print
	// as if minutes are the leading value.
	if((totalMins > 0) && (totalHrs > 0))
	{
		cout << ", " << totalMins << " min";
	}
	else if ((totalMins > 0) && (totalHrs == 0))
	{
		cout << totalMins << " min";
	}

	// Check to see if anything has been printed already. If so, print
	// out a comma. If not, print as if seconds are the leading value.
	if((totalSecs > 0) && ((totalHrs > 0) || (totalMins > 0)))
	{
		cout << ", " << totalSecs << " sec";
	}
	else if((totalSecs > 0) && (totalHrs == 0) && (totalMins == 0))
	{
		cout << totalSecs << " sec";
	}

	// Check to see if anything has already been printed. If so, print
	// out a comma. If not,  print milliseconds as the leading value.
	if(((totalUsecs/1000) > 0) && ((totalHrs > 0) || (totalMins > 0) || (totalSecs > 0)))
	{
		cout << ", " << totalUsecs/1000 << " msec";
	}
	else if(((totalUsecs/1000) > 0) && (totalHrs == 0) && (totalMins == 0) && (totalSecs == 0))
	{
		cout << totalUsecs/1000 << " msec";
	}

	// It's a good assumption to make that our program will at least
	// take microseconds to complete.  This conditional will at least
	// trigger a print of microseconds as a worst case scenario.
	if(((totalUsecs/1000) > 0) || (totalHrs > 0) || (totalMins > 0) || (totalSecs > 0))
	{
		cout << ", " << totalUsecs%1000 << " usec";
	}
	else
	{
		cout << totalUsecs%1000 << " usec";
	}
}

// This function grabs the current time values and stores them in this
// class's end time integer parameters.  It also moves the previous end
// time values into start time values.  Doing this allows the user to
// call this one function twice, and put whatever they are timing in
// between the two calls.
void timeStamp::getTime(void)
{
	struct timeval tv;		// Time value structure for grabbing time.
	struct timezone tz;		// Time zone structure for checking time zone.
	struct tm* currentTime;		// Structure pointer for grabbing start and end times.

	// Get the time of day based on the time zone.
	gettimeofday(&tv, &tz);

	// Point currentTime at the time stamp that we just took.
	currentTime=localtime(&tv.tv_sec);

	// Move all the old end time values to start time values.
	startHrs = endHrs;
	startMins = endMins;
	startSecs = endSecs;
	startUsecs = endUsecs;

	// Store all the new time values.
	endHrs = currentTime->tm_hour;
	endMins = currentTime->tm_min;
	endSecs = currentTime->tm_sec;
	endUsecs = (int)tv.tv_usec;
}

// This function calls the smart time print function.
void timeStamp::printTimeDiff(void)
{	
	// Print the time result intelligently without unused time values.
	smartTimePrint();
}

// Returns total time taken in microseconds.
unsigned int timeStamp::timeTaken(void)
{
	// Stores the total time for our return value.
	unsigned int totalTime = 0;

	// Take the differences in end and start times.
	totalUsecs = endUsecs - startUsecs;
	totalSecs = endSecs - startSecs;
	totalMins = endMins - startMins;
	totalHrs = endHrs - startHrs;

	// Account for negative times created when taking the differences.
	rolloverCorrection();

	// We can only safely return total time in microseconds if we have
	// taken less than an hour to crunch our numbers.
	if(totalHrs == 0)
	{
		totalTime = totalUsecs + totalSecs*1000000 + totalMins*60000000;
	}

	// Return the total time in microseconds.
	return totalTime;
}
