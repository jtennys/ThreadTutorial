// Author: Jason Tennyson
// File: ThreadTutorial.cpp
// Date: 11/2/10
//
// This file contains the implementation of the thread tutorial program.
// These functions allow the user to either run single tests on a specific
// set of circumstances, or run an automated group of tests within a range
// that they can define that gets dumped into a spreadsheet file.
//
// Each calculation comes to a result. The result is used to check for data
// hazards. If the result is 1, there was no problem. If the result is less
// than 1, a data hazard caused an incorrect result to be calculated.

#include "ThreadTutorial.h"

using namespace std;

// These values have to be stored globally to be accessed by the threads.
unsigned int nThreads;
unsigned int n;
bool gVarUsed = false;
bool threadSafe = false;

// This is the shared variable that the threads fight over.
unsigned int SHARED_VARIABLE;

// This mutex is used for thread safety when sharing one variable.
pthread_mutex_t sharedVarMutex;

// This function asks the user what they want to do for the test, and
// then it runs the test and prints the results.
void runTest(unsigned int threadNo, unsigned int calcs, bool gVar, bool tSafe)
{
	// Store all of the values passed into the global variables for them.
	nThreads = threadNo;
	n = calcs;
	gVarUsed = gVar;
	threadSafe = tSafe;

	// Create an instance of timeStamp. The class is used simply to
	// grab two system times and print the difference between them.
	timeStamp threadTimer;

	// This is where the end result is stored.
	float endResult;

	// This array stores the integer total that each thread comes to.
	// These unshared totals are combined and used for the calculation
	// if global variable use is toggled off (gVarUsed = 0).
	unsigned int unsharedTotal[nThreads];

	// Array of thread handles. These are used as thread IDs.
	pthread_t threads[nThreads];

	// Clear the shared variable to zero before using it.
	SHARED_VARIABLE = 0;

	// Grab the first time stamp.
	threadTimer.getTime();

	// Set this process's concurrency to the number of threads used.
	// This is not entirely necessary for parallel execution, but is done
	// as a precaution, in case the system wants a weird concurrency value.
	if(pthread_setconcurrency(nThreads) == 0)
	{
		// Create nThreads number of threads.
		for(unsigned int i = 0; i < nThreads; i++)
		{
			// Create thread i with handle threads[i] that executes
			// the sharedIncrementer function and returns its end
			// increment value in unsharedTotal[i].
			pthread_create(&threads[i], NULL, calcGenerator, &unsharedTotal[i]);
		}

		// Wait for threads to finish.
		for(unsigned int i = 0; i < nThreads; i++)
		{
			// This joins the thread with handle threads[i] and won't
			// go further until it is done.  Needless to say, the program
			// won't exit this for loop until all threads are done.
			pthread_join(threads[i], NULL);
		}
	}

	// If we are not using a global variable, total the unshared values.
	if(!gVarUsed)
	{
		// Cram them into the global variable that would have
		// held them if they were shared.
		for(unsigned int i = 0; i < nThreads; i++)
		{
			SHARED_VARIABLE += unsharedTotal[i];
		}
	}

	// This is the end result calculation that is talked about at the top of this file.
	endResult = ((float)SHARED_VARIABLE)/n;

	// Get the second time stamp after the work is done.
	threadTimer.getTime();

	// Print out a warning if the number of calculations is not evenly
	// divisible by the number of threads.
	if(n%nThreads > 0)
	{
		cout << "\nWARNING: The number of threads used does not \n"
			 << "evenly divide into the number of calculations done.\n"
			 << "This may lead to a slightly incorrect result...\n";
	}

	// Print the end result.
	cout << "\nThe total is " << endResult << "!\n";

	// We print the difference between the last two time stamps,
	// assuming we called threadTimer.getTime() at least twice.
	cout << "The program took ";
	// This calculates the difference between the last two time stamps.
	threadTimer.timeTaken();
	// This prints the time difference.
	threadTimer.printTimeDiff();
	cout << " to compute it!\n\n";
}

