/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aWorldObject. all objects in the world are extended from this class. 
        cannot be instantiated directly -- must be sub classed to implement virtuals
        aWorldObjects can handler events from EventEngine via eventHandler function

	CHANGE HISTORY:
	


 */
 
 
 
#ifndef AOBJECT_H
#define AOBJECT_H


/* LOCAL INCLUDES */
#include <stdio.h>
#include "defs.h"
#include "String.h"
#include "coords.h"

class aWorldObject;		// events.h needs to know what this is
#include "events.h"
#include "universeTime.h"

/* LOCAL DEFINES */
#define EVENT_HANDLER_STATUS_OK     0
#define EVENT_HANDLER_ERROR         1

// base world object class
class aWorldObject
{
	
public:

	// constructor
	aWorldObject(void) {
		objectType = AOBJECT_UNKNOWN;           // start off as unknown object
		uniqueID = genID();                     // generate a unique ID for this object
        ageStamp = universeTime.timeStamp();    // timestamp of object creation, in universe time
	}

	// virtual descrutor
	virtual ~aWorldObject(void) { /* nothing */ }

	// return text string of our world coordinates
	String getCoordText(void) { return location.getCoordText(); }

	// get the objects location
	aWorldCoords& getCoordinates(void) { return location; }

	// event handler for this object
	// must be overidden
	virtual int eventHandler(aEventClass * event) = 0;

	// return our unique ID
	String getID(void) { return uniqueID; }

	// set object name
	void setName(String n) { objName = n; }

	// set short description
	void setShortDesc(String d) { objShortDesc = d; }

	// set long description
	void setLongDesc(String d) { objLongDesc = d; }

	// get object name
	String getObjName(void) { return objName; }

	// get object short description
	String getObjShortDesc(void) { return objShortDesc; }

	// get object long description
	String getObjLongDesc(void) { return objLongDesc; }

	// set location. make sure worldEngine knows about this if it gets changed!
	void setLocation(aWorldCoords newLocation) { location = newLocation; }

    // get the time stamp of when the object was created, in universe time
    timeStampType getAgeStamp(void) { return ageStamp; }

// private: all code (including derived objects!) may not access these variables directly
private:
	String uniqueID;					// get a unique id for this object
	enum aWorldObjectType objectType;   // type of object we are dealing with
    timeStampType   ageStamp;           // timestamp of the world time when created

// protected: derived objects can use these freely (as public), but code that accesses
//            an object instantiated from derived objects see them as private
protected:
	// set this objects type 
	void setObjectType(enum aWorldObjectType newType) { objectType = newType; }

	aWorldCoords location;    			// location in the world coordinates        

    // textual descriptives	(for non detailed look)
	String objName;						// the objects name
	String objShortDesc;				// the objects short description
	String objLongDesc;					// the objects long description

    // physical properties of the object
    unsigned int    cellSpace;              // how large (in cells) is this object.
    enum aWorldObjectSpaceType spaceType;   // how the cells are laid out
    float           weight;                 // how heavy is this object (pounds)
    unsigned char percSpace;    			// what percent cell space does this object take up

    // how many cells TALL is this object??
	// how much is this object worth in monetary value?


};


#endif
