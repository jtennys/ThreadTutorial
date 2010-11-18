// Author: Jason Tennyson
// File: ThreadTutorial.h
// Date: 11/2/10
//
// This file contains the function prototypes for the thread tutorial program.  The minimum and
// maximum thread and calculation count are set in this file. MAX_CALCULATIONS cannot be set any
// higher than 4294967295 for a 32-bit unsigned integer (which is what we are using). It is also
// not recommended that MAX_THREADS is set any higher than the number of CPUs that you have. In
// order for the percentage calculation to work, MAX_CALCULATIONS can be no greater than 2000000000.

#ifndef ThreadTutorial_h_
#define ThreadTutorial_h_

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <cstdlib>
#include "TimeStamp.h"
#include "CatHerder.h"

#define MIN_THREADS		(1)				// Minimum amount of threads.
#define MAX_THREADS		(16)				// Maximum amount of threads.

#define MIN_CALCULATIONS	(1)				// Minimum calculations.
#define	MAX_CALCULATIONS	(1000000000)			// Maximum calculations.

#define DEFAULT_THREADS		(2)				// Default amount of threads.
#define DEFAULT_CALCULATIONS	(1000000)			// Default calculations.
#define DEFAULT_DELTA		(1000)				// Step between samples.
#define DEFAULT_FILENAME	("ThreadTutorial")		// Default filename.
#define SPREADSHEET_FOLDER	("spreadsheets")		// Name of the data folder.
#define FILE_EXTENSION		(".csv")			// File extension.

// This is the routine used to run a single test.
void runTest(unsigned int threadNo, unsigned int calcs, bool gVar, bool tSafe);

// This function runs an automatic performance test.
void runAutoTest(const char* filename, bool gVar, bool tSafe,
		 unsigned int delta, unsigned int max, unsigned int threadNo);

// The function that each thread executes.
void* calcGenerator(void* threadObject);

#endif
