/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
	

	CHANGE HISTORY:
	


 */

#ifndef ACELL_H
#define ACELL_H

#include "coords.h"
#include "aobject.h"

// a single cell in the map of a sector
class aMapCell {

public:
	aMapCell(aWorldCoords loc) {
		location = loc;
		passable = true;
	}

    aMapCell(void) { passable = true; }

	// destructor
	~aMapCell(void) {
		objList.freeList();
	}

    // return the coordinate text of this cells location
	String getCoordText(void) { return location.getCoordText(); }

    // can a creature pass through this cell?
	bool isPassable(void) { return passable; }
	void setPassable(bool p) {	passable=p; }

    // add an object into this cell
	int addObjectToCell(String objID) {
		String * newObjID = new String(objID);
		int retVal = objList.addItem(newObjID, objID);
		if (retVal)
			Log.logError("aMapCell::addObjectToCell: couldn't add object to cell " + location.getCoordText());
		return retVal;
	}
	
	// remove object from cell
	int removeObjectFromCell(String objID) {
		int retVal = objList.removeItem(objID);
		if (retVal)
			Log.logError("aMapCell::removeObjectFromCell: couldn't remove object ["+objID+"] from cell");
		return retVal;
	}

	// get a hash of the objects in this cell
	Hash<String>& getObjectList(void) { return objList; }


	// see if the given object can "fit" in this cell, based on it's size, etc
	bool canFit(String objID) {

		// for now, if somethings in the cell, we'll say it can't fit. ignore testObj.
		if (!objList.isListEmpty())
			return false;
		else
			return true;

	}

    // set the cell's location in the sector
    void setLocation(aWorldCoords loc) { location = loc; }

private:
	aWorldCoords location;  // what location is this cell at? local to this sector
	bool passable;			// is this cell passable?

	Hash<String> objList;  // list of objects present in this cell, by object id

};

#endif

