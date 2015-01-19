/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aCreatureBodyPart class. used to implement various body parts for creatures.
		these are extended from aWorldObject and carry all the functionality therein (ie, events)

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include "aobject.h"
#include "defs.h"		
 
#ifndef ACREATUREBP_H
#define ACREATUREBP_H
					
class aCreatureBodyPart : public aWorldObject {
public:
	aCreatureBodyPart(void) { setObjectType(AOBJECT_CREATURE_BODYPART); }

private:
	enum aBodyPartType bpType;	// which body part type are we
	HPtype itsMaxHP;			// max hit points                        
	HPtype itsCurrentHP;		// current hit points           
    armourClasses AC;			// armour class structure 

};

#endif

