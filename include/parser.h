/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		header file for aCommandParser and aCommand.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include "String.h"
#include "aobject.h"
#include "events.h"
#include "eventengine.h"

#ifndef APARSER_H
#define APARSER_H

/* LOCAL DEFINES */
#define MAX_PARSE_PARAMS		20     // maximum number of paramters that will ever be passed (or parsed, at least)
#define APARSER_COMMAND_UNKNOWN	"Sorry, I didn't understand your command."

//	aCommand can have commands below it. each command can have help. any one aCommand can
//	have multiple commands below it in it's list, but only ONE required paramter, the rest must be optional
class aCommand {
public:
	aCommand(String cmd, String h="No Help For This Item", bool req=false) {
		itsCommand = cmd;
		itsHelp = itsCommand + " - " + h;
		required = req;
		hasReq = false;
		reqCommand = NULL;
		itsEventType = AEVENT_NULL;
	}

	// cleanup
	~aCommand(void) {
		aliasList.freeList();
		paramList.freeList();
	}

	// get the command
	String getCommand(void) {
		return itsCommand;
	}

	// number of required paramters
	bool hasRequired(void) {
		return hasReq;
	}

	// return a pointer to the command below us that is required 
	aCommand * getReqParam(void) {
		return reqCommand;
	}

	// set an event type
	void setEventType(aEventType newType) {
		itsEventType = newType;
	}

	// retrieve an event type
	aEventType getEventType(void) {
		return itsEventType;
	}

	// set the help string for this command 
	void setHelp(String h) {
		itsHelp = itsCommand + " \t" + h;
	}

	// return the help string for this command
	String getHelp(void) {
		return itsHelp + "\n\t" + getAliasHelp();
	}

	// add an alias for this command
	void addAlias(String alias) {
		String * newA = new String(alias);
		aliasList.addItem(newA, alias);
	}

	// see if the passed command is one of our aliases 
	bool isAlias(String cmd) {
		if (aliasList.findItem(cmd))
			return true;
		else
			return false;
	}

	// add a paramter to a command
	void addParam(String param, String help, bool req) {
		if ((req)&&(hasReq)) {
			// CANT DO THIS -- MAKE PROGRAM FAIL
			Log.logError("Command Parser was give a command with two required commands below it: "+param);
		}
		aCommand * newParam = new aCommand(param, help);
		if (!newParam) {
			Log.logError("aCommand::addParam: couldn't get memory for new parameter");
			return;
		}
		if (paramList.addItem(newParam, param))
			Log.logError("aCommand::addParam: couldn't add new parameter to paramList");
		if (req) {
			hasReq = true;
			reqCommand = newParam;
		}
	}

	// return a string list of help for the parameters for this command */
	String getParamHelp(void) {

		String help = "\nList Of Parameters:\n";

		paramList.resetCounter();
		aCommand * tCommand = paramList.current();
		while (tCommand) {
			help += "\t"+	tCommand->getHelp();
			tCommand = paramList.next();
		}

		return help;

	}

	// return a comma delimited list of aliases for this command
	String getAliasHelp(void) {

		String help = "no aliases";

		aliasList.resetCounter();
		String * tString = aliasList.current();
		if (tString) {
			help = "available aliases: ";
		}
		while (tString) {
			help += *tString + ",";
			tString = aliasList.next();
		}

		return help.before(help.length());

	}

private:
	String itsCommand;					// command we're defining
	String itsHelp;						// help for the command we're defining
	bool required;						// is this command/param required?
	bool hasReq;  						// does this command have a required command below it?
	aCommand * reqCommand;				// a convieniece pointer to our required command. NULL if we never set one up
	Hash<String> aliasList;	// list of aliases for this command
	Hash<aCommand> paramList; // parameter list 
	aEventType	itsEventType;			// our event type, if any

};
	
class aCommandParser {
public:
	aCommandParser(String sourceObjID, String command) {

		itsSrcObjID = sourceObjID;
		itsCommand = command;

		// build up our command list
		buildCommands();

	}

	~aCommandParser(void) {
		commandList.freeList();
	}

	// parse the command and throw an event
	String parseCommand(void);

private:
	// build a list of commands we recognize, including help, etc
	void buildCommands(void);

	String itsSrcObjID;	  	// unique ID of the object who gave us the command
	String itsCommand;		// the command we're processing

	Hash<aCommand> commandList;	// list of commands we recognize

};

#endif

