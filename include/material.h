/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
	    a material. used to build objects and walls for cells.

	CHANGE HISTORY:
	


 */

#ifndef AMATERIAL_H
#define AMATERIAL_H

#include "defs.h"
 
// a particular way in which a material can be affected	
class aMaterialAffect {

public:
	~aMaterialAffect(void) {
		adjList.freeList();
		damageAmtList.freeList();
	}

protected:
	String affectName;				// unique state name (ie, "water" would be how this material is affected by water)

	bool repairable;				// whether this affect can be repaired (damage level reduced)
	
	unsigned char resistance;		// how well this material resists this particular affect 0-255

	// description list, based on 0-255 damage info
	Hash<String> adjList;					// this list of adjectives
	Hash<unsigned char> damageAmtList;	// assocaited damage level needed for adjList

};

// a single material 
// a material is defined once and is constant information available to a materialState
// materials are loaded and setup in the material engine.
class aMaterial {

public:
    ~aMaterial(void) {
		affectList.freeList();
	}

private:
    String name;                    				// name of the material. must be unique.			
	bool repairable;								// can this material be "repaired"? ie, glass can't, iron can.

	Hash<aMaterialAffect> affectList; 	// what affects this material?

};


#endif

