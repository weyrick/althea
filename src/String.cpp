/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
			implementation for string class

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include <stdio.h>
#include <ctype.h>
#include "String.h"

/* IMPLEMENTATION */
												   
// default constructor creates string of 0 bytes
String::String(void) 
{

	myString = new char[1];
	myString[0] = '\0';
	myLen = 0;

}

// private helper constructor, used only by class methods for
// creating a new string of required size. Null Filled
String::String(stringLengthType len)
{

	myString = new char[len+1];
	for (stringLengthType i=0; i<=len; i++)
		myString[i] = '\0';

	myLen = len;

}

// convert a character array to a String
String::String(const char * const cString) {


	if (!cString) {
		myString = new char[1];
		myString[0] = '\0';
		return;
	}
		
	myLen = strlen(cString);
	myString = new char[myLen+1];
	for (stringLengthType i = 0; i<myLen; i++)
		myString[i] = cString[i];
	myString[myLen] = '\0';

}

// create a new string from an integer
String::String(int nStr) {

	char cString[MAX_INT_LEN];
	snprintf(cString, MAX_INT_LEN, "%d", nStr);

	myLen = strlen(cString);
	myString = new char[myLen+1];
	for (stringLengthType i = 0; i<myLen; i++)
		myString[i] = cString[i];
	myString[myLen] = '\0';

}

// create a new string from an unsigned integer
String::String(unsigned int nStr) {

	char cString[MAX_INT_LEN];
	snprintf(cString, MAX_INT_LEN, "%d", nStr);

	myLen = strlen(cString);
	myString = new char[myLen+1];
	for (stringLengthType i = 0; i<myLen; i++)
		myString[i] = cString[i];
	myString[myLen] = '\0';

}

// copy constructor
String::String(const String& rhs) {
	myLen = rhs.length();
	myString = new char[myLen+1];
	for (stringLengthType i = 0; i < myLen; i++)
		myString[i] = rhs[i];
	myString[myLen] = '\0';
}

// destructor - free memory
String::~String(void) {

	delete[] myString;
	myLen = 0;

}

// operator equals, frees existing memory then copies string and size
String& String::operator = (const String& rhs) {

	if (this == &rhs)
		return *this;

	delete[] myString;
	myLen=rhs.length();
	myString = new char[myLen+1];
	for (stringLengthType i = 0; i<myLen; i++)
		myString[i] = rhs[i];
	myString[myLen] = '\0';
	return *this;

}

// non constant offset operator, returns reference to character so it can be changed 
char& String::operator [] (stringLengthType offset) {

	if (offset > myLen)
		return myString[myLen-1];
	else
		return myString[offset];

}

// contant offset operator for use on const objects (see copy constructor)
char String::operator [] (stringLengthType offset) const {

	if (offset > myLen)
		return myString[myLen-1];
	else
		return myString[offset];

}

// creates a new string by adding current string to rhs
String String::operator + (const String& rhs) {

	stringLengthType totalLen = myLen + rhs.length();
	String temp(totalLen);
	stringLengthType i;
	for (i = 0; i<myLen; i++)
		temp[i] = myString[i];
	for (stringLengthType j = 0; j<rhs.length(); j++, i++)
		temp[i] = rhs[j];
	temp[totalLen] = '\0';
	return temp;

}


// changes current string, returns nothing
void String::operator += (const String& rhs) {

	stringLengthType rhsLen = rhs.length();
	stringLengthType totalLen = myLen + rhsLen;
	String temp(totalLen);

	stringLengthType i;
	for (i = 0; i < myLen; i++)
		temp[i] = myString[i];

	for (stringLengthType j = 0; j < rhsLen; j++, i++)
		temp[i] = rhs[i-myLen];

	temp[totalLen] = '\0';
	*this = temp;

}

