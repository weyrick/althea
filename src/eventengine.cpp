/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		implementation for eventEngineClass. core event system.

	CHANGE HISTORY:
	


 */

#include "eventengine.h"
#include "worldengine.h"



// we're getting a timeslice, handle an event
void EventEngineClass::timeSlice(void) {

	// do we have any events to handle?
	if (eventQueue.getNumItems()) {

		// yep, run all events for this tick
		eventQueue.resetCounter();
		aEventClass * tEvent = eventQueue.current();

		// loop through all events, throwing to each target
		aWorldObject * targetObject;
		while (tEvent) {
			targetObject = worldEngine.getObject(tEvent->getTargetID());
			if (!targetObject)
				Log.logError("EventEngineClass::timeSlice: couldn't retrieve target object for eventHandler() class");
			targetObject->eventHandler(tEvent);
			tEvent = eventQueue.next();
		}

		// now erase the event queue for this tick
		eventQueue.freeList();

	}

}

// add a new event to the systems event queue
unsigned int EventEngineClass::throwEvent(aEventClass * newEvent) {

	unsigned int retVal = eventQueue.addItem(newEvent, newEvent->getEventID());	
	if (retVal)
		Log.logError("aEventClass::throwEvent() couldn't add event to event queue");
	return retVal;

}

// add a new event to the systems event queue, building an event object on the fly
unsigned int EventEngineClass::throwEvent(aEventType eType, String targetID) {
	
	aEventClass * newEvent = new aEventClass(eType, targetID);
	unsigned int retVal = eventQueue.addItem(newEvent, newEvent->getEventID());
	if (retVal)
		Log.logError("aEventClass::throwEvent() couldn't add event to event queue");
	return retVal;
   
}

