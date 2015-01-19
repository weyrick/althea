/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aIOinterface. a single I/O interface, of which IOEngineClass keeps a list. should be extended.

	CHANGE HISTORY:
	


 */

	 
/* LOCAL INCLUDES */						 
#include "String.h"
#include "universeTime.h"

#ifndef AIOINTERFACE_H
#define AIOINTERFACE_H

// a single interface for the I/O engine. should be subclassed
class aIOinterface {
public:
	aIOinterface(String i, String v)
	{
		//  setup the interfaces ID
		ifaceID = i;
		ifaceVersion = v;
	}

    // virtual destructor
    virtual ~aIOinterface(void) { /* nothing */; }

	String getID(void) { return ifaceID; }
	String getVersion(void) { return ifaceVersion; }
	
	// virtual function to check for new connections
	virtual bool checkForNewConnects(void) = 0;
	
    // virtual function to maintain an interfaces client list
    virtual bool maintainClients(void) = 0;

protected:
	String ifaceID;				// interface ID
	String ifaceVersion; 	// interface version
};


#endif

