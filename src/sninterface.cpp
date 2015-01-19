/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		implements SNinterface class. code for a "simple network" interface (ie, telnet). this is the interface portion.

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include "defs.h"
#include "snclient.h"
#include "dbengine.h"
#include "sninterface.h"
#include "eventengine.h"
#include "parser.h"


/* IMPLEMENTATION */

// check for new connections.. overridden from base class
bool SNinterface::checkForNewConnects(void) {

	// sanity check
	if (!server) {
		Log.logError("SNinterface::checkForNewConnects: server == NULL");
		return false;
	}

	// Check for new connections
	if (server->newConnection()) {

		Socket * sock = server->accept();
		
		if (sock) {
			sock->nonBlock();

			// create a new client
			SNclient * newClient = new SNclient(server, sock);     

			if (!newClient)  {
				Log.logError("SNinterface::checkForNewConnects: unable to create new SNclient object");
				return false;
			}

			// add him to our client list
			if (clientList.addItem(newClient, newClient->getHash())) {
				Log.logError("SNinterface::checkForNewConnects: unable to add new client to our local client list");
				return false;
			}

			// set his state to let us know he's a new connection
			newClient->setNetState(NETSTATE_NEW_CONNECT);

			// succesfull connection
			Log.logDebug(ifaceID + ": new connection from: " + sock->getHostName());

		}
		else {
			// woops, couldn't get a socket
			Log.logError("SNinterface::checkForNewConnects couldn't get a socket for new connection");
			return false;
		}
	}

	// everything went ok
	return true;
}

// maintain our client list
bool SNinterface::maintainClients(void)
{


	// poll all active descriptors
	server->poll();

	SNclient * tClient = 0;
	SNclient * bClient = 0;

	clientList.resetCounter();
	tClient = clientList.current();

	// loop through my client list
	while (tClient) {

		// get input from client 
		tClient->getInput();

		// handle a client based on his current state
		switch (tClient->getNetState()) {
		/**************/
		/* DISCONNECT  */
		/**************/
		case NETSTATE_DISCONNECT:
			Log.logDebug(ifaceID+ ": CLIENT DISCONNECT");
			bClient = tClient;
			tClient = clientList.next();
			int eCode;
			if ( (eCode = clientList.removeItem(bClient->getHash()))) {
				Log.logError(ifaceID+": could not remove client upon NETSTATE_DISCONNECT");
			}
			continue;
		/***************/
		/* NEW CONNECT */
		/***************/
		case NETSTATE_NEW_CONNECT:
			// they just connected, show MOTD, ask for a user name
			tClient->say(dbEngine.getMessage("MOTD"));
			tClient->say(dbEngine.getMessage("PLAYERNAME_PROMPT"));
			// change them to need password state
			tClient->setNetState(NETSTATE_GETTING_PLAYERNAME);
			break;
		/******************/
		/* GET PLAYER NAME  */
		/******************/
		case NETSTATE_GETTING_PLAYERNAME:
			// waiting for playername 
			// have they sent anything?
			if (tClient->hasInput()) {
				// yep, assume it's their playername
				tClient->setPlayerName(tClient->getInBuffer());

				// lookup playername in database
				aCreature * newPlayer;
				// FIXME hack
				if ((newPlayer = new aCreature(SPHERE_BLACK, 0, 0)) != NULL) {
				//if ((newPlayer = dbEngine.getPlayer(tClient->getInBuffer())) != NULL) {
					// set our client as the controller of their character
					newPlayer->setClient(tClient);
					newPlayer->setName(tClient->getPlayerName()); // the objects name
					tClient->setCreature(newPlayer);
					tClient->setNetState(NETSTATE_GETTING_PASSWORD);
					tClient->say(dbEngine.getMessage("PASSWORD_PROMPT"));
					Log.logDebug("player attempting to login: " +  tClient->getPlayerName());
				}
				else {
					// new user
					Log.logDebug("SNinterface::maintainClients() - dbEngine.getPlayer() didn't give us a player, so must be new...");
					tClient->setNetState(NETSTATE_NEW_CHAR); 
				}
			}
			break;
		/****************/
		/* GET PASSWORD */
		/***************/
		case NETSTATE_GETTING_PASSWORD:
			// waiting for password
			// have they sent anything?
			if (tClient->hasInput()) {
				// yep, assume it's their password
				tClient->setPassWord(tClient->getInBuffer());
				// checkPassword;
				tClient->setNetState(NETSTATE_MAIN_MENU);
				tClient->say(dbEngine.getMessage("MAIN_MENU"));
			}
			break;
		/*****************/
		/* NEW CHARACTER */
		/*****************/
		case NETSTATE_NEW_CHAR:
			Log.logDebug("new player, going into new character mode");
			break;
		/************/
		/* MAIN MENU */
		/************/
		case NETSTATE_MAIN_MENU:
			if (tClient->hasInput()) {
				// has input in main menu, process 
				String option = tClient->getInBuffer();
				/* 1 == view character */
				/* 2 == enter world */
				if (option == "2") {
					tClient->writeln("Welcome to the world of Althea");  // prompt
					tClient->setNetState(NETSTATE_PLAYING);
					// add them to the world
					if (worldEngine.addObject(tClient->getCreature())) {
						Log.logError("SNinterface::maintainClients() -- unable to add new player object to worldEngine");
					}
					tClient->refreshScreen();
					break;
				}
				/* 3 == quit game */
				if (option == "3") {
					tClient->say(dbEngine.getMessage("QUIT_MESSAGE"));
					tClient->disconnect();
				}
			}
			else if (tClient->isInputNewline()) {
				// pressed enter
				tClient->say(dbEngine.getMessage("MAIN_MENU"));
				tClient->clearInputNewline();
			}
			break;
		/***********/
		/* IN WORLD */
		/***********/
		case NETSTATE_PLAYING:
			if (tClient->hasInput()) {
				aCommandParser parser(tClient->getCreature()->getID(), tClient->getInBuffer());
				String returnString = parser.parseCommand();
				if (returnString != "") {
					tClient->writeln(returnString);
					tClient->refreshScreen();
				}
			}
			else if (tClient->isInputNewline()) {
				// they pressed enter
				tClient->refreshScreen();
				tClient->clearInputNewline();
			}
			break;
		default:
			break;
		}

		// does this client need to be disconnected? (ie, did they issue QUIT?)
		if (tClient->getNetState() == NETSTATE_DISCONNECT) {
			Log.logDebug(ifaceID+ ": CLIENT DISCONNECT");
			bClient = tClient;
			tClient = clientList.next();
			if (clientList.removeItem(bClient->getHash())) {
				Log.logError("SNinterface::maintainClients() -- unable to remove client from clientList upon disconnect");
			}
			continue;
		}

		// send pending output 
		tClient->flushOut();
		// clear old input
		tClient->flushIn();

		tClient = clientList.next();
	}

	return true;

}

