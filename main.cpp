// Author: Jason Tennyson
// File: main.cpp
// Date: 11/2/10
//
// This file contains the main function for the thread tutorial program,
// as well as a couple of functions to extract command line arguments.

#include "ThreadTutorial.h"
#include "CatHerder.h"

using namespace std;

// This function is used to extract a user-defined filename from the command line.
char* extractFilename(const char* filename);

// This function is used to extract a user-provided number from the command line.
unsigned int extractNumber(const char* argument);

// This is the function that starts it all.
int main(int argc, char** argv)
{
	// These are function parameters used by all phases of this program. They define shared
	// variable use, thread safety, number of threads used, and number of calculations done,
	// in that order.
	bool gVarUsed = false;
	bool threadSafe = false;
	unsigned int nThreads = DEFAULT_THREADS;
	unsigned int n;

	// If the user has specified an argument in addition to running the program...
	if(argc > 1)
	{
		// If the user has specified an automatic test, look for more
		// arguments that they have passed. Otherwise, we do nothing and exit.
		if((argv[1][0] == '-') && ((argv[1][1] == 'a') || (argv[1][1] == 'A')))
		{
			// Set all possible input parameters to their default values.
			string filename = DEFAULT_FILENAME;
			unsigned int delta = DEFAULT_DELTA;
			unsigned int max = DEFAULT_CALCULATIONS;
			unsigned int samples = 0;

			// If we have more than just an auto flag, parse the rest.
			if(argc > 2)
			{
				// Argument 3 (index 2) is the first argument after the -auto argument.
				// We start there and parse until we reach the argument count.
				for(int i = 2; i < argc; i++)
				{
					// If we have found an argument indicator, decode it.
					if(argv[i][0] == '-')
					{
						if((argv[i][1] == 'f') || (argv[i][1] == 'F'))
						{
							// The user is specifying a file name.
							// If the file name is valid, use it.
							if(extractFilename(argv[i]))
							{
								filename = strdup(extractFilename(argv[i]));
							}
						}
						else if((argv[i][1] == 'd') || (argv[i][1] == 'D'))
						{
							// The user is specifying the n step size.
							delta = extractNumber(argv[i]);

							// If the number is out of bounds, throw it out.
							if((delta < MIN_CALCULATIONS) || (delta > MAX_CALCULATIONS))
							{
								delta = DEFAULT_DELTA;
							}
						}
						else if((argv[i][1] == 'm') || (argv[i][1] == 'M'))
						{
							// Store the user-defined maximum.
							max = extractNumber(argv[i]);

							// If the number is out of bounds, throw it out.
							if((max < MIN_CALCULATIONS) || (max > MAX_CALCULATIONS))
							{
								max = DEFAULT_CALCULATIONS;
							}
						}
						else if((argv[i][1] == 't') || (argv[i][1] == 'T'))
						{
							// Store the user-defined thread count.
							nThreads = extractNumber(argv[i]);

							// If the number is out of bounds, throw it out.
							if((nThreads < MIN_THREADS) || (nThreads > MAX_THREADS))
							{
								nThreads = DEFAULT_THREADS;
							}
						}
						else if((argv[i][1] == 's') || (argv[i][1] == 'S'))
						{
							if((argv[i][2] == 'h') || (argv[i][2] == 'H'))
							{
								// The user wants shared variable usage.
								gVarUsed = true;
							}
							else if((argv[i][2] == 'a') || (argv[i][2] == 'A'))
							{
								if((argv[i][3] == 'f') || (argv[i][3] == 'F'))
								{
									// The user wants thread safety.
									threadSafe = true;
								}
								else if((argv[i][3] == 'm') || (argv[i][3] == 'M'))
								{
									// The user is specifying the number of samples.
									samples = extractNumber(argv[i]);

									// If the number is out of bounds, throw it out.
									if(samples > MAX_CALCULATIONS)
									{
										samples = 0;
									}
								}
							}
						}
					}
				}
			}

			// Append the file extension to the file name we are using.
			filename += FILE_EXTENSION;

			// If the user has specified a sample number, calculate delta.
			if(samples)
			{
				delta = max/samples;
			}

			// If the user gave a delta value that is larger than max, fix it.
			if((delta > max) || (delta == 0))
			{
				max = DEFAULT_CALCULATIONS;
				delta = DEFAULT_DELTA;
			}

			// Tell the user that we are starting.
			cout << "Auto test started! This may take a while...\n";

			// Call the auto test function.
			runAutoTest(filename.c_str(), gVarUsed, threadSafe, delta, max, nThreads);

			cout << filename << " has been saved in the '"
				 << SPREADSHEET_FOLDER << "' folder!\n";
			cout << "Open a spreadsheet program to do operations on the data!\n\n";
		}
	}
	else
	{
		// Create an instance of catHerder so that we can get safe and
		// formatted input responses from our users.
		catHerder inputFormat;

		const char* threadQuery = "How many threads would you like to run?";
		const char* calcQuery = "How many calculations would you like to do?";
		const char* sharedVarQuery = "Would you like to use a shared variable?";
		const char* threadSafeQuery = "Would you like to use thread safety?";
		const char* runThisAgain = "Would you like to run another test?";

		do
		{
			// Use the askForUnsignedInt function to ask the user for a number
			// of threads to run between MIN_THREADS and MAX_THREADS.
			nThreads = inputFormat.askForUnsignedInt(threadQuery, MIN_THREADS, MAX_THREADS);

			// Use the askForUnsignedInt function to ask the user for a number
			// of threads to run between MIN_CALCULATIONS and MAX_CALCULATIONS.
			n = inputFormat.askForUnsignedInt(calcQuery, MIN_CALCULATIONS, MAX_CALCULATIONS);

			if(nThreads > 1)
			{
				// Use the askYesOrNo function to ask the user if they want to use
				// shared variables or not.
				gVarUsed = inputFormat.askYesOrNo(sharedVarQuery);
			
				// If the user wants to use shared variables, also see if they want
				// those shared variables to be thread safe or not.
				if(gVarUsed)
				{
					threadSafe = inputFormat.askYesOrNo(threadSafeQuery);
				}
			}
			else
			{
				gVarUsed = false;
			}

			// Run an individual thread test.
			runTest(nThreads, n, gVarUsed, threadSafe);

		// Do this while the user still wants to run tests.
		}while(inputFormat.askYesOrNo(runThisAgain));
	}

	return 0;
}

