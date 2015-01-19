/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		map sector

	CHANGE HISTORY:
	


 */

#ifndef ASECTOR_H
#define ASECTOR_H

#include "cell.h"

/* LOCAL DEFINES */
#define SECTOR_STATUS_OK		0
#define SECTOR_ERROR_CELLNF		1
#define SECTOR_ERROR_UNKNOWN 	2
#define SECTOR_ERROR_OBJECTNF  	3

#define SECTOR_LOAD_SEARCH   	0
#define SECTOR_LOAD_SECTOR   	1
#define SECTOR_LOAD_MATERIAL   	2
#define SECTOR_LOAD_CELL     	3
    
// defining	this will print more debug information about sectors when running
#define DEBUG_SECTORS	1

// a sector in the world. each sector has it's own coordinate system.
class aMapSector {

public:
    aMapSector(void) {

		// setup
        sectorName = "undefined sector";
        sectorNumber = "-1";

    }

	// cleanup
	~aMapSector(void) {
		objList.freeList();
		cellMap.freeList();
	}

	// add a new (precreated) cell to this sector
	int addCell(aMapCell * newCell);

	// return a reference to our internal object list, which contains a list of all objects in this sector
    Hash<String>& getObjectList(void) { return objList; }

	// add an object to this sector
    int addObjectToSector(String objID);

	// remove an object from this sector
    int removeObjectFromSector(String objID);

    // return a cell from this sector
    aMapCell * getCell(aWorldCoords loc) { return cellMap.findItem(loc.getCoordText()); }

	// get the sector name
    String getSectorName(void) { return sectorName; }

	// set the sector name
    void setSectorName(String n) { sectorName = n; }

	// get the sector number
    String getSectorNumber(void) { return sectorNumber; }

	// set the sector number
    void setSectorNumber(String n) { sectorNumber = n; }

private:
	Hash<String> objList;  		// list of objects present in this sector, keyed by object id
    Hash<aMapCell> cellMap;		// our list of cells

    String sectorName;      // the name of this sector
    String sectorNumber;    // the sector number

	// how high is the ceiling? what kind of ceiling?
	// what level is the floor at in relation to surrounding sectors? what kind of floor?

};

#endif

