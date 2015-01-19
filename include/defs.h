/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		global definitions

	CHANGE HISTORY:
	


 */

#ifndef ADEFS_H
#define ADEFS_H
				 
/* -=-=-=-=-=-=-=-=-=-=-=-= PROGRAM DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
             
#define PROJECT_NAME		"ALTHEA"    // project name           		   
#define PROJECT_VERSION		".01"          // code version           	    
     
/* -=-=-=-=-=-=-=-=-=-=-=-= END PROGRAM DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* -=-=-=-=-=-=-=-=-=-=-=-= DATABASE  DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#define DBNAME		"althea"
#define DBUSER		"weyrick"
#define DBPASS		"b1gf1sh"

/* -=-=-=-=-=-=-=-=-=-=-=-= END DATABASE DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* -=-=-=-=-=-=-=-=-=-=-=-= WORLD OBJECT  DEFINES -=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// world object types 
enum aWorldObjectType { AOBJECT_UNKNOWN, AOBJECT_CREATURE, AOBJECT_CREATURE_BODYPART };

// cell space fill type 
enum aWorldObjectSpaceType { SPACE_CUBIC, SPACE_FLATWIDTH, SPACE_FLATLENGTH };

// world timestamp type 
typedef unsigned long timeStampType;

/** CREATURE DEFINES **/

// spheres / alignments 
// white  - lawful good, divine - creation      
// yellow - lawful neutral - elemental/weather  
// orange - lawful evil - evocation             
// blue   - neutral good - healing              
// green  - true neutral - plant/animal         
// red    - neutral evil - killing, combat      
// brown  - chaotic good - knowledge, lerning   
// purple - chaotic neutral - alteration        
// black  - chaotic evil - necromatic, satanic  
enum sphereType { SPHERE_WHITE, SPHERE_YELLOW, SPHERE_ORANGE, SPHERE_BLUE, SPHERE_GREEN, SPHERE_RED, SPHERE_BROWN, SPHERE_PURPLE, SPHERE_BLACK };

// attributes 
enum attributeType { ATR_STRENGTH, ATR_INTELLIGENCE, ATR_WISDOM, ATR_DEXTERITY, ATR_CONSTITUTION, ATR_CHARISMA };

// skill categories 
enum skillCategoryType { SKILL_COMBAT, SKILL_MOVEMENT, SKILL_MENTAL,SKILL_ROGUE, SKILL_HEALING, SKILL_GENERAL };

// body part types 
enum aBodyPartType { ABODYPART_ARM,  ABODYPART_LEG, ABODYPART_TORSO,  ABODYPART_HEAD,  ABODYPART_WING };

// level type 
typedef unsigned char creatureLevelType;

// hit points type 
typedef unsigned int HPtype;

// move points type 
typedef unsigned int MPtype;

/** END CREATURE DEFINES **/

/*** ARMOUR **/

// armour classes. each have a valuce of 0-255, higher is more effective 
struct armourClasses {
    unsigned char piercing;
    unsigned char bashing;
    unsigned char slashing;
    unsigned char magic;
    unsigned char heat;
    unsigned char cold;
    unsigned char electric;
    unsigned char acid;
    unsigned char mental;
    unsigned char gas;
    unsigned char crushing;
};

// armour class types 
enum armourClassType { ARM_PIERCING, ARM_BASHING, ARM_SLASHING, ARM_MAGIC, ARM_HEAT, ARM_COLD, ARM_ELECTRIC,  ARM_ACID, ARM_MENTAL, ARM_GAS, ARM_CRUSHING };

// armour class defines 
#define AC_NO_PROTECTION        10

/** END ARMOUR **/

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- END CREATURE DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- EVENT DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// used to define what an event has occured 
typedef unsigned int aEventType;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- END EVENT DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- NETWORK DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// network states 
#define	NETSTATE_NEW_CONNECT        1       // just connected, show MOTD, ask for username  
#define NETSTATE_GETTING_PASSWORD   2       // got username, get password                   
#define NETSTATE_MAIN_MENU          3       // waiting at main menu                         
#define NETSTATE_NEW_CHAR           4       // creating a new character                     
#define NETSTATE_PLAYING            5       // playing in the world                         
#define NETSTATE_DISCONNECT			6       // player is/needs to be disconnected           
#define NETSTATE_GETTING_PLAYERNAME	7       // sent motd, getting playername                

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- END NETWORK DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- TIME DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// universe time definitions
#define SECONDS_PER_MINUTE	100
#define MINUTES_PER_HOUR	100
#define HOURS_PER_DAY		10
#define DAYS_PER_WEEK		5
#define DAYS_PER_MONTH		10
#define MONTHS_PER_SEASON	2
#define MONTHS_PER_YEAR		10
#define WEEKS_PER_MONTH		(DAYS_PER_MONTH / DAYS_PER_WEEK)
#define WEEKS_PER_YEAR		(WEEKS_PER_MONTH * MONTHS_PER_YEAR)
#define DAYS_PER_YEAR		(DAYS_PER_WEEK * WEEKS_PER_YEAR)
#define SEASONS_PER_YEAR	(MONTHS_PER_YEAR / MONTHS_PER_SEASON)
#define DAYS_PER_SEASON		(DAYS_PER_MONTH * MONTHS_PER_SEASON)
#define FORTNIGHT			(DAYS_PER_WEEK * 2)  /* fornight is two weeks */

// how many microseconds should pass before pulsing time
#define PULSE_INTERVAL		1000000		// 1,000,000 (1 million) is 1 second

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- END TIME DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- MATERIAL DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

// used to define what an event has occured 
typedef unsigned char aMaterialFlagType;

/* -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- END MATERIAL DEFINES =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#endif

