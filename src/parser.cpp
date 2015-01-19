/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		aCommandParser implementation. used to parse input from clients into
		events.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include "parser.h"
					
/* IMPLEMENTATION */					
					
// parse a command, throw events
String aCommandParser::parseCommand(void) {

    aEventClass * newEvent;
	String params[MAX_PARSE_PARAMS];
	bool wantHelp = false;

	int numParams = split(itsCommand, params, MAX_PARSE_PARAMS);
	if (!numParams)
		return "How about typing something?";

	String command = params[0];

	// see if they are asking for help or not
	if (params[numParams-1] == "?")
		wantHelp = true;

	// now handle parameters
	// loop through our list of known commands
	commandList.resetCounter();
	aCommand * tCommand = commandList.current();

	// FIXME -- this needs to be recursive for multiple parameters
	while (tCommand) {

		// does the command the user gave us match the one we're checking, or one of it's aliases?
		if ((command == tCommand->getCommand()) || (tCommand->isAlias(command)) ) {

			// jackpot

			// do they want help, or are they issuing a command?

			if (wantHelp) {
				// they want help, so show help for this command and that's it 
				return  tCommand->getHelp() + tCommand->getParamHelp();
			}

			// first of all, does it have any required?
			if (tCommand->hasRequired()) {

				// yep, it has some required parameters, so did they give us any?
				if (numParams < 2)
					// nope, they didn't enter a required paramter, show them some help
					return tCommand->getReqParam()->getHelp();
				else {

					// ok, the command has a required paramter, and they've entered one
					// which could possibly be it.

					// create our event here 
					newEvent = new aEventClass(tCommand->getEventType(), itsSrcObjID);
					if (!newEvent) {
						Log.logError("aCommandParser::parseCommand: unable to create new event object");
						return "";
					}
					newEvent->addString(tCommand->getReqParam()->getCommand(), params[1]);
					eventEngine.throwEvent(newEvent);
					return "";

				}

			}
			else {

				// no, no required parameters, so execute action here
				newEvent = new aEventClass(tCommand->getEventType(), itsSrcObjID);
				if (!newEvent) {
					Log.logError("aCommandParser::parseCommand: unable to create new event object");
					return "";
				}
				eventEngine.throwEvent(newEvent);
				return "";

			}

		}

		// nope, move on and check the next
		tCommand = commandList.next();
	}

	return APARSER_COMMAND_UNKNOWN;

}


// define our list of commands
void aCommandParser::buildCommands(void) {

	aCommand *newCommand;

	// AEVENT_CREATURE_MOVE
	newCommand = new aCommand("move","Move the creature you are controlling in the direction specified");
	newCommand->setEventType(AEVENT_CREATURE_MOVE);
	newCommand->addParam(AEVENT_PARAM_DIRECTION, "The direction to move your creature in: N,S,E,W,U,D", true);
	newCommand->addAlias("walk");
	commandList.addItem(newCommand, newCommand->getCommand());

	// AEVENT_CREATURE_MOVE_N
	newCommand = new aCommand("north","Move the creature you are controlling north one cell");
	newCommand->setEventType(AEVENT_CREATURE_MOVE_N);
	newCommand->addAlias("n");
	commandList.addItem(newCommand, newCommand->getCommand());

	// AEVENT_CREATURE_MOVE_S
	newCommand = new aCommand("south","Move the creature you are controlling south one cell");
	newCommand->setEventType(AEVENT_CREATURE_MOVE_S);
	newCommand->addAlias("s");
	commandList.addItem(newCommand, newCommand->getCommand());

    // AEVENT_CREATURE_MOVE_E
	newCommand = new aCommand("east","Move the creature you are controlling east one cell");
	newCommand->setEventType(AEVENT_CREATURE_MOVE_E);
	newCommand->addAlias("e");
	commandList.addItem(newCommand, newCommand->getCommand());

	// AEVENT_CREATURE_MOVE_W
	newCommand = new aCommand("west","Move the creature you are controlling west one cell");
	newCommand->setEventType(AEVENT_CREATURE_MOVE_W);
	newCommand->addAlias("w");
	commandList.addItem(newCommand, newCommand->getCommand());

	// AEVENT_PLAYER_QUIT
	newCommand = new aCommand("quit","Quit the game");
	newCommand->setEventType(AEVENT_PLAYER_QUIT);
	newCommand->addAlias("exit");
	commandList.addItem(newCommand, newCommand->getCommand());

}

