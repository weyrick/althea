/*

....Copyright(C).1995.Melvin.Smith.............. Enjoy. 
server.h
*/

#include "socket.h"
#include "defs.h"
#include "sys/time.h"
#include "unistd.h"

#include "String.h"

#ifndef SERVER_H
#define SERVER_H

// global log class
#include "log.h"

#define NULL_CHAR '\0'
#ifndef NULL
#define NULL 0
#endif

#ifndef FNDELAY
#define FNDELAY O_NONBLOCK
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK  EAGAIN 
#endif


#define SOFT_MAX_DESC  256  // Fuzzy _POSIX_OPEN_MAX, only used for array size
#define MAX_READ       256
#define MAX_WRITE      4096 // Dont write more than this in a call


class Server
{
private:
	Socket *	master;
	int			masterport;
	int			masterdesc;
	int			type;
	bool		nameserver;
	int			maxdesc;
	int			maxread;
	int			maxidle;
	int			descripts;
	int			topdesc;
	int			bitwidth;
	timeval		null_time;
	fd_set		MASTER_SET;
	fd_set		R_SET;
	fd_set		W_SET;
	fd_set		E_SET;
	timeval		boot_time;
	String		interFace;
	
public:
	Server(String iFaceName);
	Server( int, String iFaceName );
	~Server();
	
	Socket * getSocket() { return master; }
	int getPort() { return masterport; }
	int getDescriptor() { return masterdesc; }
	int boot( int tport = 4000 ); // boot on a specific port
	int boot( int, int ); // reboot ( port and TCP desc )
	void close() { master->close(); }
	int newConnection();
	void addSock( Socket * );  // Add a previously opened desc.
	Socket * accept();
	int poll();
	void sleep( long millisecs ); 
	void useNameServer() { nameserver = true; }
	void remove( Socket * );
	int error( Socket * );
	bool canWrite( Socket * );
	bool canRead( Socket * );
	const timeval & getUpTime();
	bool setmaxdesc( int i );
};

inline bool Server::canRead( Socket * s )
{
	return (bool)FD_ISSET( (int)s->getDescriptor(), &R_SET );
}

inline bool Server::canWrite( Socket * s )
{
	return (bool)FD_ISSET( (int)s->getDescriptor(), &W_SET );
}

#endif
