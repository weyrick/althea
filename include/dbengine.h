/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		dbEngineClass header file

	CHANGE HISTORY:
	


 */

/* LOCAL DEFINES */
class dbEngineClass;

/* LOCAL INCLUDES */
#include <mysql/mysql.h>
#include <stdio.h>
#include "defs.h"
#include "String.h"

#ifndef ADBENGINE_H
#define ADBENGINE_H

class dbEngineClass {

public:

	dbEngineClass(void);
	~dbEngineClass(void);

    String getMessage(String mType);

private:
	MYSQL_RES * executeSQL(String SQL);		// private function to execute a query
	MYSQL * mysql;  						// pointer to the server


};

// global database engine instantiated in main.cpp
extern dbEngineClass dbEngine;

#endif

