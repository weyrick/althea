/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
	

	CHANGE HISTORY:
	


 */
 
#include "sector.h"
#include "worldengine.h"
 
// add a cell to this sector
int aMapSector::addCell(aMapCell * newCell) {
	int retVal = cellMap.addItem(newCell, newCell->getCoordText());
	if (retVal)
		Log.logError("aMapSector::addCell: couldn't add new cell: "+newCell->getCoordText());
	return retVal;
}


// add an object into this sector, also adding to cell
int aMapSector::addObjectToSector(String objID) {

	// get the object from the world engine
	aWorldObject * newObj = worldEngine.getObject(objID);
	if (!newObj)
		Log.logError("aMapSector::addObjectToSector: couldn't retrieve object for insertion into sector");

	// find the cell of the location we're adding them to the sector at
	aMapCell * theCell = cellMap.findItem(newObj->getCoordText());

	if (theCell) {
		// found it, add them to the cell
		if (theCell->addObjectToCell(objID))
			Log.logWarning("aMapSector::addObjectToSector: theCell->addObjectToCell(newObj) call failed");
	}
	else {
		Log.logWarning("aMapSector::addObjectToSector: couldn't find requsted cell in this sector to add object: "+objID+", at location "+newObj->getCoordText());
		// couldn't find cell, return error
		return SECTOR_ERROR_CELLNF;
	}

	// add them to our sector object list
	String * newObjID = new String(objID);
	if (objList.addItem(newObjID, objID)) {
		Log.logError("aMapSector::addObjectToSector: couldn't add requested object to local sector object list: "+objID);
		return SECTOR_ERROR_UNKNOWN;
	}

	// everythings ok
	return SECTOR_STATUS_OK;
}

// remove object from sector. also remove from cell. DONT erase data.
int aMapSector::removeObjectFromSector(String id) {

	// found this object in sector
	if (objList.findItem(id)) {

		aWorldObject * obj = worldEngine.getObject(id);
		if (!obj)
			Log.logError("aMapSector::removeObjectFromSector: found object in sector list, but couldn't retrieve same object from worldEngine");

		// find the cell of the location this object was at
		aMapCell * theCell = cellMap.findItem(obj->getCoordText());

		if (theCell) {
			// found it, remove them from the cell
			if (theCell->removeObjectFromCell(id))
				Log.logError("aMapSector::removeObjectFromSector: found object in sector list, but cell object list couldn't find same object!");
		}
		else {
			Log.logError("aMapSector::removeObjectFromSector: couldn't find requested cell location "+obj->getCoordText()+", which should exist since an object was found at that location!");
			// couldn't find cell, return error
			return SECTOR_ERROR_CELLNF;
		}

	}
	// didn't have the objectin this sector
	else {
		Log.logWarning("aMapSector::removeObjectFromSector: couldn't find requested object for removal from sector: "+id);
		// couldn't find the object
		return SECTOR_ERROR_OBJECTNF;
	}

	// remove the object from our list, but dont delete the data
	objList.removeItem(id);

	// everythings ok
	return SECTOR_STATUS_OK;
}
	