// Extracts the filename from the filename argument. If the filename
// does not contain only alphanumeric characters, a null pointer is returned.
char* extractFilename(const char* filename)
{
	// Start the filename character index at 0.
	int index = 0;
	// We start out assuming a valid filename.
	bool validFilename = true;

	// Parse through the filename argument to find the '=' sign.
	// Also check to see if we have hit the end of the filename argument
	// in case the user didn't put an equals sign. That would be bad.
	while((filename[index] != '=') && (filename[index] != '\0'))
	{
		index++;
	}

	// Skip past the '=' sign.
	index++;

	// Parse until we hit the end of the argument or have proven the file name invalid.
	for(int i = index; ((filename[i] != '\0') && (validFilename)); i++)
	{
		// Check to see if the current character is alphanumeric.
		if(((filename[i] >= '0') && (filename[i] <= '9')) ||
		   ((filename[i] >= 'a') && (filename[i] <= 'z')) ||
		   ((filename[i] >= 'A') && (filename[i] <= 'Z')))
		{
			// Do nothing.
		}
		else
		{
			// If it is not, this file name is invalid.
			validFilename = false;
		}
	}

	// If we have a valid filename, return it, otherwise, return pointer to NULL.
	if(validFilename  && (filename[index] != '\0'))
	{
		return (char*)&filename[index];
	}
	else
	{
		return NULL;
	}
}

// This function simply extracts an integer out of an argument.
unsigned int extractNumber(const char* argument)
{
	// We start parsing from index 0.
	int index = 0;
	// This is where the output of the character array to integer conversion is stored.
	unsigned int output;

	while((argument[index] != '=') && (argument[index] != '\0'))
	{
		index++;
	}

	// Skip past the '=' sign.
	index++;

	// Store the user-defined number with the char* to integer function from the cstdlib header.
	output = atoi(&argument[index]);

	return output;
}
