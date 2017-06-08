/*
 *	WHEEL OF FORTUNE online - CLIENT
 *	Final project for Networking Course
 *	Written in: C-compiled-as-VC++
 *	Members:	_______
 *				_______
 *				1559040
 */

#include <stdio.h>		// printf(), ...
#include <stdlib.h>		// exit(), malloc(), ...
#include <WinSock2.h>	// duh...

// Be REALLY sure that ws2_32.lib is linked
#pragma comment( lib, "ws2_32.lib" )

// Errors
enum
{
	ERROR_1 = 1,
	ERROR_2,
	ERROR_3,
	ERROR_4	
};

// Prototypes
static void		WOF_ClientCallThisFirst( void );
static SOCKET	WOF_ClientPrepareGame( void );
static void		WOF_ClientPlayGame( SOCKET com_socket );
static void		WOF_ClientCallThisLast( void );

int main( void )
{
	WOF_ClientCallThisFirst();

	SOCKET com_socket =
		WOF_ClientPrepareGame();

	WOF_ClientPlayGame( com_socket );

	// Cleanup
	WOF_ClientCallThisLast();
	getc( stdin );

	return 0;
}

//================================================================
static void WOF_ClientCallThisFirst( void )
{
	// Request WINSOCK version 2.2 via MAKEWORD macro
	WORD wsa_version_requested = MAKEWORD( 2, 2 );

	// Starting WINSOCK API
	WSADATA wsa_data;
	int wsa_err = WSAStartup( wsa_version_requested, &wsa_data );

	// Check if there are errors:
	{
		if (wsa_err != NO_ERROR) {
			printf( "Error: No usable WINSOCK dll found.\n" );
			exit( ERROR_1 );
		}
		else {
			printf( "WINSOCK dll found!\n" );
			printf( "Status: %s.\n\n", wsa_data.szSystemStatus );
		}
	}
	// Ensure WINSOCK version 2.2:
	{
		if (LOBYTE( wsa_data.wVersion ) != 2  ||
			HIBYTE( wsa_data.wVersion ) != 2) {
			printf( "Error: Your WINSOCK dll (%u.%u) does not support version 2.2.\n",
					LOBYTE( wsa_data.wVersion ),
					HIBYTE( wsa_data.wVersion ));

			WSACleanup();
			exit( ERROR_2 );
		}
		else {
			printf( "WINSOCK dll (%u.%u).\n",
					LOBYTE( wsa_data.wVersion ),
					HIBYTE( wsa_data.wVersion ));
			printf( "The highest version this dll can support: %u.%u.\n\n",
					LOBYTE( wsa_data.wHighVersion ),
					HIBYTE( wsa_data.wHighVersion ));
		}
	}
}
//================================================================
static SOCKET WOF_ClientPrepareGame( void )
{
	// Create a socket (for server <-> client connection)
	SOCKET com_socket;
	// USING:	AF_INET address family (IPv4),
	//			TCP socket type (STREAM),
	//			Transmission Control Protocol
	com_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	{
		if (com_socket == INVALID_SOCKET) {
			printf( "Error: Failed at socket() (err_code: %d).\n", WSAGetLastError() );
			// Cleanup
			closesocket( com_socket ),	WSACleanup();
			exit( ERROR_3 );
		}
		else
			printf( "Successfully executed socket()!\n" );
	}

	// Create a sockaddr_in object
	//   containing the game server's connection details
	SOCKADDR_IN game_server;
	{
		// AF_INET: Internet address family with IPv4
		// Socket is bound to IP address 127.0.0.1
		// Socket is bound to port 51234 (which is converted
		// from host byte order to network byte order with htons)
		game_server.sin_family		= AF_INET;
		game_server.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		game_server.sin_port		= htons( 51234 );
	}
	// Connect + error handling
	{
		if (connect( com_socket, (SOCKADDR *)&game_server, sizeof game_server )
					== SOCKET_ERROR) {
			printf( "Error: Failed at connect() (err_code: %d).\n", WSAGetLastError() );
			// Cleanup
			closesocket( com_socket ),	WSACleanup();
			exit( ERROR_4 );
		}
		else
			printf( "Successfully executed connect()!\n\n" );
	}

	// Receive and echo the welcoming message
	{
		char buf_received[ 500 ]	= "";
		recv( com_socket, buf_received, 64, 0 );
		puts( buf_received );
	}

	return com_socket;
}
//================================================================
static void WOF_ClientPlayGame( SOCKET com_socket )
{
	printf( "\nAwaiting..." );

	// Enter a nickname
	do {
		char buf_sent[ 500 ]	= "";
		char buf_received[ 500 ]	= "";

		// Receive request for nickname
		{
			recv( com_socket, buf_received, 64, 0 );
			printf( "\r%s", buf_received );
		}
		// User input
		{
			fgets( buf_sent, 30, stdin );
			putchar( '\n' );
		}

		// Send nickname to server
		send( com_socket, buf_sent, 64, 0 );
		// Receive response
		recv( com_socket, buf_received, 64, 0 );

		if (strcmp( buf_received, "[OK]" ) == 0)
			break;
		else
			if (strcmp( buf_received, "[DUP]" ) == 0)
				printf( "This nickname is already used. "
						"Choose another one.\n" );
	}
	while (1);
	

	closesocket( com_socket );
}
//================================================================
static void	WOF_ClientCallThisLast( void )
{
	WSACleanup();
}