/*

....Copyright(C).1995.Melvin.Smith.............. Enjoy. 
socket.cc
*/

#include <ctype.h>
#include "socket.h"

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


int Descriptor::write( const char * buf, int size )
{
	
	int error;
	int bytes = 0;
	int max_write = 1024;
	
	if( size < max_write )
		max_write = size;
	
	while( 1 )
	{
		if( ( error = ::write( fd, ( buf + bytes ), max_write ) ) >= 0 )
		{
			bytes += error;
			if( bytes >= size )
				return bytes;
			else
				max_write = size - bytes;
			continue;
		}
		else
		{
			if( errno == EAGAIN || errno == EINTR )
				continue;
			else
				return -1;
		}
	}
	return -1;
	
}


int Descriptor::read( char * buf, int max )
{
	
	int bytes = 0;
	int error;
	while( 1 )
	{
		if( ( error = ::read( fd, buf + bytes, ( max - bytes ) ) ) > 0 )
			bytes += error;
		else if( !error )
			{
				eofd = true;
				return bytes;
			}
			else
			{
				if( errno == EAGAIN || errno == EWOULDBLOCK )
					return bytes;
				else if( errno == ECONNRESET )
					{
						eofd = true;
						return 0;
					}
					else if( errno == EINTR )
							continue;
				return error;
			}
	}
	return -1;
}


// UDP stuff not finished, just cutting and pasting from
// the TCP code, will finish soon.
int Descriptor::sendto( const char * buf, int size, int flags,
	const struct sockaddr * to, int tolen )
{
	
	int error;
	int bytes = 0;
	int max_write = 1024;
	
	if( size < max_write )
		max_write = size;
	
	while( 1 )
	{
		if( ( error = ::sendto( fd, ( buf + bytes ), max_write, flags,
							to, tolen ) ) >= 0 )
		{
			bytes += error;
			if( bytes >= size )
				return bytes;
			else
				max_write = size - bytes;
			continue;
		}
		else
		{
			if( errno == EAGAIN || errno == EINTR )
				continue;
			else
				return -1;
		}
	}
	return -1;
	
}


// UDP stuff not finished, just cutting and pasting from
// the TCP code, will finish soon.
int Descriptor::recvfrom( char * buf, int max, int flags,
	struct sockaddr * from, int * fromlen )
{
	
	int bytes = 0;
	int error;
	while( 1 )
	{
		if( ( error = ::recvfrom( fd, buf + bytes, ( max - bytes ),
							flags, from, (socklen_t *)fromlen ) ) > 0 )
			bytes += error;
		else if( !error )
			{
				eofd = true;
				return bytes;
			}
			else
			{
				if( errno == EAGAIN || errno == EWOULDBLOCK )
					return bytes;
				else if( errno == ECONNRESET )
					{
						eofd = true;
						return 0;
					}
					else if( errno == EINTR )
							continue;
				return error;
			}
	}
	return -1;
	
}


int Descriptor::noDelay()
{
	
	return fcntl( fd, F_SETFL, FNDELAY );
	
}


int Descriptor::nonBlock()
{
	
	return fcntl( fd, F_SETFL, O_NONBLOCK );
	
}


Socket::Socket( int type, int port )
{
	if( type != SOCK_STREAM && type != SOCK_DGRAM )
	{
		printf( "Invalid socket type.\n" );
		exit(0);
	}
	sock_type = type;
	
	
	if( ( sock = ::socket( AF_INET, sock_type, 0 ) ) == -1 )
	{
		err = SOCK_NOT_CREATED;
		return;
	}
	
	strcpy( ip, "0.0.0.0" );
	sin_port = htons( port );
	sin_family = AF_INET;
	sin_addr.s_addr = htonl( INADDR_ANY );
	err = 0;
}


