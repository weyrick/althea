/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		EventEngineClass header file.

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include "events.h"
#include "defs.h"
#include "listhash.h"
#include "String.h"
#include "aobject.h"		
 
#ifndef AEVENTENGINE_H
#define AEVENTENGINE_H

// maintainer of events 
class EventEngineClass {

public:

	// destructor
	~EventEngineClass(void) {
		eventQueue.freeList();
	}

	// throw an event, creating it on the fly
	unsigned int throwEvent(aEventType eType, String targetID);

	// throw a precreated event
	unsigned int throwEvent(aEventClass * newEvent);

	// receive a timeslice and handle events
	void timeSlice(void);

private:
	Hash<aEventClass>	eventQueue;		// our queue of events

};

// global eventEngine instantiated in main.cpp
extern EventEngineClass eventEngine;

#endif

