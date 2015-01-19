/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for CreatueSkills and related classes. used to implement skills for creatures.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include "listhash.h"

#ifndef ASKILLS_H
#define ASKILLS_H

// a single skill
class aBaseSkill {

public:


	private:
	String skillName;						// skill name, must be UNIQUE
	enum skillCategoryType skillCategory;	// category this skill belongs to

	/* levels: 0-255. when created, a skill starts at its base level. before reaching
	   competantLevel, a chance  to succeed at a skill is based on skillLevel alone 
	   (a baseLevel of 5 would yield a base 5% chance of success (and could be modified 
	   by other conditions). when the level reaches compentantLevel (and above), 
	   skill checks are then (skillLevel + prime requsite). Once expert level is reached,
	   skill checks are made on (skillLevel + prime requisite) and derived skills can be
	   learned. if expertLevel is set higher than maxLevel, one can never become an expert */

	enum attributeType primeReq;	  	// prime requisite                                  
	unsigned char skillLevel;			// the current level of this skill                  
	unsigned char baseLevel;		    // default level at creation                        
	unsigned char competantLevel;	 	// level at which prime requisite is added
										//	  to checks for skill success                      
	unsigned char expertLevel;			// level at which a character can learn
										//   derived skills, and prime req bonus may double   
	unsigned char maxLevel;    			// the highest level that can be reached    

};


// skill class
class aCreatureSkills {

public:

	// cleanup
	~aCreatureSkills(void) {
		skillList.freeList();
	}

	// get the number of skills this creature has
	unsigned int getNumSkills(void) { return skillList.getNumItems(); }

private:
	Hash<aBaseSkill> skillList;    // the main list of all skils 

};

#endif