Socket::Socket( int type, const char * address, int port )
{
	strcpy( ip, address );
	sin_port = htons( port );
	sin_family = AF_INET;
	
	if( type != SOCK_STREAM && type != SOCK_DGRAM )
	{
		printf( "Invalid socket type.\n" );
		exit(0);
	}
	sock_type = type;
	
	if( isdigit( *address ) )
	{
		if( ( sin_addr.s_addr = inet_addr( address ) ) == (unsigned int)-1 )
		{
			perror( "inet_addr" );
			err = SOCK_BAD_ADDRESS;
			return;
		}
	}
	else
	{
		struct hostent *he = ::gethostbyname( address );
		if( !he )
		{
			
			perror( "gethostbyname" );
			err = SOCK_UNKNOWN_HOST;
			return;
			
		}
		
		memcpy( (char*)&sin_addr, he->h_addr, sizeof( sin_addr ) );
	}
	
	
	if( ( sock = ::socket( AF_INET, sock_type, 0 ) ) == -1 )
	{
		perror( "socket" );
		err = SOCK_NOT_CREATED;
		return;
	}
	
	err = 0;
}


// This is for recreating a socket on a live TCP connection after a
// reboot.
Socket::Socket( const char *address, int port, int desc )
{
	sock = desc;
	sin_port = htons( port );
	sin_family = AF_INET;
	
	sock_type = SOCK_STREAM;
	if( !*address || !strcmp( address, "0.0.0.0" ) )
	{
		strcpy( ip, "0.0.0.0" );
		return;
	}
	
	strcpy( ip, address );
	
	// These two calls don't necessarily stop the show if they fail
	// but some things might not work as planned. Need to think
	// more about how to handle failures here.
	getSockName();
	getPeerName();
/*
	Testing - getSockName() should now take care of this.
	if( isdigit( *ip ) )
	{
		if( ( sin_addr.s_addr = inet_addr( ip ) ) == (unsigned int)-1 )
		{
			perror( "inet_addr" );
			err = SOCK_BAD_ADDRESS;
			return;
		}
	}
	else
	{
		struct hostent *he;
		if( !( he = ::gethostbyname( address ) ) )
		{
			perror( "gethostbyname" );
			err = SOCK_UNKNOWN_HOST;
			return;
		}

		memcpy( (char*)&sin_addr, he->h_addr, sizeof( sin_addr ) );
	}
*/
}



int Socket::open()
{
	
	if( ( ::bind( (int)sock,
					( sockaddr * )this, sizeof( sockaddr_in ) ) ) == -1 )
		
	{
		// err = SOCK_CANT_BIND;
		return -1;
	}
	
	// Skip listen() call for UDP sockets
	if( sock_type == SOCK_DGRAM )
		return 0;
	
	if( ( ::listen( (int)sock, 5 ) ) == -1 )
	{
		// err = SOCK_CANT_LISTEN;
		return -1;
	}
	
	return 0;
}


Socket * Socket::accept()
{
	Socket * newSock = new Socket;
	int desc;
	int size = sizeof( sockaddr_in );
	
	if( ( desc = ::accept( sock, ( sockaddr * ) newSock, (socklen_t *)&size ) ) < 0 )
	{
		// err = SOCK_CANT_ACCEPT;
		delete newSock;
		return 0;
	}
	
	newSock->setDescriptor( desc );
	
	if( newSock->getFamily() != AF_INET )
	{
		// err = SOCK_INVALID_ADDRESS;
		newSock->close();
		delete newSock;
		return 0;
	}
	
	if( newSock->noDelay() == -1 )
	{
		// err = SOCK_CANT_SET_NODELAY;
		newSock->close();
		delete newSock;
		return 0;
	}
	
	// Now get info for new connection
	if( newSock->getPeerName() == -1 )
	{
		// err = SOCK_CANT_GET_PEER_NAME;
		delete newSock;
		return 0;
	}
	
	if( newSock->resolveIP() == -1 )
	{
		delete newSock;
		return 0;
	}
	
	return newSock;
}


int Socket::resolveIP()
{
	struct hostent * he = ::gethostbyaddr( (char *) &sin_addr,
			sizeof( sin_addr ), AF_INET );
	
	if( he )
		strcpy( ip, he->h_name );
	else
	{
		
		unsigned long addr = ntohl( sin_addr.s_addr );
		if( (long) addr == -1 )
		{
			*ip = '\0';
			return -1;
		}
		
		sprintf( ip, "%ld.%ld.%ld.%ld", ( addr >> 24 ) & 0x000000ff,
			( addr >> 16 ) & 0x000000ff,
			( addr >> 8  ) & 0x000000ff,
			( addr       ) & 0x000000ff  );
	}
	
	return 0;
}


