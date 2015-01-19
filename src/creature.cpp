/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		implementation for aCreature class.

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include "creature.h"


/* IMPLEMENTATION */	 
	 
// event handler for this creature. may be overridden
// for list of event types, see eventengine.h
int aCreature::eventHandler(aEventClass * event) {
	
	bool moved; 	// use this to test if we succesfully completed a move event
	String dir;		// use this to see what direction they wish to move in

	if (!event) {
		Log.logError("aCreature::eventHandler received a NULL event");
		return EVENT_HANDLER_ERROR;
	}

	switch (event->getEventType()) {

	// ok, we're moving. the event should tell us which direction
	case AEVENT_CREATURE_MOVE:

		dir = event->getString(AEVENT_PARAM_DIRECTION);
		
		if ((dir == "n") || (dir == "no") || (dir == "north"))
			moved = worldEngine.moveObject(aWorldCoords(location).moveNorth(), getID());
		else if ((dir == "e") || (dir == "ea") || (dir == "east"))
			moved = worldEngine.moveObject(aWorldCoords(location).moveEast(), getID());
		else if ((dir == "s") || (dir == "so") || (dir == "south"))
			moved = worldEngine.moveObject(aWorldCoords(location).moveSouth(), getID());
		else if ((dir == "w") || (dir == "we") || (dir == "west"))
			moved = worldEngine.moveObject(aWorldCoords(location).moveWest(), getID());
		else {
			if (IS_PC)
				myMaster->worldMessage("I didn't recognize the direction you wanted to move in");
			return 1;
		}

		if ((IS_PC)&&(moved))
			myMaster->refreshScreen();

		break;

	// creature move north one cell
	case AEVENT_CREATURE_MOVE_N:

		// if this fails, a message event will be thrown as to why
		moved = worldEngine.moveObject(aWorldCoords(location).moveNorth(), getID());
		if ((IS_PC)&&(moved))
			myMaster->refreshScreen();

		break;

	// creature move south one cell
	case AEVENT_CREATURE_MOVE_S:

		// if this fails, a message event will be thrown as to why
		moved = worldEngine.moveObject(aWorldCoords(location).moveSouth(), getID());
		if ((IS_PC)&&(moved))
			myMaster->refreshScreen();

		break;

	// creature move east one cell
	case AEVENT_CREATURE_MOVE_E:

		// if this fails, a message event will be thrown as to why
		moved = worldEngine.moveObject(aWorldCoords(location).moveEast(), getID());
		if ((IS_PC)&&(moved))
			myMaster->refreshScreen();
				
		break;

	// creature move west one cell
	case AEVENT_CREATURE_MOVE_W:

		// if this fails, a message event will be thrown as to why
		moved = worldEngine.moveObject(aWorldCoords(location).moveWest(), getID());
		if ((IS_PC)&&(moved))
			myMaster->refreshScreen();

		break;

	// world message to client
	case AEVENT_CREATURE_MESSAGE:
		if (IS_PC)
			myMaster->worldMessage(event->getString("message"));
		break;

	// player wants to leave. send them to main menu.
	case AEVENT_PLAYER_QUIT:
		if (IS_PC) {
			myMaster->setNetState(NETSTATE_MAIN_MENU);
			myMaster->worldMessage(dbEngine.getMessage("MAIN_MENU"));
		}
		break;	

	default:  
		Log.logWarning("aCreature::eventHandler: unhandled event occured, type ["+(String)event->getEventType()+"]");
		break;

	}

	return EVENT_HANDLER_STATUS_OK;

}

