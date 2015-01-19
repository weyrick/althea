/*

....Copyright(C).1995.Melvin.Smith.............. Enjoy. 
socket.h
*/

#ifndef _SOCKET_H
#define _SOCKET_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <arpa/telnet.h>
#include <arpa/inet.h>
#include <netdb.h>


// You can remove io.h to use socket stuff standalone without io.h and io.cc
// It will try to use the standard C++ iostreams instead. You may have to
// add a -l or remove the -nostdinc++ to link your io lib or libstdc++.a
#include <iostream.h>
#define Cout cout   // support for standard C++ lib to work outside of MUD++

// This is the beginning of redesigning the Server and all IO classes
// The Socket class should be in the Stream class hierarchy but as
// I have the hierarchy now, it is not easy to insert Socket since
// Sockets can't be memory mapped so the MPPIStream class wont work with it.
// The Read(), Write() functions are the same as Server:: functions and
// will replace them when Socket class is done. The Server will then
// have a slightly different role.

/*
Structures describing an Internet socket address
from sys/in.h

INADDR_ANY is 0.0.0.0 which means accept all incoming
#define INADDR_ANY ((unsigned long int) 0x00000000)

struct in_addr
{
	__u32 s_addr;
};

struct sockaddr_in
{
	short int sin_family;          Address family
	unsigned short int sin_port;   Port number ( network byte order )
	struct in_addr sin_addr;       IP address

	unsigned char __pad[ XXX ];    Pads to size of sockaddr
};
*/

#ifndef FNDELAY
#define FNDELAY O_NDELAY
#endif

#ifndef EWOULDBLOCK
#define EWOULDBLOCK EAGAIN
#endif

#ifndef EINPROGRESS
#define EINPROGRESS EWOULDBLOCK
#endif

#define MAX_READ	256
#define MAX_WRITE	4096


// Telnet codes.
const char WILL_ECHO [] =
{
	(char)	IAC,
	(char)	WILL,
	(char)	TELOPT_ECHO,
	(char)	0
};

const char WONT_ECHO [] =
{
	(char)	IAC,
	(char)	WONT,
	(char)	TELOPT_ECHO,
	(char)	0
};

const char WILL_SUPPRESS_GA [] =
{
	(char)	IAC,
	(char)	WILL,
	(char)	TELOPT_SGA,
	(char)	0
};

const char WONT_SUPPRESS_GA [] =
{
	(char)	IAC,
	(char)	WONT,
	(char)	TELOPT_SGA,
	(char)	0
};

const char TELNET_GA [] =
{
	(char)	IAC,
	(char)	GA,
	(char)	0
};


// These codes aren't really accurate but it is enough for the class to work
#define SOCK_NOT_CREATED		1
#define SOCK_FCNTL_ERR			2
#define SOCK_UNKNOWN_HOST		3
#define SOCK_BAD_ADDRESS		4
#define SOCK_NOT_CONNECTED		5
#define SOCK_SOCKOPT_ERR		6
#define SOCK_EOF			    7	


class Descriptor
{
protected:
	int fd;
	bool eofd;
	
public:
	Descriptor()
	: fd(-1), eofd(false)
	{
	}
	
	Descriptor( int x )
	: fd(x), eofd(false)
	{
	}
	
	int operator = ( int x ) { return ( fd = x ); }
	operator int () { return fd; }
	bool isOpen() { if( fd >= 0 ) return true; return false; }
	int read( char *, int );
	int recvfrom( char *, int, int, struct sockaddr *, int * );
	int sendto( const char *, int, int, const struct sockaddr *, int );
	int write( const char *, int );
	int write( const char * buf ) { return write( buf, strlen( buf ) ); }
	int close()
	{ eofd = true; int i = fd; fd = -1; 
		
		return ::close( i ); 
		
	}
	
	bool eof() { return eofd; }
	bool canWrite();
	bool canRead();
	bool canWriteWait( long, long );
	bool canReadWait( long, long );
	int noDelay();
	int nonBlock();
};


inline bool Descriptor::canWrite()
{
	timeval tzero;
	tzero.tv_sec = 0;
	tzero.tv_usec = 0;
	fd_set W_SET;
	FD_ZERO( &W_SET );
	FD_SET( fd, &W_SET );
	while( select( fd+1, 0, &W_SET, 0, &tzero ) < 0 )
	{
		
		if( errno == EINTR )
			continue;
		return false;
		
	}
	return (bool)FD_ISSET( fd, &W_SET );
}


inline bool Descriptor::canRead()
{
	timeval tzero;
	tzero.tv_sec = 0;
	tzero.tv_usec = 0;
	fd_set R_SET;
	FD_ZERO( &R_SET );
	FD_SET( fd, &R_SET );
	while( select( fd+1, &R_SET, 0, 0, &tzero ) < 0 )
	{
		
		if( errno == EINTR )
			continue;
		return false;
		
	}
	
	return (bool)FD_ISSET( fd, &R_SET );
}


inline bool Descriptor::canWriteWait( long sec, long usec )
{
	timeval tzero;
	tzero.tv_sec = sec;
	tzero.tv_usec = usec;
	fd_set W_SET;
	FD_ZERO( &W_SET );
	FD_SET( fd, &W_SET );
	while( select( fd+1, 0, &W_SET, 0, &tzero ) < 0 )
	{
		
		if( errno == EINTR )
			continue;
		return false;
		
	}
	return (bool)FD_ISSET( fd, &W_SET );
}


