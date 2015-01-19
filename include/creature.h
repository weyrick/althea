/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aCreature class. extended from aWorldObject, all living creatures in the world
		should be extended or instantiated from this.

	CHANGE HISTORY:
	


 */


 
#ifndef ACREATURE_H
#define ACREATURE_H

/* LOCAL INCLUDES */
class aCreature;
#include "defs.h"
#include "skills.h"
#include "aobject.h"
#include "races.h"
#include "client.h"
#include "eventengine.h"
#include "listhash.h"
#include "creaturebp.h"
#include "worldengine.h"

/* LOCAL DEFINES */
#define IS_PC (myMaster)&&(myMaster->getControlState() == CLIENT_CONTROL_HUMAN)

class aCreature : public aWorldObject {

public:
    // initialize new creature
    aCreature(enum sphereType pS, HPtype mHP, MPtype mMP)  {

        playerSphere = pS;
        maxHP = mHP;
        currentHP = mHP;    // start at full health     
        maxMP = mMP;
        currentMP = mMP;    // start at full move points

        // armour class sets to naked
        AC.acid     = AC_NO_PROTECTION;
        AC.bashing  = AC_NO_PROTECTION;
        AC.cold     = AC_NO_PROTECTION;
        AC.crushing = AC_NO_PROTECTION;
        AC.electric = AC_NO_PROTECTION;
        AC.gas      = AC_NO_PROTECTION;
        AC.heat     = AC_NO_PROTECTION;
        AC.magic    = AC_NO_PROTECTION;
        AC.mental   = AC_NO_PROTECTION;
        AC.piercing = AC_NO_PROTECTION;
        AC.slashing = AC_NO_PROTECTION;

		// set my master to no one
		myMaster = NULL;

		// set our object type
		setObjectType(AOBJECT_CREATURE);

    }
    
	// destructor.
	~aCreature(void) {

		// free memory from body parts
		bpList.freeList();

	}

	aCreatureRaceClass getRaceName(void) { return creatureRace.getRaceName(); }

    HPtype getMaxHP(void) { return maxHP; }
    HPtype getCurrentHP(void) { return currentHP; }

    MPtype getMaxMP(void) { return maxMP; }
    MPtype getCurrentMP(void) { return currentMP; }
    
	void setClient(aClient * newClient) {  myMaster = newClient; }

	// return a pointer to the client who's controlling this creatue. Could be NULL!
	aClient * getClient(void) { return myMaster; }

    // return armour class value for a particular AC type
    unsigned char getAC(enum armourClassType acType) {
        switch (acType) {
        case ARM_ACID:
            return AC.acid;
            break;
        case ARM_BASHING:
            return AC.bashing;
            break;
        case ARM_COLD:
            return AC.cold;
            break;
        case ARM_CRUSHING:
            return AC.crushing;
            break;
        case ARM_ELECTRIC:
            return AC.electric;
            break;
        case ARM_GAS:
            return AC.gas;
            break;
        case ARM_HEAT:
            return AC.heat;
            break;
        case ARM_MAGIC:
            return AC.magic;
            break;
        case ARM_MENTAL:
            return AC.mental;
            break;
        case ARM_PIERCING:
            return AC.piercing;
            break;
        case ARM_SLASHING:
            return AC.slashing;
            break;
        default:
            return 0;
            break;
        }
    }


    enum sphereType getSphere(void) { return playerSphere; }

	// event handler for this creature. may be overridden
	// for list of event types, see eventengine.h
	int eventHandler(aEventClass * event);


private: 
    HPtype maxHP;                   // total hit points based on sum of parts
    HPtype currentHP;               // current hit points                      

    armourClasses AC;               // armour class structure                   

    // MP = Move Points. arbitrary point system used like hit points to simulate
    //  getting tired. When they are active, MP goes down, when resting (or through
    //   other means), MP goes back up                                           
    MPtype maxMP;            	    // max amount of arbitrary "points" a
									// creature can move.                              
    MPtype currentMP;        	    // current move points                             

    enum sphereType playerSphere;   // the creatures sphere/alignment           

    aCreatureRaceClass creatureRace;	// the creatures race                       
    aCreatureSkills  playerSkills;      //  the creatures skills                     

	aClient * myMaster;				    // who is controlling this creature? 

	Hash<aCreatureBodyPart> bpList;	// list of body parts

};

#endif

