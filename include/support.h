/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for global support functions

	CHANGE HISTORY:
	


 */

#include "String.h"

#ifndef ASUPPORT_H
#define ASUPPORT_H

// generate a unique string
String genID(void);

// compare two character arrays
int scmp(const char * a, const char * b);

// get our current memory usage
unsigned int getMemUsage(void);

#endif



