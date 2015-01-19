/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aCreatureRaceClass. used to implement races in creatures.

	CHANGE HISTORY:
	


 */

#ifndef ARACES_H
#define ARACES_H

/* LOCAL INCLUDES */
#include "String.h"

class aCreatureRaceClass  {
public:
	aCreatureRaceClass(void) {
		raceName = "Unknown";
	}

	aCreatureRaceClass(String name) {
		raceName = name;
	}

	String getRaceName(void) {
		return raceName;
	}

private:
	String raceName;

};

#endif
