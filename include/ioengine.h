/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for IOEngineClass. used to keep a list of I/O mechanisms, maintain them, etc.

	CHANGE HISTORY:
	


 */



/* LOCAL INCLUDES */
#include "String.h"
#include "listhash.h"

#ifndef AIOENGINE_H
#define AIOENGINE_H

/* IO INTERFACES */
#include "sninterface.h"

// main controlling class of all input/output functions for the universe
class IOEngineClass {
	
public:
	IOEngineClass(void) {
		// for now, add our only interface: the simple network interface
		SNinterface * snIO = new SNinterface();
		if (interfaceList.addItem(snIO, snIO->getID())) {
			Log.logError("IOEngineClass couldn't add interface");
		}
	}
	
	~IOEngineClass(void) {
		interfaceList.freeList();
	}

	// we're getting a time slice - make the best of it
	bool timeSlice(void) {
		
		aIOinterface * anInterface;	
		
		interfaceList.resetCounter();
		anInterface = interfaceList.current();
		while (anInterface) {
			
			// let each interface check for new connects
			anInterface->checkForNewConnects();

            // let each interface run it's think code
            if (!anInterface->maintainClients())
				Log.logWarning("IOEngineClass::timeSlice(): maintainClients returned false");
			
			anInterface = interfaceList.next();	
		}
		
		return true;
	}
	
private:
	Hash<aIOinterface> interfaceList;	 // list of interfaces
	
};


#endif

