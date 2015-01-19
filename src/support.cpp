/*

	project althea
	original code: weyrick
	
	copyright (c) 2000 Roadsend, Inc.


	DESCRIPTION:
		misc global support functions

	CHANGE HISTORY:
	


 */

/* LOCAL INCLUDES */
#include <unistd.h>
#include <sys/time.h>
#include <stdio.h>
#include <fstream.h>
#include "String.h"					  
#include "support.h"

// for getMemUsage           
#define MAX_MEM_LEN     1024
#define MAX_MEM_ARGS    7
					
/* IMPLEMENTATION */					
					  
// generate a unique string
String genID(void) {
        
		char uniqid[128];
        int sec, usec;
        struct timeval tv;
        usleep(1);
        gettimeofday((struct timeval *) &tv, (struct timezone *) NULL);
        sec = (int) tv.tv_sec;
        usec = (int) (tv.tv_usec % 1000000);
        /* The max value usec can have is 0xF423F, so we use only five hex
         * digits for usecs:
         */
        sprintf(uniqid, "%08x%05x", sec, usec);
		String uID(uniqid);
		return uID;

}

// string comparison helper function
int scmp(const char * a, const char * b)
{
  if (b == 0)
    return *a != 0;
  else {
    int diff = 0;
    while ((diff = *a - *b++) == 0 && *a++ != 0);
    return diff;
  }
}

// get memory usage
unsigned int getMemUsage(void) {

    pid_t   myPid = getpid();
    char buf[MAX_MEM_LEN];
    String splits[MAX_MEM_ARGS];
    unsigned int totalMem = 0;

    // Linux only
    String fn = "/proc/"+(String)myPid+"/statm";

	ifstream infile(fn);
    if (!infile) {
        cout << "getMemUsage: unable to open stat file for my pid: " << myPid << endl;
        exit(-1);
    }
    infile.getline(buf, MAX_MEM_LEN);
    infile.close();

    String sBuf(buf);
    split(sBuf, splits, MAX_MEM_ARGS);

    for (int x=0; x < MAX_MEM_ARGS; x++)
        totalMem += atoi(splits[x]);
    
    return totalMem;

}
