/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
	

	CHANGE HISTORY:
	


 */

#ifndef AMATERIALENGINE_H
#define AMATERIALENGINE_H

#include "material.h"
#include "log.h"
		
// keeps track of materials
class MaterialEngineClass {
public:
	MaterialEngineClass(void) {

		// build our list of materials

	}

	~MaterialEngineClass(void) {
		materialList.freeList();
	}

	// load a material from a file
	int loadMaterial(String fn) {
		
		Log.logDebug("MaterialEngineClass::loadMaterial stub");

	}

	// return true if we have a material object by this name
	bool haveMaterial(String m) {
		if (materialList.findItem(m))
			return true;
		else
			return false;
	}

	// return a pointer to the material requested, or NULL if not found
	aMaterial * getMaterial(String m) { return materialList.findItem(m); }

private:
	Hash<aMaterial> materialList;

};


// our global material engine
extern MaterialEngineClass materialEngine;


/* LOCAL DEFINES */
#define MATERIAL_COMP_STATUS_OK				0
#define MATERIAL_COMP_ERROR_BAD_PERCENT		1
#define MATERIAL_COMP_ERROR_MATERIALNF		2
#define MATERIAL_COMP_ERROR_OUTOFROOM		3

// associates a material with how much of it is composed of
class aMaterialCompositionClass {
public:

	~aMaterialCompositionClass(void) {
		percentList.freeList();
		materialList.freeList();
	}

	// add a material to this composition
	int addMaterial(String m, unsigned char perc) {

		if ((perc < 1)||(perc > 100))
			return MATERIAL_COMP_ERROR_BAD_PERCENT;

		
		if (perc+totalPercent <= 100) {

			// ok, we've got room to add this material
			aMaterial * newMaterial =	materialEngine.getMaterial(m);
			if (!newMaterial) {
				// material not found
				Log.logWarning("aMaterialCompositionClass::addMaterial: material not found: "+m);
				return MATERIAL_COMP_ERROR_MATERIALNF;
			}

			// else, we found it, so add it in
			if (materialList.addItem(newMaterial, m))
				Log.logError("aMaterialCompositionClass::addMaterial: coulnd't add material to local list");
			unsigned char * lPerc = new unsigned char(perc);
			if (percentList.addItem(lPerc, m))
				Log.logError("aMaterialCompositionClass::addMaterial: coulnd't add percent to local list");
			totalPercent += perc;

			return MATERIAL_COMP_STATUS_OK;

		}
		else {

			// didn't have room to add all of this material, so add all that 
			// we can and let caller know we couldn't fit it all
			int newPerc = (100-totalPercent);

			aMaterial * newMaterial =	materialEngine.getMaterial(m);
			if (!newMaterial) {
				// material not found
				Log.logWarning("aMaterialCompositionClass::addMaterial: material not found: "+m);
				return MATERIAL_COMP_ERROR_MATERIALNF;
			}

			// else, we found it, so add it in
			if (materialList.addItem(newMaterial, m))
				Log.logError("aMaterialCompositionClass::addMaterial: coulnd't add material to local list");
			unsigned char * lPerc = new unsigned char(newPerc);
			if (percentList.addItem(lPerc, m))
				Log.logError("aMaterialCompositionClass::addMaterial: coulnd't add (new) percent to local list");
			totalPercent = 100;

			return MATERIAL_COMP_ERROR_OUTOFROOM;

		}

	}

	// see what percent of this composition is a certain material
	unsigned char getPercentage(String m) {

		aMaterial * mat = materialList.findItem(m);
		if (!mat) {
			Log.logWarning("aMaterialCompositionClass::getPercentage: didn't find requested material: "+m);
			return 0;
		}
		else {
			unsigned char * perc = percentList.findItem(m);
			if (!perc)
				Log.logError("aMaterialCompositionClass::getPercentage: material and percentage list are desynched. I found the material, but not the percentage!");
			else
				return *perc;
		}

	}

	// get a list of materials for this composition
	int getMaterialList(String dest[], int max) { return materialList.getKeys(dest, max); }

private:

	unsigned char totalPercent;					// current total percentage of this composition

	// is material visible??
	Hash<unsigned char> percentList;	// what percent each material makes up
	Hash<aMaterial> materialList;		// list of materials in this composition

};

#endif