int Socket::getSockName()
{
	int size = sizeof( sockaddr );
	if( ::getsockname( sock, (sockaddr *)this, (socklen_t *)&size ) == -1 )
	{
		perror( "Socket::getsockname" );
		return -1;
	}
	
	return 0;
}


int Socket::getPeerName()
{
	int size = sizeof( sockaddr );
	if( ::getpeername( sock, (sockaddr *)this, (socklen_t *)&size ) == -1 )
	{
		perror( "Socket::getPeerName" );
		return -1;
	}
	
	return 0;
}


int Socket::reuseAddr()
{
	int i;
	if( ::setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof(i) ) == -1 )
	{
		err = SOCK_SOCKOPT_ERR;
		return -1;
	}
	
	return 0;
}


int Socket::connect()
{
	if( sock_type != SOCK_STREAM )
	{
		printf( "connect: socket is not TCP.\n" );
		exit(0);
	}
	
	if( ::connect( sock, (sockaddr *)this, sizeof( sockaddr ) ) )
	{
		
		if( errno != EWOULDBLOCK )
		{
			err = SOCK_NOT_CONNECTED;
			return -1;
		}
	}
	
	return 0;
}

int Socket::willEcho()
{	return write( WILL_ECHO, 3 );	}

int Socket::willSuppressGA()
{	return write( WILL_SUPPRESS_GA, 3 );	}

int Socket::wontEcho()
{	return write( WONT_ECHO, 3 );	}

int Socket::wontSuppressGA()
{	return write( WONT_SUPPRESS_GA, 3 );	}


bool Buf::setSize( long size )
{
	char * temp = _buf;
	_buf = new char[ size ];
	if( ! _buf )
	{
		_buf = temp;
		return false;
	}
	
	_buf_size = size;
	_front = _buf;
	return false;
}


// buf must be null terminated.
// use add( buf, size ) for better performance
void Buf::add( char * buf )
{
	strcpy( _front + _bytes, buf );
	_bytes += strlen( buf );
}

// buf does not have to be null terminated
// this is the fastest way to add to buffer
void Buf::add( char * buf, long bytes )
{
	// add checks for overflow and resize
	memcpy( _front + _bytes, buf, bytes );
	_bytes += bytes;
}


void Buf::filter_isprint()
{
	
}

char * Buf::getLine( char * line )
{
	char * ptr = line;
	char * save = _front;
	if( _bytes == 0 )
		return 0;
	
	for( ; _bytes > 0 && *_front != '\n'; _front++, ptr++, _bytes-- )
		*ptr = *_front;
	if( *_front != '\n' )
	{
		_front = save;
		return 0;
	}
	
	*(++ptr) = 0;
	if( _bytes )
		if( --_bytes && *(++_front) == '\r' )
			_bytes--, _front++;
	return line;
}

// Add support for wildcards with %
bool Buf::findLine( const char * line )
{
	return false;
}

// Add support for wildcards with %
bool Buf::findStr( const char * str )
{
	if( _bytes <= 0 )
		return false;
	
	int count = _bytes;
	int count2;
	const char * ptr1;
	char * ptr2 = _front;
	while( count )
	{
		ptr1 = str;
		while( count && *ptr2 != *ptr1 )
			ptr2++, count--;
		
		if( !count )
			return false;
		
		count2 = strlen( str ) - 1;
		for( ; ; ptr1++, ptr2++, count--, count2-- )
		{
			if( !count2 )
				return true;
			if( !count )
				return false;
			if( *ptr1 != *ptr2 )
				break;
		}
	}
	
	return false;
}


long SocketBuf::readBuf()
{
	long bytes;
	char buf[ 4096 ];
	bytes = Socket::read( buf, 4095 );
	if( bytes > 0 )
		_inbuf.add( buf, bytes );
	return bytes;
}




