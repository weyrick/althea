/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		world engine, maintains world map and all objects in the world.

	CHANGE HISTORY:
	


 */

#include "worldengine.h"
 
// add a new object to the world
int WorldEngineClass::addObject(aWorldObject * newObj) {

	// add them to the object list
	if (objectList.addItem(newObj, newObj->getID())) {
		Log.logError("WorldEngineClass::addObject: unable to add an object to our local list");
		return WORLDENGINE_ERROR_OBJECT_INSERT;
	}
	
	// add them to the map
	if (mapEngine.insertObject(newObj->getID())) {
		Log.logError("WorldEngineClass::addObject: mapEngine.insertObject call failed");
		return WORLDENGINE_ERROR_OBJECT_INSERT;
	}

	
	Log.logDebug("WorldEngineClass::addObject added new object ["+newObj->getID()+"] successfully ("+newObj->getObjName()+")");		

	// eveythings ok
	return WORLDENGINE_STATUS_OK;

}

// add an object to the remove queue for removal upon next tick
// will free the memory of the object being removed, unless told otherwise
int WorldEngineClass::removeObject(aWorldObject * remObj, bool saveObject) {
	
	// save these for later
	String objID = remObj->getID();
	String objName = remObj->getObjName();
	
	// remove object from map
	if (mapEngine.removeObject(objID)) {
		Log.logError("WorldEngineClass::removeObject: call to mapEngine.removeObject failed");
		return WORLDENGINE_ERROR_OBJECT_REMOVE;		
	}

	// remove object from world object list
	if (objectList.removeItem(objID, saveObject)) {
		Log.logError("WorldEngineClass::removeObject: unable to remove object from world object list");
		return WORLDENGINE_ERROR_OBJECT_REMOVE;
	}

	Log.logDebug("WorldEngineClass::removeObject removed object ["+objID+"] successfully ("+objName+")");

	// everythings ok
	return WORLDENGINE_STATUS_OK;

}

