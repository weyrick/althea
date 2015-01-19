/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for WorldEngineClass
		
		the world engine maintains ALL aWorldObject objects in the world. anyone else who needs to reference
		an object will do so by retriving it from this class based on it's unique ID. if another class needs
		to keep a list of object ID's

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include "listhash.h"
#include "String.h"
#include "aobject.h"
#include "mapengine.h"
#include "events.h"

#ifndef AWORLDENGINE_H
#define AWORLDENGINE_H

/* LOCAL DEFINES */
#define WORLDENGINE_STATUS_OK            0
#define WORLDENGINE_ERROR_OBJECT_INSERT  1
#define WORLDENGINE_ERROR_OBJECT_REMOVE  2
	  
class WorldEngineClass {

public:

	// add an object to the world
    int addObject(aWorldObject * newObj);

	// remove an object from the world
    int removeObject(aWorldObject * remObj, bool saveObject=false);

	// someone is asking the world to move an object. if we can do it, then do it.
	// if not, send PLAYER_MESSAGE() as to why not
    // return true if we succeded, false otherwise.
	bool moveObject(aWorldCoords newLocation, String objID) { return mapEngine.moveObject(newLocation, objID); }

	// we're getting a time slice (1 tick)
	// keep here for inline code
	void timeSlice(void) {

		/* nothing */

	}

	// return a pointer to an object by object ID, or NULL if not found
	aWorldObject * getObject(String objID) {
		aWorldObject * target = objectList.findItem(objID);
		if (!target)
			Log.logWarning("WorldEngineClass::getObject: unable to find requested object: "+objID);
		return target;
	}

private:  

	Hash<aWorldObject> objectList;   	// the main object list in the world
	
	MapEngineClass mapEngine;					// map engine


};

// global worldEngine instantiated in main.cpp
extern WorldEngineClass worldEngine;

#endif

