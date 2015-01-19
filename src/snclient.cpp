/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		implements SNclient class. code for a "simple network" interface (ie, telnet). this is the client portion.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES  */	
#include "snclient.h"
			   

/* IMPLEMENTATION */
			   
// check for input from network, fill buffer. also check for disconnect
bool SNclient::getInput(void) {

	if (!mySocket) {
		Log.logDebug("SNclient::getInput found a NULL mySocket, assuming disconnect");
		setNetState(NETSTATE_DISCONNECT);
		return false;
	}

	if (myServer->canRead(mySocket)) {

		inputNewline = false;

		char inBuf[SNCLIENT_IN_BUFFER_SIZE];
		memset(inBuf, '\0', SNCLIENT_IN_BUFFER_SIZE);
		mySocket->read(inBuf, SNCLIENT_IN_BUFFER_SIZE);
		if (inBuf) {
			inBuffer = inBuf;
			// strip whitespace
			inBuffer.trim();
			// if we have nothing in the buffer now, it was all newline, so set inputNewline to true
			if (inBuffer == "")
				inputNewline = true;
		}
		else
			inBuffer = NULL;
		if (mySocket->eof()) {
			Log.logDebug("SNclient::getInput: encountered EOF, assuming disconnect");
			inBuffer = NULL;
			disconnect();
			return false;
		}
	}

	if (myServer->error(mySocket)) {
		Log.logDebug("SNclient::getInput: encountered socket error, assuming disconnect");
		disconnect();
		return false;
	}

	return true;
}


// refresh the clients screen (something has changed)
void SNclient::refreshScreen(void) {
		
	// right now just updates coordinates
	say("\n<"+myCreature->getCoordText()+"| mem: "+(String)getMemUsage()+"k > ");

}


