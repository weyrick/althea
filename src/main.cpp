/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		main function and program entry point.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include <time.h>
#include "dbengine.h"
#include "eventengine.h"
#include "universeTime.h"
#include "worldengine.h"
#include "aobject.h"
#include "ioengine.h"
#include "materialengine.h"

/* GLOBAL VARIABLES */

// initialize global Log (first so other classes can use logger)
LogClass Log;

// initialize main world time
universeTimeClass universeTime;                 

// initialize global DB class
dbEngineClass dbEngine;

// initialize global Event Engine
EventEngineClass eventEngine;

// initialize global Material Engine
MaterialEngineClass materialEngine;

// initialize global World Engine
WorldEngineClass worldEngine;

/* IMPLEMENTATION */

// main program entry / loop
int main(void) {
	
	// banner
	cout << PROJECT_NAME << " " << PROJECT_VERSION << endl;

	// setup logger
	Log.setLevel(LOGLEVEL_DEBUG);
	Log.setUseDatabase(false);
	Log.setUseStdError(true);
	Log.setStopOnCritical(true);

	// startup IO engine
	IOEngineClass IOEngine;

	// infinite loop
	for (;;) {
		universeTime.timeSlice();   // give a time slice to the universe time
		IOEngine.timeSlice();		// give a time slice to the IO engine
		eventEngine.timeSlice();	// give time slice to the event engine
		worldEngine.timeSlice();  	// give time slice to the world engine
	}
	
	
}