// changes current string, returns nothing
void String::operator += (const char& rhs) 
{
	
    stringLengthType rhsLen = 1;
	stringLengthType totalLen = myLen + rhsLen;
	String temp(totalLen);

	stringLengthType i;
	for (i = 0; i < myLen; i++)
		temp[i] = myString[i];

	temp[i] = rhs;

	temp[totalLen] = '\0';
	*this = temp;

}

// return a string composed of the characters before character c 
String String::before(int c)
{

	if (c > myLen)
		return *this;   // create new string through copy constructor based on our whole string
	else {
		
		stringLengthType totalLen = c-1;
		String temp(totalLen);

		for (stringLengthType i = 0; i < totalLen; i++)
			temp[i] = myString[i];

		temp[totalLen] = '\0';
		return temp;

	}

}

// makes the string all uppercase
void String::toupper(void) {
	for (char * p = myString; *p; p++) {
		if (islower(*p))
				   *p = ::toupper(*p);
	}
}


// trim whitespace on either end of string
void String::trim(void) {

	bool firstChar = true;
	bool restWS;
	int bufChar = 0;
	int la;

	// buffer will never be longer than our current string...
	char tBuf[myLen+1];
	for (int j = 0; j <=myLen; j++)
		tBuf[j] = '\0';

	// loop for each character in our buffer 
	for (stringLengthType i = 0; i < myLen; i++) {

		// if we haven't hit a letter yet and it's whitespace, dont copy it to buffer
		if ((firstChar) && (isspace(myString[i])))
			continue;

		// if we've already hit a non-whitespace character, and THIS is a whitespace, look ahead real quick
		// and see if the rest are all whitespace
		else if ((!firstChar) && (isspace(myString[i]))) {
			restWS = true;
			for (la = i+1; la < myLen; la++) {
				if (!isspace(myString[la])) {
					restWS = false;
					break;
				}
			}
			// if restWS is true, then we can stop copying here. otherwise, keep going, AND copy the current
			// space because it's not on either end, it's in the middle
			if (restWS) {
				break;
			}
			else {
				tBuf[bufChar++] = myString[i];  // which will be the white space character ...
			}
		}
		else {
			// otherwise, it was a non whitespace, we should copy it
			firstChar = false;
			tBuf[bufChar++] = myString[i];
		}

	}

	// now clear our old string, and redo it with the new one
	delete [] myString;
	myLen = bufChar;
	myString = new char[bufChar];
	for (stringLengthType i = 0; i<myLen; i++)
		myString[i] = tBuf[i];
	myString[myLen] = '\0';

}



/********************/
// FRIEND FUNCTIONS //
/********************/

// stream operator
ostream& operator << (ostream& theStream, String& theString) 
{

	theStream << theString.getCstr();
	return theStream;

}

// create a new string by adding one string to another
String operator + (const String& lhs, const String& rhs) {

	stringLengthType totalLen = lhs.length() + rhs.length();
	String temp(totalLen);

	stringLengthType i;

	for (i = 0; i<lhs.length(); i++)
		temp[i] = lhs[i];
	for (stringLengthType j = 0; j < rhs.length(); j++, i++)
		temp[i] = rhs[j];
	temp[totalLen] = '\0';

	return temp;

}

// split a string into a bunch of strings based on a delimiter
int split(String& src, String results[], int max, char delim=' ') {

	// buffer we'll use to store each split result in 
	char tBuf[src.length()+1];

	// the character array we'll be looking through
	const char * cBuf = src.getCstr();
	char * p = tBuf;

	// count of split we're on 
	int numSplits = 0;

	while ((*cBuf)&&(numSplits < max)) {

		if (*cBuf == delim)	{
			*p = '\0';
			results[numSplits++] = tBuf;
			p = tBuf;
		}
		else {
			*p++ = *cBuf;
		}

		cBuf++;

	}

	// if numSplits > 0, we still need to copy the last split to the buffer, if there's still room 
	if ((numSplits >= 0)&&(numSplits < max)) {
			*p = '\0';
			results[numSplits++] = tBuf;
			p = tBuf;
	}

	return numSplits;

}

