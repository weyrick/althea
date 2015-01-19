/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for String class

	CHANGE HISTORY:
	


 */

 
#ifndef ASTRING_H
#define ASTRING_H

/* LOCAL INCLUDES */
#include <iostream.h>
#include <string.h>
#include <stdlib.h>

class String;
#include "support.h"

/* LOCAL DEFINES */
#define MAX_INT_LEN		10  							// maximum # of digits we'll convert from int -> String 								 
typedef unsigned short int stringLengthType;

// String Class
class String {

public:
	// constructors
	String(void);
	String(int nStr); 
	String(unsigned int nStr);
	String(const char *const cString);
	String(const String&);

	~String(void);

	// overloaded operators
	char &operator [] (stringLengthType offset);
	char operator [] (stringLengthType offset) const;

	String operator + (const String&);
	friend String operator + (const String&, const String&);

	void operator += (const String&);
	void operator += (const char&);
	String& operator = (const String&);

	friend ostream& operator << (ostream& theStream, String& theString);
	friend int operator == (const String& x, const String& y)  { return scmp(x.getCstr(), y.getCstr()) == 0; }
	friend int operator != (const String& x, const String& y)  { return scmp(x.getCstr(), y.getCstr()) != 0; }
    friend int operator > (const String& x, const String& y)  { return scmp(x.getCstr(), y.getCstr()) > 0; }
    friend int operator >= (const String& x, const String& y)  { return scmp(x.getCstr(), y.getCstr()) >= 0; }
	friend int operator < (const String& x, const String& y)  { return scmp(x.getCstr(), y.getCstr()) < 0; }
	friend int operator <= (const String& x, const String& y)  { return scmp(x.getCstr(), y.getCstr()) <= 0; }
	
	friend int operator == (const String& x, const char * y)  { return scmp(x.getCstr(), y) == 0; }
	friend int operator != (const String& x, const char * y)  { return scmp(x.getCstr(), y) != 0; }
    friend int operator > (const String& x, const char * y)  { return scmp(x.getCstr(), y) > 0; }
    friend int operator >= (const String& x, const char * y)  { return scmp(x.getCstr(), y) >= 0; }
	friend int operator < (const String& x, const char * y)  { return scmp(x.getCstr(), y) < 0; }
	friend int operator <= (const String& x, const char * y)  { return scmp(x.getCstr(), y) <= 0; }

	// conversion operators
	operator const char * () { return myString; }

	// returns a string composed of the characters before character c
	String before(int c);

	// converts this string to all upper case 
	void toupper(void);

	// trim white space from left and right sides (including newlines, spaces and tabs)
	void trim(void);

	// split a string into an array of strings off some delimiter. returns the number of strings split 
	friend int split(String& src, String results[], int max, char delim=' ');

	// general accessors
	stringLengthType length(void) const { return myLen; }

	// return a "c compatible" pointer to a character array based on this string
	const char * getCstr(void) const { return myString; }

	// clear this string back to a single null
	void clear(void) {
		delete [] myString;
		myString = new char[1];
		myString[0] = '\0';
		myLen = 0;
	}

private:
	String (stringLengthType);		// private constructor 
	char * myString;
	stringLengthType myLen;

};


#endif
