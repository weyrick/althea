 /*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aClient code.
		
		a "client" is the "life force" of a creature. it could be a networked user controlling the creature, or it may be
		the computer controlling the client.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include "String.h"

#ifndef ACLIENT_H
#define ACLIENT_H
											
/* LOCAL DEFINES */
#define CLIENT_CONTROL_UNKNOWN			0	// undefined
#define CLIENT_CONTROL_COMPUTER			1   // computer is controlling this client
#define CLIENT_CONTROL_HUMAN			2   // human player is controlling this client

// a single client. should be subclassed
class aClient {
	
public:
	aClient(void) { controlState = CLIENT_CONTROL_UNKNOWN; }

	virtual ~aClient(void) { /* nothing */ }

	// set the control state of the client
	void setControlState(unsigned int newState) { controlState = newState; }

	// get the control state of the client (ie, who's controlling it)
	unsigned int getControlState(void) { return controlState; }

	// set the network state of the client (will be extended by human controllable clients)
	virtual void setNetState(unsigned int)  { /* nothing */ }

	// get the network state of the client (will be extended by human controllable clients)
	virtual unsigned int getNetState(void) { return 0; }

	// tells the client to update their screen in some fashion
	virtual void refreshScreen(void) { /* nothing */ }

	// send textual message to the client
	virtual void worldMessage(String message) { /* nothing */ }

private:
	unsigned int controlState;   // flag which tells us how this client is controlled
	
};

#endif

