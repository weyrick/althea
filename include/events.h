/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		aEventClass header file. also defines event types.

	CHANGE HISTORY:
	


 */




#ifndef AEVENTS_H
#define AEVENTS_H

/* LOCAL INCLUDES */
class aEventClass;
#include "String.h"
#include "support.h"
#include "aobject.h"
#include "defs.h"
#include "listhash.h"
#include "log.h"


/* -=-=-=-=-=-=-=-=-=-= EVENT DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=- */

const aEventType AEVENT_NULL 	= 0;
const aEventType AEVENT_FIRST 	= 10000;

const aEventType AEVENT_CREATURE_MOVE		= AEVENT_FIRST + 1;	
const aEventType AEVENT_PLAYER_QUIT 		= AEVENT_FIRST + 2;
const aEventType AEVENT_CREATURE_MOVE_N		= AEVENT_FIRST + 3;
const aEventType AEVENT_CREATURE_MOVE_S		= AEVENT_FIRST + 4;
const aEventType AEVENT_CREATURE_MOVE_E		= AEVENT_FIRST + 5;
const aEventType AEVENT_CREATURE_MOVE_W		= AEVENT_FIRST + 6;
const aEventType AEVENT_CREATURE_MESSAGE	= AEVENT_FIRST + 7;

/* -=-=-=-=-=-=-=-=-=-=- PARAMETER DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=- */

// we'll use these for parameters to events, to avoid typos. this way it wont
// compile if we've made a typo in the parameter

#define AEVENT_PARAM_DIRECTION		"direction"


/* -=-=-=-=-=-=-=-=-=-=-=-= EVENT MACROS -=-=-=-=-=-=-=-=-=-=-=-=- */

// macro for sending a message event to a player
#define PLAYER_MESSAGE(target, message) \
	aEventClass * mEvent = new aEventClass(AEVENT_CREATURE_MESSAGE, target->getID()); \
	if (!mEvent) Log.logError("PLAYER_MESSAGE() macro - couldn't create event object"); \
	mEvent->addString("message",message); \
	eventEngine.throwEvent(mEvent);

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


// Event Class
class aEventClass {
public:
	aEventClass(aEventType eType,  String objID) {

		// type of event
		myEvent = eType;

		// who we'll be throwing this event at
		targetID = objID;

		// unique event ID for this event
		eventID = genID();

		// set our lists to null
        stringPairs = NULL;
		intPairs = NULL;
		objectIDPairs = NULL;

	}

	// cleanup upon destruction of this event
	~aEventClass(void) {

		if (stringPairs)
			delete stringPairs;
		if (intPairs)
			delete intPairs;
		if (objectIDPairs)
			delete objectIDPairs;
			
	}

	// add a String data pair to this event
	int addString(String key, String data) {

		// if we haven't initialized our string hash yet, do so now
		if (!stringPairs)
			stringPairs = new Hash<String>;

		if (!stringPairs)
			Log.logError("aEventClass::addString() couldn't create Hash<String>");

		String * newData = new String(data);
		int retVal;
		if ( (retVal = stringPairs->addItem(newData, key)) )
			Log.logError("aEventClass::addString() couldn't add ["+key+"/"+*newData+"] to stringPairs");

		return retVal;
	}

	// get data from our string pairs, by key
	String getString(String key) {
		if (!stringPairs)
			return "";
		String * s = stringPairs->findItem(key);
		if (s)
			return *s;
		else {
			Log.logWarning("aEventClass::getString() couldn't find requested data/pair: "+key);
			return "";
		}
	}

	// add a aWorldObject data pair to this event
	int addObject(String key, String objID) {

		// if we haven't initialized our hash yet, do so now
		if (!objectIDPairs)
			objectIDPairs = new Hash<String>;

		if (!objectIDPairs)
			Log.logError("aEventClass::addObject() couldn't create Hash<String>");

        int retVal;
		String * newID = new String(objID);
		if ( (retVal = objectIDPairs->addItem(newID, key)) )
			Log.logError("aEventClass::addObject() couldn't add object id to objectPairs");

		return retVal;
	}

	// get data from our object pairs, by key
	String getObject(String key) {

		if (!objectIDPairs)
			return NULL;

		String * fObjID = objectIDPairs->findItem(key);
		if (!fObjID)
			Log.logWarning("aEventClass::getObject() didn't find requested object: "+key);

		return *fObjID;
	}

	// add int data pair to this event
	int addInt(String key, int data) {

		// if we haven't initialized our hash yet, do so now
		if (!intPairs)
			intPairs = new Hash<int>;

		if (!intPairs)
			Log.logError("aEventClass::addInt() couldn't create Hash<int>");

		int * newData = new int(data);
		int retVal;
		if ( (retVal = intPairs->addItem(newData, key)) ) {
			Log.logError("aEventClass::addInt() couldn't add int: "+(String)*newData);
		}

		return retVal;
	}

	// get data from our int pairs, by key
	int getInt(String key) {
		if (!intPairs)
			return 0;
		int * i = intPairs->findItem(key);
		if (i)
			return *i;
		else {
			Log.logWarning("aEventClass::getInt() couldn't find requested data/pair: "+key);
			return 0;
		}
	}

	// return ID of target object of this event
	String getTargetID(void) { return targetID; }

	// return the event ID of this event
	String getEventID(void) { return eventID; }

	// see what kind of event occured
	aEventType getEventType(void) { return myEvent; }


private:
	aEventType	myEvent;		// the type of event we are throwing
	String eventID;				// our eventID
	String targetID;			// object ID of who the event will be thrown at

	// list of data/pairs we can store with our event
	// these will be pointers initialized on the fly
	// so that we dont waste time initializing a hash if we wont use it
	Hash<String> * stringPairs;
	Hash<int>    * intPairs;
	Hash<String> * objectIDPairs;

};

#endif

