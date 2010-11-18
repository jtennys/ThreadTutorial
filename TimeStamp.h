// Author: Jason Tennyson
// File: TimeStamp.h
// Date: 11/2/10
//
// This file contains the class definition for the timeStamp class.
// This class is used for (you guessed it) grabbing time stamps that are
// wrapped around code that you want to measure for run time.

#ifndef TimeStamp_h_
#define TimeStamp_h_

#include <iostream>
#include <sys/time.h>

// This class contains the variables and function prototypes necessary
// to store and evaluate start, end, and total time with the purpose of
// measuring how long a certain piece of code takes. The overhead that
// this time stamping requires should be considered as well.
class timeStamp
{
	public:
		// This is the constructor for this class. Upon creation, it
		// initializes all of its private members.
		timeStamp(void)
		{
			// Set all of the time values to zero initially
			startHrs = startMins = startSecs = startUsecs = 0;
			endHrs = endMins = endSecs = endUsecs = 0;
			totalHrs = totalMins = totalSecs = totalUsecs = 0;
		}

		// This function grabs the current time values and stores them.
		void getTime(void);

		// This function takes the difference between the last two stamps,
		// formats that difference, and prints all nonzero time values.
		void printTimeDiff(void);

		// This function calculates total time taken in microseconds
		// and returns that time value.
		unsigned int timeTaken(void);

	private:
		// These are all the time variables that we use to measure time.
		int startHrs, startMins, startSecs, startUsecs;
		int endHrs, endMins, endSecs, endUsecs;
		int totalHrs, totalMins, totalSecs, totalUsecs;

		// This function accounts for time rollover and negative time differences.
		void rolloverCorrection(void);

		// This total execution time printer prints only nonzero time values.
		void smartTimePrint(void);
};

#endif
