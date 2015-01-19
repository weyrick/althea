/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		map engine. managers sectors and cells.

	CHANGE HISTORY:
	


 */

#include "mapengine.h"
#include "worldengine.h"
#include "configreader.h"
    
// insert an object into the world at the specified location
int MapEngineClass::insertObject(String objID) { 

	// get the object from the world engine
	aWorldObject * newObj = worldEngine.getObject(objID);
	if (!newObj)
		Log.logError("MapEngineClass::insertObject: couldn't retrieve object for insertion into map");

	// see if this sector exists
	aMapSector * sector = sectorList.findItem(newObj->getCoordinates().getSector());
	if (sector) {
		
		// found sector, add this object
		int retVal = sector->addObjectToSector(objID);
		if (retVal)
			Log.logWarning("MapEngineClass::insertObject: sector->addObjectToSector call failed");
		return retVal;

	}
	else {
		Log.logWarning("MapEngineClass::insertObject: sector not found to insert object into: "+newObj->getCoordText());
		// didn't find sector
		return MAP_ERROR_SECTORNF;
	}

}

// remove an object from the world based on the object's id
int MapEngineClass::removeObject(String objID) {
	
	// get the object from the world engine
	aWorldObject * obj = worldEngine.getObject(objID);
	if (!obj)
		Log.logError("MapEngineClass::removeObject: couldn't retrieve object for removal from map");
	
	// see if this sector exists
	aMapSector * sector = sectorList.findItem(obj->getCoordinates().getSector());

	if (sector) {
		
		// found sector, add this object
		int retVal = sector->removeObjectFromSector(objID);
		if (retVal)
			Log.logWarning("MapEngineClass::removeObject: sector->removeObjectFromSector call failed");
		return retVal;

	}
	else {
		Log.logError("MapEngineClass::removeObject: sector not found to remove object from: "+obj->getCoordText());
		// didn't find sector
		return MAP_ERROR_SECTORNF;
	}

}


// move an object in the world. 
// check to see if it's possible, and execute if so.
// if not, create a message event explaining why not.
// return true if we succeded, false otherwise.
bool MapEngineClass::moveObject(aWorldCoords newLocation, String objID) {


	// get the object from the world engine
	aWorldObject * obj = worldEngine.getObject(objID);
	if (!obj)
		Log.logError("MapEngineClass::moveObject: couldn't retrieve object from worldEngine to move in map");

	// figure out what sector we're working with
	aMapSector * sector = sectorList.findItem(obj->getCoordinates().getSector());

	// sanity check
	if (!sector) {
		Log.logError("MapEngineClass::moveObject: sector not found for object ["+objID+"] at location ["+obj->getCoordText()+"]");
		// woops, not found
		return false;
	}

	// ok, get the cell from the sector
	aMapCell * cellToMoveTo = sector->getCell(newLocation);

	if (!cellToMoveTo) {
		// couldn't find that cell in our list of cells for that sector
		PLAYER_MESSAGE(obj, "You cannot move that way -- it's off the map!");
		return false;
	}
	else if (!cellToMoveTo->isPassable()) {
		// this cell is not passable
		PLAYER_MESSAGE(obj, "You cannot move that way -- your passage is blocked by an unknown force.");
		return false;
	}
	else if (!cellToMoveTo->canFit(objID)) {
		// the cell wont hold the object that wants to move there
		// we'll tell them what they see as to why not
		Hash<String>& objList = cellToMoveTo->getObjectList();
		String nameList;
		aWorldObject * tObj;
		objList.resetCounter();
		String * bObjID = objList.current();
		while (bObjID) {
			tObj = worldEngine.getObject(*bObjID);
			if (!tObj)
				Log.logError("MapEngineClass::moveObject: couldn't retrieve object from world engine in object name display");
			nameList += tObj->getObjName() + ", ";
			bObjID = objList.next();
		}
		nameList = nameList.before(nameList.length()-1);
		nameList += " blocks your entrance to this location.";
		PLAYER_MESSAGE(obj, nameList);
		return false;
	}
	else {

		// the cell is available to move to.
		// remove them from their old cell, and add them to their new one
		aMapCell * currentCell = sector->getCell(obj->getCoordinates());
	
		if (!currentCell) {
			Log.logError("MapEngineClass::moveObject: could not get current cell of object  ["+objID+"] at location ["+obj->getCoordText()+"]");
			return false;
		}

		if (currentCell->removeObjectFromCell(objID))
			Log.logWarning("MapEngineClass::moveObject:  currentCell->removeObjectFromCell call failed");

		if (cellToMoveTo->addObjectToCell(objID))
			Log.logWarning("MapEngineClass::moveObject: cellToMoveTo->addObjectToCell call failed");
	
		obj->setLocation(newLocation);

		return true;
	}

}


// load a new sector
void MapEngineClass::loadSector(String fn) {
   
	// parse the sector file
	Log.logDebug("MapEngineClass::loadSector: loading new sector from ["+fn+"]");
	ConfigReader sectorConfig(fn);

	if (!sectorConfig.parse()) {
		Log.logWarning("MapEngineClass::loadSector: parsing of sector file failed -- sector not loaded");
		return;
	}

	configBlock * sectorBlock = sectorConfig.getBlock("sector");

	if (!sectorBlock) {
		Log.logWarning("MapEngineClass::loadSector: sector file did not containt sector {} block");
	}

	// create the new sector
	aMapSector * sector = new aMapSector();
	if (!sector) {
		Log.logError("MapEngineClass::loadSector: can't get memory for new sector");
		return;
	}

	// setup the sector information
	sector->setSectorName(sectorBlock->getPair("name"));
	if (sector->getSectorName() == "") {
		Log.logWarning("MapEngineClass::loadSector: sector name wasn't specified -- sector not loaded");
		return;
	}
	sector->setSectorNumber(sectorBlock->getPair("number"));
	if (sector->getSectorNumber() == "") {
		Log.logWarning("MapEngineClass::loadSector: sector number wasn't specified -- sector not loaded");
		return;
	}

	// add the sector
	if (sectorList.addItem(sector, sector->getSectorNumber())) {
		Log.logError("MapEngineClass::loadSector: unable to add sector to world");
	}

	// add the cells
	Log.logDebug("mapengineclass::****** loadsector load cells stub ********");

	Log.logDebug("MapEngineClass::loadSector: added new sector successfully: "+sector->getSectorName()+"/"+sector->getSectorNumber());

} // end load sector function