// This function runs an automatic CPU performance test for the user.
void runAutoTest(const char* filename, bool gVar, bool tSafe,
		 unsigned int delta, unsigned int max, unsigned int threadNo)
{
	// Store the passed values into the global variable equivalents.
	gVarUsed = gVar;
	threadSafe = tSafe;

	// Create an instance of timeStamp.
	timeStamp threadTimer;

	// This stores the percentage completed for the calculations.
	double percentComplete = 0;

	// Stores the previous percentage for comparison when we are
	// re-printing the percentage to the terminal.
	int lastPercentage;

	// This is the message that precedes the percentage printout.
	const char* percentMessage = "Percentage Complete: ";

	// This stores the calculations per millionth of a percent.
	// We have to do this to avoid overflow due to the gigantic amounts
	// of calculations that this program can do.
	double calcsPerMillionth =
		((double)(max + delta)/2.0)*
		((double)((max - delta) + 1)/100000000.0);

	// Start n at delta and go from there.
	n = delta;

	// Start nThreads at MIN_THREADS as well.
	nThreads = MIN_THREADS;

	// Create an output file stream.
	ofstream dataDump;

	// Create a temp filename to append the spreadsheet folder location
	// in front of the desired filename.
	string tempFilename = SPREADSHEET_FOLDER;
	// Add a slash to denote that the filename is in a folder.
	tempFilename += "/";
	// Add the filename to the total string.
	tempFilename += filename;

	// Create a file inside of the spreadsheet folder with the name
	// that was passed by the user.
	dataDump.open(tempFilename.c_str());

	// Write the top line of the data file...
	// The first cell is the number of calculations 'n'.
	dataDump << "n";
	// The next cells are number of threads and the result calculated
	// for that number of threads alternating until we have a column for
	// all values for each thread number.
	for(unsigned int i = MIN_THREADS; i <= threadNo; i++)
	{
		dataDump << ",Time " << i << ",Result " << i;
	}
	// Write a new line to prepare for the first line of data.
	dataDump << "\n";

	// Loop until we have reached max.
	while(n <= max)
	{
		// Save n to our data file.
		dataDump << n;

		// Stores the last percentage so that we can move the output
		// on the terminal accordingly.
		lastPercentage = (int)percentComplete;

		// Loop through the number of threads we use.
		while(nThreads <= threadNo)
		{
			// This is where the end result is stored.
			float endResult;

			// This array stores the integer total that each thread comes to.
			// These unshared totals are combined and used for the calculation
			// if global variable use is toggled off (gVarUsed = 0).
			unsigned int unsharedTotal[nThreads];

			// Array of thread handles. These are used as thread IDs.
			pthread_t threads[nThreads];

			// Clear the shared variable to zero before using it.
			SHARED_VARIABLE = 0;

			// Grab the first time stamp.
			threadTimer.getTime();

			// Set this process's concurrency to the number of threads used.
			// This is not entirely necessary for parallel execution, but is done
			// as a precaution, in case the system wants a weird concurrency value.
			if(pthread_setconcurrency(nThreads) == 0)
			{
				// Create nThreads number of threads.
				for(unsigned int i = 0; i < nThreads; i++)
				{
					// Create thread i with handle threads[i] that executes
					// the sharedIncrementer function and returns its end
					// increment value in unsharedTotal[i].
					pthread_create(&threads[i], NULL, calcGenerator, &unsharedTotal[i]);
				}

				// Wait for threads to finish.
				for(unsigned int i = 0; i < nThreads; i++)
				{
					// This joins the thread with handle threads[i] and won't
					// go further until it is done.  Needless to say, the program
					// won't exit this for loop until all threads are done.
					pthread_join(threads[i], NULL);
				}
			}

			// If we are not using a global variable, total the unshared values.
			if(!gVarUsed)
			{
				// Cram them into the global variable that would have
				// held them if they were shared.
				for(unsigned int i = 0; i < nThreads; i++)
				{
					SHARED_VARIABLE += unsharedTotal[i];
				}
			}

			// This is the end result calculation that is talked about at the top of this file.
			endResult = ((float)SHARED_VARIABLE)/n;

			// Get the second time stamp after the work is done.
			threadTimer.getTime();

			// Increment the number of threads used.
			nThreads++;

			// Save the time taken and the result.
			dataDump << "," << threadTimer.timeTaken() << "," << endResult;
		}

		// Move down to the next line to prepare for the next group of data.
		dataDump << "\n";

		// Reset thread number to MIN_THREADS.
		nThreads = MIN_THREADS;

		// Update the percentage complete.
		percentComplete += ((double)n/calcsPerMillionth)*((double)delta/1000000.0);

		// If we have hit or surpassed 100 (eek), cap it at 100.					  
		if(percentComplete >= 100)
		{
			percentComplete = 100;
		}

		// Only print if we are ticking over a percent.
		if((int)percentComplete != lastPercentage)
		{
			// Print out what the program has done.
			cout << percentMessage << (int)percentComplete << "%\n";
		}

		// Increment the number of calculations to be done next time.
		n += delta;
	}

	// Append an extra new line character to the end of the file.
	dataDump << "\n";

	// Close the file.
	dataDump.close();
}

// This is the function that all threads run, which does the calculation.
void* calcGenerator(void* calculation)
{
	// This variable is used to store this thread's calculation.
	unsigned int unsharedVariable = 0;

	// Mangle the input parameter to equal something we can work with
	// and pass back at the end if we are using unshared data.
	unsigned int* threadResult = (unsigned int*)calculation;

	// Number of calculations to do.
	unsigned int calcTotal;				

	// Find out how many calculations this function will do. This is
	// simply the number of calculations n spread evenly across nThreads.
	calcTotal = n/nThreads;

	// Do the calculation calcTotal times. If a shared variable is desired,
	// use it, otherwise pass the value back to main through threadResult.
	if(gVarUsed)
	{
		// If the user wants thread safety, lock the mutex before
		// doing anything to the shared variable.  If another thread
		// has control of the mutex, this thread will block and wait
		// at this mutex call.  This is potentially dangerous, as it
		// will cause a deadlock if the other thread never unlocks
		// the mutex.  This makes unlocking when we're done very important.
		if(threadSafe)
		{
			pthread_mutex_lock(&sharedVarMutex);
		}

		// Do the calculation calcTotal times.
		for(unsigned int i = 0; i < calcTotal; i++)
		{
			// This is where the calculation happens if a shared variable
			// is used. We simply increment the shared variable here.
			SHARED_VARIABLE++;
		}

		// If we locked the mutex, we need to unlock it
		// so that other threads can use the shared variable.
		if(threadSafe)
		{
			pthread_mutex_unlock(&sharedVarMutex);
		}
	}
	else
	{
		// Do the calculation calcTotal times.
		for(unsigned int i = 0; i < calcTotal; i++)
		{
			// This is where each calculation is carried out if the user
			// wants to use unshared variables.
			unsharedVariable++;
		}

		// Pass back the unshared variable via the pointer we created
		// to point in the same direction as the input parameter.
		// This statement just says to make the unsigned int that
		// threadResult is pointing to equal to unsharedVariable.
		*threadResult = unsharedVariable;
	}

	// There is no variable to return.
	return (NULL);
}
