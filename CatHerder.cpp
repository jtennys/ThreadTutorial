// Author: Jason Tennyson
// File: CatHerder.cpp
// Date: 11/2/10
//
// This holds the function definitions for the user input control
// functions used in the catHerder class.

#include "CatHerder.h"

using namespace std;

// This is the constructor for the catHerder class. It initializes the
// random number generator, and allocates all of the memory required
// for the random scoldings the user is given for not following directions.
catHerder::catHerder(void)
{
	srand(time(NULL));	// Initialize the random generator.

	numResponses = 0;	// Initialize the number of responses to 0.
	impatience = 0;		// We are very tolerant right now.

	// Here is where you add responses. Be sure to check the number
	// for MAX_RESPONSES in CatHerder.h, and change it if you run out of
	// room for responses.
	addResponse("Maybe that Darwin guy was wrong...");
	addResponse("Reply hazy, try again...");
	addResponse("You're what the French call \"les incompetents\"...");
	addResponse("You have got to be kidding me...");
	addResponse("Really?...Really?!");
	addResponse("Just enter your IQ? Oh, that would be invalid, too...");
	addResponse("I'll bet you shoved square pegs into round holes as a kid.");
	addResponse("Is your cat running across the keyboard?");
	addResponse("Stop rolling your face across the keyboard!");
	addResponse("Pleading the fifth is not an option.");
}

// This is the destructor for the catHerder class.
catHerder::~catHerder(void)
{
	// Delete all the memory we allocated for random scoldings.
	for(int i = 0; i < numResponses; i++)
	{
		delete [] scoldings[i];
	}
}

// This function randomly prints out one of the responses specified in the constructor.
void catHerder::randomScolding(void)
{
	// This could get ugly.
	if(impatience < UP_TO_HERE)
	{
		impatience++;
	}

	// If you have had it up to or beyond "here" with this person, let them hear about it.
	if(impatience >= UP_TO_HERE)
	{
		// This float is the 0-1 decimal value of the random generation.
		float RNG;

		// This is the index of the response we decide to print.
		int responseIndex;

		// Generate the random floating point number between 0 and 1.
		RNG = ((float)rand() / (float)RAND_MAX);

		// Multiply the total number of responses by RNG to give us a decimal value
		// between 0 and numResponses, then shove it into the integer for the response index.
		responseIndex = (int)(RNG * numResponses);

		// Print out the scolding we have decided upon.
		cout << scoldings[responseIndex] << endl;
	}
	else
	{
		// Let it slide this time.
		cout << "That response is invalid. Try again please...\n";
	}
}

// This function adds a response to the array of possible scoldings.
void catHerder::addResponse(const char* response)
{
	// If we have not yet reached the maximum number of responses...
	if(numResponses < MAX_RESPONSES)
	{
		// Make this index of the scoldings string array point to a new memory
		// space with enough room to store the characters of the response variable.
		scoldings[numResponses] = new char[strlen(response)];
		// Create a duplicate character pointer that points to the response variable
		// and make the current scoldings index pointer equal that address.
		scoldings[numResponses] = strdup(response);

		// We now have another response ready to use.
		numResponses++;
	}
}

// This function converts a 32 bit unsigned integer input to a string.
const char* catHerder::unsignedToString(unsigned int input)
{
	char* tempInput = new char[11];		// Make room to store input temp.
	int index = 0;				// Temp input index.
	unsigned int divisor = 1000000000;	// At least 10 digits in input.

	// Initialize all of the characters in tempInput to null.
	for(int i = 0; i < 11; i++)
	{
		tempInput[i] = '\0';
	}

	// If we have a nonzero value, convert it, else, just return 0.
	if(input != 0)
	{
		// Find the leading digit.
		while(((input/divisor) == 0) && (divisor > 0))
		{
			divisor = divisor/10;
		}
	}
	else
	{
		divisor = 0;
	}

	// If the number is not zero...
	if(divisor > 0)
	{
		// While we have a divisor left...
		while(divisor > 0)
		{
			// Find how many times divisor will fit into the input
			// and add 48 to convert it to an ascii character
			// equivalent of the integer.
			tempInput[index] = 48 + (input/divisor);

			// Subtract the digit we just found off of the input.
			input = input - ((input/divisor)*divisor);

			// Increment the index so that we store the next digit
			// in the right place.
			index++;

			// Move the leading 1 in the divisor to compare against
			// the next digit.
			divisor = divisor/10;
		}
	}
	else
	{
		// If we don't have a divisor, we must have a zero as an input.
		tempInput[0] = '0';
	}

	// Duplicate the converted input and point a const char* at it.
	const char* convertedInput = strdup(tempInput);

	// Delete the memory that we allocated for storing the conversion.
	delete [] tempInput;

	// Return the pointer to our conversion.
	return convertedInput;
}

