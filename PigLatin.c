#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "cs50.h"

#define SHIFT	32	//ASCII shift between upper and lowercase
#define APOSTROPHE  39    //ASCII character for apostrophe, used in checking contractions

int spacecount, phraselen;  string input;
void GetInput();
bool Vowel(int index);
void PrintPig(string word, bool wsuffix, bool iscaps);
void CountSpaces();     void WordScan();    void Translate();

int main(int argc, string argv[])
{
	while (1)
	{
	GetInput();

	Translate();
	}

	return 0;
}

void GetInput() //Simply retrieves the input and calculated a few things
{
	input = GetString();
	phraselen = strlen(input); //used multiple times, mostly to end the loop. Is simply the length of the phrase being translated (before translation)

	CountSpaces();

	printf("\n\n");
}

void CountSpaces()
{
	for (int i = 0; i < phraselen; i++) // counts up spaces until first letter is seen in string
	{
		if (isalpha(input[i]))
		{
			break;
		}
		else;
		{
			spacecount++;
		}
	}
}

bool Vowel(int index) //Makes checking for vowels a function that can be called in an if statement, returns a boolean 1 if true and 0 if false
{
	if (input[index] == 'a' || input[index] == 'A'
		|| input[index] == 'e' || input[index] == 'E'
		|| input[index] == 'i' || input[index] == 'I'
		|| input[index] == 'o' || input[index] == 'O'
		|| input[index] == 'u' || input[index] == 'U')
		return 1;
	else
		return 0;
}

void PrintPig(string word, bool wsuffix, bool iscaps) //Function made to print translated word with a few choice properties
{
	//word is the word that is translated.
	//wsuffix is a boolean that checks to see if the suffix is way or ay the suffix will always be "way" if the word was not shifted around, and "ay" if it was. 
	//iscaps checks to see if the suffix needs to be capitalized
	//A bit inefficient, but trying to simply capitalize a string creates a segmentation fault? Perhaps because the suffix would be a string literal?
	if (wsuffix && iscaps)
		printf("%sWAY", word);
	else if (iscaps)
		printf("%sAY", word);
	else if (wsuffix)
		printf("%sway", word);
	else
		printf("%say", word);
}

char wordbuffer[256]; //word currently being translated, it is created to be 4 longer than the phrase just incase the phase is just that single word, probably can properly resize the array in a better fashion when I have the time
int inputindex, conscount; //inputindex is index for main loop and typically the position for the input. conscount is a continuously recycled counter of how many consonants each word has before its first vowel

void WordScan()
{
	conscount = 0;

	if (!Vowel(inputindex))
	{
		int scanindex = inputindex; // scanindex represents an index that can scroll through the current word without changing the inputindex position

		if (input[inputindex] == 'y' || input[inputindex] == 'Y' || ((input[inputindex] == 'q' || input[inputindex] == 'Q') && (input[inputindex + 1] == 'u' || input[inputindex + 1] == 'U')))
		{   //If the first letter in the word is "Y", we see it as a consonant that will be shifted, any "Y" after this will be viewed as a vowel. Also treats "QU" as one letter.
			conscount++;
			scanindex++;
		}

		for (scanindex = inputindex; scanindex < phraselen; conscount++, scanindex++) //Counts how many consonants before the first vowel
		{
			if (!isalpha(input[scanindex]))
			{
				conscount = -1; //special value for if the word has no vowels at all (ie. Mr. Mrs. Ms.)
				break;
			}

			if (Vowel(scanindex) || input[scanindex] == 'y' || input[scanindex] == 'Y') // Begins the consonant count for the current word and only ends when it hits a vowel (or y)
				break;
		}
	}
}

void Translate() //Large translation function
{
	for (inputindex = 0; inputindex < phraselen; inputindex++)
	{
		for (int i = 0, v = strlen(wordbuffer); i < v; i++) //nullifies the temporary word to renew it (possibly not necessary but is a good safeguard)
			wordbuffer[i] = 0;

		WordScan();

		if (isalpha(input[inputindex])) //checks if current position is not a word, and the next position is the beginning of a word
		{
			int outputindex; //new index for translation output

			if (conscount < 1)
			{   //translates words that begin with a vowel, or words with only consonants (no shifting in text)
				for (outputindex = 0; isalpha(input[inputindex]) || input[inputindex] == APOSTROPHE; outputindex++, inputindex++) //copy pastes current word to translation buffer
					wordbuffer[outputindex] = input[inputindex];

				wordbuffer[outputindex] = 0; //ends the buffer so we dont print random characters
				if (conscount == 0)
					PrintPig(wordbuffer, 1, (outputindex == 1) ? 0 :isupper(wordbuffer[outputindex - 1])); //If it has a vowel, end the word with "way"
				else //Both also check to see if there is only one letter in the word, and if so don't bother capitalizing
					PrintPig(wordbuffer, 0, (outputindex == 1) ? 0 : isupper(wordbuffer[outputindex - 1])); //If there are no vowels, end the word with "ay"

				inputindex--; //shifts index (random output bug fix)
			}
			else
			{   //translates words that begin with a consonant
				int caps = 0;
				outputindex = 0;
				int wordstart = inputindex; // wordstart holds the beginning position of the word so the inputindex shifts as little as possible

				for (int i = inputindex; isalpha(input[i]); i++) //If the word starts with a capital but is not all caps, remember to shift the capital when the letters are shifted
				{
					if (islower(input[i]) && isupper(input[inputindex]))
					{
						caps = 2;
						break;
					}
				}

				while (isalpha(input[inputindex + conscount]) || input[inputindex + conscount] == APOSTROPHE) //pastes letters after starting consonant(s)
				{
					if (caps == 2)  //shifts the new starting letter to caps if true
					{
						wordbuffer[outputindex] = input[inputindex + conscount] - SHIFT;
						caps--;
					}
					else
						wordbuffer[outputindex] = input[inputindex + conscount];

					inputindex++; //shift indexes
					outputindex++;
				}

				for (int i = wordstart; i < (wordstart + conscount); i++, outputindex++) //pastes starting consonants at the end of the word
				{
					if (caps == 1) //if the previous first letter started with a capital, remove it (if the word is not all caps)
					{
						wordbuffer[outputindex] = input[i] + SHIFT;
						caps = 0;
					}
					else
						wordbuffer[outputindex] = input[i];
				}

				wordbuffer[outputindex] = 0; //ends the buffer so we dont print random characters
				PrintPig(wordbuffer, 0, (outputindex == 1) ? 0 : isupper(wordbuffer[outputindex - 1])); //Prints the word and ends it with "ay"

				if (conscount > 1)
					inputindex = inputindex + (conscount - 1); //strange bugfix that helps reset the index after a complicated shift
			}
		}
		else
			printf("%c", input[inputindex]); //Prints non alphabetic characters
	}
	printf("\n\n");
}