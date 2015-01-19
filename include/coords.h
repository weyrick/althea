/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		world coordinates. used whenever referencing a location in the world.
		Z is north/south
		X is east/west 
		Y is up/down

	CHANGE HISTORY:
	


 */
 
/* LOCAL INCLUDES */
#include "String.h"
#include "log.h"
 
#ifndef ACOORDS_H
#define ACOORDS_H

class aWorldCoords {
public:

	// create new world coordinates variable based on passed info
	aWorldCoords(int x, int y, int z, unsigned int s) {
			worldX = x;
			worldY = y;
			worldZ = z;
            sector = s;
	}

	// create new world coordinates variable, set to 0,0,0,0
	aWorldCoords(void) {
			worldX = 0;
			worldY = 0;
			worldZ = 0;
            sector = 0;
	}

	// create new world coordinates variable, based on aWorldCoords variable passed
	aWorldCoords(const aWorldCoords & loc) {
		worldX = loc.worldX;
		worldY = loc.worldY;
		worldZ = loc.worldZ;
        sector = loc.sector;
	}

    // create new world coordinates variable, based on text string in the form X,Y,Z,S
    aWorldCoords(String coordText) {

        String sCoords[4];
        if (split(coordText, sCoords, 4, ',') == 4) {
            worldX = atoi(sCoords[0]);
            worldY = atoi(sCoords[1]);
            worldZ = atoi(sCoords[2]);
            sector = atoi(sCoords[3]);
        }
		else
			Log.logWarning("aWorldCoords::aWorldCoords(String): couldn't parse X,Y,Z,S passed ["+coordText+"]");

    }

	// increase Z by 1. returns reference to self.
	aWorldCoords& moveNorth(void) { worldZ++; return *this; }
	// decrease Z by 1. returns reference to self
	aWorldCoords& moveSouth(void) { worldZ--; return *this; }
	// increase X by 1. returns reference to self
	aWorldCoords& moveEast(void) { worldX++; return *this; }
	// decrease X by 1. returns reference to self
	aWorldCoords& moveWest(void) { worldX--; return *this; }
	// increase Y by 1. returns reference to self
	aWorldCoords& moveUp(void) { worldY++; return *this; }
	// decrease Y by 1. returns reference to self
	aWorldCoords& moveDown(void) { worldY--; return *this; }

	// increase Z by desired amount
	void moveNorth(int num) { worldZ+=num; }
	// decrease Z by desired amount
	void moveSouth(int num) { worldZ-=num; }
	// increase X by desired amount
	void moveEast(int num) { worldX+=num; }
	// descrease X by desired amount
	void moveWest(int num) { worldX-=num; }
	// increase Y by desired amount
	void moveUp(int num) { worldY+=num; }
	// decrease Y by desired amount
	void moveDown(int num) { worldY-=num; }

	// set the location of this variable
	void set(int x, int y, int z, int s) {
		worldX = x;
		worldY = y;
		worldZ = z;
        sector = s;
	}

	// get X coord
	int getX(void) { return worldX; }
	// get Y coord
	int getY(void) { return worldY; }
	// get Z coord
	int getZ(void) { return worldZ; }
	// get sector
    unsigned int getSector(void) { return sector; }

	// get a text versino of the coordinates
	String getCoordText(void) { return (String)sector+":"+(String)worldX+","+(String)worldY+","+(String)worldZ; }

	// assignment operator, for setting one aWorldCoords variable equal to another
	aWorldCoords operator = (const aWorldCoords& loc) {
		if (this == &loc)
			return *this;

		worldX = loc.worldX;
		worldY = loc.worldY;
		worldZ = loc.worldZ;
        sector = loc.sector;
		return loc;
	}

private:
	int worldX;		// current X location
	int worldY;		// current Y location
	int worldZ;		// current Z location
    unsigned int sector;	// current sector

};

#endif
	
