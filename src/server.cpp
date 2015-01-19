/*
....Copyright(C).1995.Melvin.Smith.............. Enjoy. 
server.cc
*/

#include <signal.h>
#include "server.h"
#include "defs.h"

Server::Server(String iFaceName) : masterport(4000), masterdesc(0), type( SOCK_STREAM ), nameserver(0), maxread( MAX_READ ), maxidle(0), descripts(0)
{
	maxdesc = sysconf( _SC_OPEN_MAX );
	interFace = iFaceName;
}

Server::Server( int theport, String iFaceName ) : masterport( theport ), masterdesc(0), type( SOCK_STREAM ), nameserver(0), maxread( MAX_READ ), maxidle(0), descripts(0)
{
	maxdesc = sysconf( _SC_OPEN_MAX );
	interFace = iFaceName;
}

Server::~Server()
{
	if( master )
		master->close();
}


// This is for booting "into" a previous servers process
// space and inheriting the open master descriptor. Skip
// all the master struct creation and just assign the
// descriptor to sock and clean up a few other things.
// This really needs some serious error handling. Also need
// to save the sock struct info in the reboot.tab file
// and recreate it from that.
// --Fusion

int Server::boot( int oldport, int olddesc )
{
	
	::signal( SIGPIPE, SIG_IGN );
	
	master = new Socket( "", oldport, olddesc ); 
	master->nonBlock();
	masterport = oldport;
	masterdesc = olddesc;
	
	Log.logDebug(interFace + ": Ok, this OS supports " + (String)maxdesc + " open descriptors per process");
	
	if( maxdesc > 256 )
		maxdesc = 256;
	bitwidth = maxdesc / 8;
	topdesc = olddesc;
	
	Log.logDebug(interFace + ": Rebooted succesfully - ready for connections");
	Log.logDebug(interFace + ": Port [" + (String)master->getPort() + "]");
	
	gettimeofday( &boot_time, 0 );
	
	return 0;
}


int Server::boot( int tport )
{
	
	::signal( SIGPIPE, SIG_IGN );
	
	master = new Socket( SOCK_STREAM, tport );
	if( master->error() )
	{
		perror( interFace + ": Server::boot() socket not created" );
		exit(0);
	}
	
	master->reuseAddr();
	master->noDelay();
	master->nonBlock();
	if( master->open() )
	{
		perror( interFace + ": Socket::open()" );
		exit(0);
	}
	
	masterdesc = topdesc = master->getDescriptor();
	masterport = master->getPort();
	
	Log.logDebug(interFace + ": Ok, this OS supports " + (String)maxdesc + " open descriptors per process");
	
	if( maxdesc > 256 )
		maxdesc = 256;
	bitwidth = maxdesc / 8;
	
	Log.logDebug(interFace + ": Rebooted succesfully - ready for connections");
	Log.logDebug(interFace + ": Port [" + (String)master->getPort() + "]");

	gettimeofday( &boot_time, 0 );
	
	return 0;
}


const timeval & Server::getUpTime()
{
	static timeval up;
	timeval now_time;
	
	gettimeofday( &now_time, 0 );
	
	up.tv_sec = now_time.tv_sec - boot_time.tv_sec;
	return up;
}


int Server::poll()
{
	memcpy( &R_SET, &MASTER_SET, bitwidth );
	memcpy( &W_SET, &MASTER_SET, bitwidth );
	memcpy( &E_SET, &MASTER_SET, bitwidth );
	FD_SET( masterdesc, &R_SET );
	// Linux modifies timeval * in select so reset every time.
	null_time.tv_sec = 0;
	null_time.tv_usec = 0;
	while( select( topdesc+1, &R_SET, &W_SET, &E_SET, &null_time ) < 0 )
	{
		if( errno == EINTR )
			continue;
		Cout << interFace << ": select() error in Poll() - exiting.\n";
		exit( errno );
	}
	return FD_ISSET( masterdesc, &R_SET );
}

// I high resolution sleep function
// Best to inline this to remove the overhead of the function call

void Server::sleep( long microsecs )
{
	static struct timeval sleep_time;
	sleep_time.tv_sec  = 0;
	sleep_time.tv_usec = microsecs; 
	while( select( 0, NULL, NULL, NULL, &sleep_time ) < 0 )
	{
		
		if( errno == EINTR )
			continue;
		Cout << interFace << ": Error select() in Sleep()\n";
		exit( errno );
		
	}
}


int Server::newConnection()
{
	FD_ZERO( &R_SET );
	FD_SET( masterdesc, &R_SET );
	// Linux modifies timeval * in select so reset every time.
	null_time.tv_sec  = 0;
	null_time.tv_usec = 0; 
	while( select( masterdesc + 1, &R_SET, 0, 0, &null_time ) < 0 )
	{
		if( errno == EINTR )
			continue;
		perror( interFace + ": select() in newConnection() - fatal" );
		exit( errno );
	}
	return FD_ISSET( masterdesc, &R_SET );
}


Socket * Server::accept()
{
	int desc;
	Socket *newSock = master->accept();
	
	if( !newSock )
	{
		if( errno == EAGAIN || errno == EWOULDBLOCK )
			return 0;
		return 0;
	}
	
	newSock->noDelay();
	newSock->getPeerName();
	newSock->resolveIP();
	desc = newSock->getDescriptor();
	
	if( desc > topdesc )
		topdesc = desc;
	
	FD_SET( desc, &MASTER_SET );
	descripts++;
	
	
	Log.logDebug(interFace + ": accept, new desc = " + (String)desc);

	return newSock;
}


void Server::addSock( Socket * sock )
{
	if( FD_ISSET( sock->getDescriptor(), &MASTER_SET ) )
	{
		Cout << interFace << ": Error. Attempt to add existing descriptor.\n";
		exit(0);
	}
	
	FD_SET( sock->getDescriptor(), &MASTER_SET );
	if( sock->getDescriptor() > topdesc )
		topdesc = sock->getDescriptor();
	descripts++;
}


void Server::remove( Socket * sock )
{
	// I exit on the following two errors because if this happens
	// you need to find out why.
	
	int desc = sock->getDescriptor();
	if( desc < 0 || desc >= maxdesc ) 
	{
		Cout << interFace << ": Close() - desc [" << desc << "] out of bounds.\n";
		abort();
	}
	
	if( ! FD_ISSET( desc, &MASTER_SET ) )
	{
		Cout << interFace << ": Invalid descriptor\n";
		abort();
	}
	
	FD_CLR( desc, &MASTER_SET );
	
	if( !--descripts )
	{
		topdesc = masterdesc;
		return;
	}
	
	if( desc == topdesc )
	{
		for( desc = topdesc-1; !FD_ISSET( desc, &MASTER_SET ); desc-- )
			;
		topdesc = desc;
	}
}


int Server::error( Socket * sock )
{
	int desc = sock->getDescriptor();
	
	if( FD_ISSET( desc, &E_SET ) )
	{
		FD_CLR( desc, &R_SET );
		FD_CLR( desc, &W_SET );
		return FD_ISSET( desc, &E_SET );
	}
	
	return 0;
}
