/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		mechanism for logging and debugging

	CHANGE HISTORY:
	


 */
 
/* LOCAL INCLUDES */
#include <stdio.h>
#include "String.h"
 
#ifndef ALOG_H
#define ALOG_H

#include "dbengine.h"
  
/* LOCAL DEFINES */
#define LOGLEVEL_PRODUCTION		0		// production setting: critical errors only
#define LOGLEVEL_WARNINGS		1		// show warnings and critical errors
#define LOGLEVEL_DEBUG			2       // debug setting - shows all warnings, errors, and messages

#define LEVEL_CRITICAL			0
#define LEVEL_WARNING			1
#define LEVEL_DEBUG				2

class LogClass {
public:
	LogClass(void) {
		// default to a production logger
		logLevel = LOGLEVEL_PRODUCTION; 
		useStdError = true;
		useDatabase = false;
		stopOnCritical = true;
	}

	// set the level we should debug at
	void setLevel(int l) { logLevel = l; }

	// set whether we should write to stderror or not
	void setUseStdError(bool b) { useStdError = b; }

	// set whether we should write to stderror or not
	void setUseDatabase(bool b) { useDatabase = b; }

	// set whether we should exit() on critical errors
	void setStopOnCritical(bool b) {stopOnCritical = b; }

	// log an error. these should NOT happen at all, and the program won't continue stabley.
	// if stopOnCritical is true, we'll exit the program
	void logError(String message) {
		
		if (useStdError)
			fprintf(stderr, "** CRITICAL: %s\n", message.getCstr());

//		if (useDatabase)
//			dbEngine.logMessage(message,  LEVEL_CRITICAL);

		if (stopOnCritical)
			exit(-1);

	}

	// log a warning. these indicate something that we didn't really expect, but the program should continue stabley.
	void logWarning(String message) {
		
		if (useStdError)
			fprintf(stderr, "WARNING: %s\n", message.getCstr());

//		if (useDatabase)
//			dbEngine.logMessage(message,  LEVEL_WARNING);

	}

	// log a message. these are for debugging purposes and could be anything.
	void logDebug(String message) {

		if (useStdError)
			fprintf(stderr, "DEBUG: %s\n", message.getCstr());

//		if (useDatabase)
//			dbEngine.logMessage(message,  LEVEL_DEBUG);

	}

private:
	int logLevel;		// what level are we currently logging at?
	bool useStdError;	// if this is true, we'll echo to stderror in addition to logging to database
	bool useDatabase;  // if this is true, we'll write the error to database
	bool stopOnCritical; // if this is true, we'll exit() on a critical error (ie, logError)

};
 

// global Log is available to everyone who includes this file (object created in main.cpp)
extern LogClass Log;

#endif
 
