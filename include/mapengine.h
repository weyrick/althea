/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for MapEngineClass. contains all information needed for maps and sectors.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include <fstream.h>
#include "defs.h"
#include "listhash.h"
#include "log.h"
#include "eventengine.h"
#include "sector.h"

#ifndef AMAPENGINE_H
#define AMAPENGINE_H

/* LOCAL DEFINES */ 
#define MAP_STATUS_OK       	0
#define MAP_ERROR_SECTORNF    	1
#define MAP_ERROR_CELLNF    	2
#define MAP_ERROR_OBJECTNF     	3	
#define MAP_ERROR_UNKNOWN    	4

// the main map engine
class MapEngineClass {

public:
	MapEngineClass(void) {

        // load/create the root sector
        loadSector("world/sectors/root.s");

    }

	~MapEngineClass(void) {
		sectorList.freeList();
	}

	// insert an object into the map engine
    int insertObject(String objID);

	// remove an object from the map engine
    int removeObject(String objID);

	// move an object from one place in the world to another, or return false if we couldn't
    bool moveObject(aWorldCoords newLocation, String objID);

	// load a new sector
	void loadSector(String fn);

private:
	Hash<aMapSector> sectorList;	// our sector list

};

#endif