inline bool Descriptor::canReadWait( long sec, long usec )
{
	timeval tzero;
	tzero.tv_sec = sec;
	tzero.tv_usec = usec;
	fd_set R_SET;
	FD_ZERO( &R_SET );
	FD_SET( fd, &R_SET );
	while( select( fd+1, &R_SET, 0, 0, &tzero ) < 0 )
	{
		
		if( errno == EINTR )
			continue;
		return false;
		
	}
	
	return (bool)FD_ISSET( fd, &R_SET );
}


class Socket : public sockaddr_in
{
protected:
	Descriptor sock;
	int err;
	int sock_type;
	char ip[ 128 ];  // char string version of ip address.
	sockaddr_in foreign; // for UDP stuff
	
	
public:
	
	Socket()
	:	err(0), sock_type(SOCK_STREAM)
	{
		strcpy( ip, "0.0.0.0" );  // 0.0.0.0 = INADDR_ANY
		sin_port = 0;
		sin_family = AF_INET;
		sin_addr.s_addr = htonl( INADDR_ANY );
	}
	
	Socket( int type, int port );
	Socket( int type, const char * address, int port );
	Socket( const char * address, int port, int desc );
	
	virtual ~Socket()
	{
		if( sock >= 0 )
			sock.close();
	}
	
	void close()
	{
		if( sock >= 0 )
			sock.close();
		sock = -1;
	}
	
	int error() { return err; }
	int eof() { return sock.eof(); }
	int getSockType() { return sock_type; }
	bool isUDP() { return ( sock_type == SOCK_DGRAM ); }
	bool isTCP() { return ( sock_type == SOCK_STREAM ); }
	char *getHostName() { return ip; }
	int getFamily() { return sin_family; }
	void setDescriptor( int x ) { sock = x; }
	int getDescriptor() { return (int)sock; }
	int reuseAddr();
	int noDelay() { return sock.noDelay(); }
	int nonBlock() { return sock.nonBlock(); }
	int getPort() { return ntohs( sin_port ); }
	int open();
	Socket * accept();
	int resolveIP();
	int connect();
	int getSockName();
	int getPeerName();
	int write( const char * );
	int write( const char *, int );
	int read( char *, int );
	int send( const char *, int );
	int recv( char *, int );
	int willEcho();
	int wontEcho();
	int wontSuppressGA();
	int willSuppressGA();
	bool canRead() { return sock.canRead(); }
	bool canWrite() { return sock.canWrite(); }
	bool canReadWait( long s, long u ) { return sock.canReadWait( s, u ); }
	bool canWriteWait( long s, long u ) { return sock.canWriteWait( s, u ); }
};

inline int Socket::write( const char * buf )
{	return sock.write( buf );	}

inline int Socket::write( const char * buf, int bytes )
{	return sock.write( buf, bytes );	}

inline int Socket::read( char * buf, int max )
{	return sock.read( buf, max );	}

inline int Socket::send( const char * buf, int bytes )
{	return sock.sendto( buf, bytes, 0,
		(struct sockaddr*)&foreign, sizeof(foreign) );	}

inline int Socket::recv( char * buf, int bytes )
{	return sock.recvfrom( buf, bytes, 0,
		(struct sockaddr*)&foreign, 0 );	}


const long DEFAULT_BUFSIZE = 2048;

class Buf
{
protected:
	char *	_buf;
	char *	_front;
	long	_buf_size;
	long	_bytes;
	
public:
	Buf()
	:	_buf( new char[ DEFAULT_BUFSIZE ] ), _front( _buf ),
	_buf_size( DEFAULT_BUFSIZE ),
	_bytes(0)
	{
	}
	
	Buf( long size )
	:	_buf( new char[ size ] ), _front( _buf ),
	_buf_size( size ), _bytes(0)
	{
	}
	
	bool setSize( long size );
	void clr() { _bytes = 0; _front = _buf; }
	void add( char * buf );
	void add( char * buf, long bytes ); // faster
	char * getLine( char * buf );
	char * getLine( char * buf, long maxsize );
	bool findLine( const char * str );
	bool findStr( const char * str );
	char * next() { return (_front + _bytes); } 
	void putLine( char * str );
	void putStr( char * str );
	
	void filter_isprint();
	void filter_isascii();
	
};

// Could write as two classes, SocketInBuf, and SocketOutBuf, but
// most real world apps use 2-way communication so one class
// serves as well.

class SocketBuf : public Socket
{
protected:
	Buf _inbuf;
	Buf _outbuf;
	
public:
	SocketBuf()
	:	Socket()
	{
	}
	
	SocketBuf( int type, int port )
	:	Socket( type, port )
	{
	}
	
	SocketBuf( int type, const char * address, int port )
	:	Socket( type, address, port )
	{
	}
	
	SocketBuf( const char * address, int port, int desc )
	:	Socket( address, port, desc )
	{
	}
	
	bool setInBufSize( long size ) { return _inbuf.setSize( size ); }
	bool setOutBufSize( long size ) { return _outbuf.setSize( size ); }
	long readBuf();
	long writeBuf();
	void clrInBuf() { _inbuf.clr(); }
	void clrOutBuf() { _outbuf.clr(); }
	void clrBufs() { _outbuf.clr(); _inbuf.clr(); }
	
	char * getLine( char * buf ) { return _inbuf.getLine( buf ); }
	char * getLine( char * buf, long maxsize )
	{ return _inbuf.getLine( buf, maxsize ); }
	bool gotLine( const char * str ) { return _inbuf.findLine( str ); }
	bool gotStr( const char * str ) { return _inbuf.findStr( str ); }
	
	void putLine( char * str );
	void putStr( char * str );
	// under construction
};

#endif
