// Author: Jason Tennyson
// File: CatHerder.h
// Date: 11/2/10
//
// This holds the function prototypes for user input control. The goal
// of these functions is to do the best we can to keep the user from
// crashing the program.

#ifndef CatHerder_h_
#define CatHerder_h_

#include <iostream>
#include <string.h>
#include <sstream>
#include <cstdlib>

// This is the maximum number of custom responses allowed.
#define MAX_RESPONSES	(100)

// Super secret define.
#define UP_TO_HERE	(3)

// This class is instantiated if user input formatting is desired. It
// has several functions to wait for a user to input a valid response,
// and scolds them if they do not do as they are told.
class catHerder
{
	public:
		// This is the class constructor.
		catHerder(void);
		// This is the class destructor.
		~catHerder(void);
		// This function prints a random scolding.
		void randomScolding(void);
		// This function converts an unsigned integer to a char string.
		const char* unsignedToString(unsigned int input);
		// This function waits for valid input in the range of the
		// given minimum and maximum unsigned integer values.
		unsigned int askForUnsignedInt(const char* query, const unsigned int min, const unsigned int max);
		// This function waits for a valid yes or no input.
		bool askYesOrNo(const char* query);
	
	private:
		// The number of random responses at our disposal.
		int numResponses;
		// This is a measure of our impatience. Results may vary.
		int impatience;
		// The array of responses for us to print randomly.
		char* scoldings[MAX_RESPONSES];
		// This function adds a response to the array of responses.
		void addResponse(const char* response);
};

#endif
