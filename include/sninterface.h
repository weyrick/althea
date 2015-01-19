/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header filef or SNinterface "simple network" interface

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include <iostream.h>
#include "String.h"
#include "listhash.h"
#include "defs.h"
#include "iointerface.h"
#include "server.h"
#include "snclient.h"
#include "worldengine.h"

#ifndef ASNINTERFACE_H
#define ASNINTERFACE_H

/* LOCAL DEFINES */
#define INTERFACE_ID			"SIMPLENET"		// interface name
#define INTERFACE_VERSION		"1.0"			// interface version
#define INTERFACE_PORT			1111			// interface port

class SNinterface : public aIOinterface {
	
public:
	// setup my interface ID
	SNinterface(void) : aIOinterface(INTERFACE_ID, INTERFACE_VERSION) { 
		
		Log.logDebug( "starting interface: " + ifaceID + " version " + ifaceVersion);
		
		server = new Server(ifaceID);
		server->boot(INTERFACE_PORT);
		
	}

	// cleanup
	~SNinterface(void) {
		clientList.freeList();
	}

	// check for new connections.. overrides pure virtual function from IOinterface
	bool checkForNewConnects(void);

    // maintain our client list
    bool maintainClients(void);
	
private:
	
	Server * server;			// the server for this instance
	Hash<SNclient> clientList;	// the local client list
	
};

#endif