// This function waits for the user to put in a correct response as a string,
// even though we are looking for a number. This is done so that negative or
// out of range numbers do not crash the program.
unsigned int catHerder::askForUnsignedInt(const char* query, const unsigned int min, const unsigned int max)
{
	string userInput;		// Stores the user input string.
	bool validInput;		// Flag that is set true for a valid input.
	unsigned int value;		// Stores the user value as an unsigned int.
	const int maxChars = 10;	// This is the maximum number of input chars.

	// Only proceed if it is possible to receive a number in between max and min.
	if(max >= min)
	{
		string minString;	// Stores the string equivalent of min.
		string maxString;	// Stores the string equivalent of max.
	
		// Grab the string equivalents of the min and max values.
		minString = unsignedToString(min);
		maxString = unsignedToString(max);

		// Do this until the user gives us something we can work with.
		do
		{
			// We take the approach of innocent until proven guilty.
			validInput = true;
			// Create a string stream for user input conversion.
			stringstream userIn;
			// Print out the given query and the desired min and max range.
			cout << query << " [" << min << "-" << max << "]: ";
			// Wait for the user to type something.
			cin >> userInput;

			// Check to see if the user at least typed 10 or less characters...
			int j = 0;
			while((userInput[j] != '\0') && (j < maxChars))
			{
				j++;
			}

			// Check where we landed.
			if(userInput[j] != '\0')
			{
				// We hit maxChars without a null character.
				validInput = false;
			}

			// If we are within the character limit still, move on.
			if(validInput)
			{
				// Cycle through the characters to make sure that they
				// are valid base ten digits.
				for(int i = 0; (userInput[i] != '\0') && validInput; i++)
				{
					// If the digit is 0-9...
					if((userInput[i] < '0') || (userInput[i] > '9'))
					{
						// If any of these digits aren't correct, this
						// flag is reset and we exit the loop and make
						// the user try their entry again.
						validInput = false;
					}
				}
			}

			// If we still have a valid input up to this point, check
			// it against our max.
			if(validInput)
			{
				// This boolean is set if we have a character over the
				// max value amount. We traverse the characters in
				// backwards order so that if we read the largest digit
				// to be a character equal to maxString[0] and we have
				// a warning, we are out of range.
				bool warning = false;

				if(strlen(userInput.c_str()) > strlen(maxString.c_str()))
				{
					// There is no way we have less than max if we have
					// more input characters than max.
					warning = true;
				}
				else if(strlen(userInput.c_str()) == strlen(maxString.c_str()))
				{
					// Traverse backwards through the string.
					for(int i = (strlen(userInput.c_str())-1); i >= 0; i--)
					{
						if(userInput[i] > maxString[i])
						{
							// Warning: character is too large...
							warning = true;
						}
						else if((userInput[i] < maxString[i]) && warning)
						{
							// Clear the warning if we are well below max.
							warning = false;
						}
					}
				}

				// We ended our traversal with a warning, so the input
				// that we received isn't safe to convert.
				if(warning)
				{
					validInput = false;
				}
			}

			// If we still have a valid input up to this point, check
			// it against our min.
			if(validInput)
			{
				// This boolean is set if we have a character over the
				// min value amount.
				bool warning = false;

				if(strlen(userInput.c_str()) < strlen(minString.c_str()))
				{
					// There is no way we have met the min if we have
					// less input characters than min.
					warning = true;
				}
				else if(strlen(userInput.c_str()) == strlen(minString.c_str()))
				{
					// Traverse backwards through the string.
					for(int i = (strlen(userInput.c_str())-1); i >= 0; i--)
					{
						if(userInput[i] < minString[i])
						{
							// Warning: character is too large...
							warning = true;
						}
						else if((userInput[i] > minString[i]) && warning)
						{
							// Clear the warning if we are well below max.
							warning = false;
						}
					}
				}

				// We ended our traversal with a warning, so the input
				// that we received isn't safe to convert.
				if(warning)
				{
					validInput = false;
				}
			}

			// If after all is said and done, we have a value that is
			// both a valid response, and is within our max and min, we
			// can safely convert it to unsigned int without any
			// unpredictable conversion values.
			if(validInput)
			{
				// Dump the user input into the string stream.
				userIn << userInput;
				// Convert that string stream to the response format.
				userIn >> value;
				// We are now more patient.
				impatience = 0;
			}
			else
			{
				// Sigh...
				randomScolding();
			}

		// We continue to force the user to try entering a valid input.
		}while(!validInput);
	}
	else
	{
		// These people don't know how to use numbers,
		// and therefore we must wreck their day.
		cout << "Not a valid min/max combination! Halting!\n";
		while(1) { }
	}

	// Return the value to the caller.
	return value;
}

// This function waits for the user to put in a correct response to a yes or no question.
bool catHerder::askYesOrNo(const char* query)
{
	bool answer = false;	// Stores the answer to the question.
	string stringInput;	// Stores the user's string input.

	do
	{
		// Print out the question.
		cout << query << " [y/n]: ";

		// Wait for the user to type a response.
		cin >> stringInput;

		// Check to see if they were capable of yes or no.
		if((stringInput[0] == 'N') || (stringInput[0] == 'n'))
		{
			// The answer is no, and we are happy.
			answer = false;
			impatience = 0;
		}
		else if((stringInput[0] == 'Y') || (stringInput[0] == 'y'))
		{
			// The answer is yes, and we are now more tolerant.
			answer = true;
			impatience = 0;
		}
		else
		{
			// Sigh...
			randomScolding();
		}

	// Do this while they can't provide a valid response.
	}while((stringInput[0] != 'N') && (stringInput[0] != 'n')
		&& (stringInput[0] != 'Y') && (stringInput[0] != 'y'));

	// Return the answer as a boolean.
	return answer;
}
