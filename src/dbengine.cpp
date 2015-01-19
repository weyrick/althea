/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		implementation for dbEngineClass. handles database access.

	CHANGE HISTORY:
	


 */


/* LOCAL INCLUDES */
#include "dbengine.h"
#include "log.h"

/* IMPLEMENTATION */					

// constructor
dbEngineClass::dbEngineClass(void) {

	if (!(mysql = mysql_init(NULL))) {
		Log.logError("couldn't initialize mysql pointer, fatal");
		exit(-1);
	}

}
					
// cleanup
dbEngineClass::~dbEngineClass(void) {

	mysql_close(mysql);

}

// lookup a player character, return NULL if not found
// this function will lookup a player in the database and build a new creature object with
// all the attributes it finds
/*
aCreature * dbEngineClass::getPlayer(String playerName) {

   	// FIXME but for right now, it fakes it 
   	aCreature * newPlayer = new aCreature(SPHERE_BLACK, 0, 0);
	if (!newPlayer) {
		Log.logError("dbEngineClass::getPlayer unable to create new player");
	}
   	return newPlayer;

}
*/

// return a message from the database
String dbEngineClass::getMessage(String mType) {

		MYSQL_RES * rs;

        // get message from database
        String SQL = "SELECT message FROM messages WHERE type='" + mType + "'";
        rs = executeSQL(SQL);
        if (!rs) {
                Log.logWarning("dbEngineClass::getMessage couldn't retrieve message of type: "+mType);
        }
        // grab stuff
        MYSQL_ROW messageRow = mysql_fetch_row(rs);
        
		String message(messageRow[0]);
        
        // free result
        mysql_free_result(rs);

		return message;

}

// execute query on database
MYSQL_RES * dbEngineClass::executeSQL(String SQL) {

    // connect to database
    if (!mysql_real_connect(mysql,"localhost",DBUSER,DBPASS,DBNAME,0,NULL,0)) {
        Log.logError("dbEngineClass::executeSQL couldn't connect to database server");
        Log.logError(mysql_error(mysql));
        exit(-1);
    }
    mysql_select_db(mysql, DBNAME);
    // execute query
    if (mysql_real_query(mysql, (const char *)SQL, SQL.length())) {
        Log.logError("dbEngineClass::executeSQL couldn't execute query: query failed");
        Log.logError(mysql_error(mysql));
        exit(-1);
    }
    // get result set
    MYSQL_RES * resultSet = mysql_store_result(mysql);
    return resultSet;

}

/*
// log a message to the database error table
void dbEngineClass::logMessage(String message, int level) {

	// FIXME log error to error table
	Log.logDebug("dbEngineClass::logMessage stub");

}

  */
