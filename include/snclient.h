/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for the SNclient "simple network" interface

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include "client.h"
#include "server.h"
#include "creature.h"
#include "defs.h"


#ifndef ASNCLIENT_H
#define ASNCLIENT_H
				
/* LOCAL DEFINES */
#define SNCLIENT_OUT_BUFFER_SIZE    2048        // max output buffer for a client
#define SNCLIENT_IN_BUFFER_SIZE     2048        // max input buffer for a client


class SNclient : public aClient {

public:
	SNclient(Server * serv, Socket * sock)
	{
		mySocket = sock;
		myServer = serv;
		myHash = (String)mySocket->getDescriptor();
		setControlState(CLIENT_CONTROL_HUMAN);
		inputNewline = false;
	}

	// cleanup this client upon deletion
	~SNclient(void) {

		// server shouldn't be deleted since it was passed to us
		// we don't need the socket anymore, however
		delete mySocket;

		// FIXME
		// if my creature still exists, remove from world and free it
		// this will, in the future, make the creatue an NPC rather than removing it
		if (myCreature) {
			worldEngine.removeObject(myCreature);
			Log.logWarning("SNclient::~SNclient: upon deletion of client, freeing associated myCreature object (FIXME: make NPC!)");
		}

	}

	// get / set network state
	unsigned int getNetState(void) { return networkState; }
	void setNetState(unsigned int newState) { networkState = newState; }

	// get hostname of client
	String getHostName(void) {if (mySocket) return mySocket->getHostName(); else return ""; }

	// return the string this client should be hashed by
	String getHash(void) { return myHash;}

	// if the last input from this client was a newline, this will return true
	bool isInputNewline(void) { return inputNewline; }

	// resets the inputNewLine flag, which determines if the last input the client entered was a new line
	void clearInputNewline(void) { inputNewline = false; }

	// whether client should ECHO or not (doesn't work?)
	void setEcho(bool val) { (val) ? mySocket->wontEcho() : mySocket->willEcho(); }

	// return the entire input buffer
	String getInBuffer(void) { return inBuffer;}

	// returns true if the user has input waiting
	bool hasInput(void) { return (inBuffer.length()) ? true : false; }

	// return the descriptor associated with this client
	int getDescriptor(void) 
	{ 
		if (mySocket)
			return mySocket->getDescriptor();
		else
			return -1; 
	}

	// disconnect the client
	void disconnect(void) {
		setNetState(NETSTATE_DISCONNECT);
		if (mySocket) {
			myServer->remove(mySocket);
			mySocket->close();
			mySocket = NULL;
		}
	}

	// write a line to the clients output buffer, adding new lines. output written to socket immediately
	void writeln(String text) {
		outBuffer = outBuffer + text + "\r\n";
		if (outBuffer.length() > SNCLIENT_OUT_BUFFER_SIZE)
			outBuffer = outBuffer.before(SNCLIENT_OUT_BUFFER_SIZE);
		flushOut();
	} 

	// write an unparsed bit of text to the client. output gets written to socket immediately
	void say(String text) {
		outBuffer = outBuffer + text;
		if (outBuffer.length() > SNCLIENT_OUT_BUFFER_SIZE)
			outBuffer = outBuffer.before(SNCLIENT_OUT_BUFFER_SIZE);
		flushOut();
	}

	// send output buffer over network
	void flushOut(void) {
		if (!mySocket)
			return;
		if (outBuffer.length()) {
			mySocket->write(outBuffer.getCstr());
		}
		outBuffer.clear();
	}

	// flush input buffer
	void flushIn(void) { inBuffer.clear();}

	// flush in/out buffer
	void flush(void) {
		inBuffer.clear();
		if (!mySocket)
			return;
		if (outBuffer.length()) {
			mySocket->write(outBuffer.getCstr());
		}
		outBuffer.clear();
	}

	// check for input from network, fill buffer. also check for disconnect
	bool getInput(void);

	// get/set the player name/password
	String getPlayerName(void) { return playerName; }
	void setPlayerName(String name) { playerName = name; }
	String getPassword(void) { return passWord; }
	void setPassWord(String pw) { passWord = pw; }

	// set the creature that this client will control
	void setCreature(aCreature * c) { myCreature = c; }

	// get the creature this client is controlling
	aCreature * getCreature(void) { return myCreature; }

	// inherited from client, refreshes the screen
    void refreshScreen(void);

	// inherited from client -- send world message to client
	void worldMessage(String message) {
		writeln("\n"+message);
		if (networkState == NETSTATE_PLAYING)
			refreshScreen();
	}

private:

	/* NETWORK  */
	String inBuffer;		 		// clients input buffer 
	String outBuffer;		 		// clients output buffer
	Socket * mySocket;	  			// socket the client is connected to 
	Server * myServer;    			// the server that's handling us	
	unsigned int networkState;   	// what network state is this client in?


	/* OBJECT */
	aCreature * myCreature;   			// the unique ID of the creature object we are controlling
	String playerName;				// the players name
	String passWord;				// the players password 

	/* MISC */
	String myHash;					// key that will be used to hash this client
	bool inputNewline;   			// if they just pressed enter at the prompt

};

#endif
