/*
 *	WHEEL OF FORTUNE online - SERVER
 *	Final project for Networking Course
 *	Written in: C-compiled-as-VC++
 *	Members:	_______
 *				_______
 *				1559040
 */
#include "stdafx.h"
#include <stdio.h>		// printf(), ...
#include <stdlib.h>		// exit(), malloc(), ...
#include <string.h>		// strcpy(), ...
#include <WinSock2.h>	// duh...

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CTR_SECURE_NO_WARNINGS
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

// Structure
typedef struct WOF_Control_t
{
	SOCKET		*com_socket;
	SOCKADDR_IN *game_client;
	int			num_of_players;
} WOF_Control;
typedef struct WOF_Player_t
{
	int		id;
	char	nickname[ 30 ];
	int		score;
} WOF_Player;

// Prototypes
static void WOF_CallThisFirst( void );
static void WOF_PrepareGame( WOF_Control *control );
static void WOF_PlayGame( WOF_Control *control );
static void WOF_CallThisLast( WOF_Control *control );

/*
// Main
int main( void )
{
	// Behind-the-scenes messy socket stuff
	WOF_CallThisFirst();

	// Even more messy socket stuff
	WOF_Control control;
	WOF_PrepareGame( &control );

	// It's not really that fun
	WOF_PlayGame( &control );

	// Cleanup
	WOF_CallThisLast( &control );

	// This should normally NOT happen
	printf( "Server shutting down...\n" );
	getc( stdin );
	return 0;
}
*/

//================================================================
static void WOF_CallThisFirst( void )
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
			getchar();
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
			getchar();
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
static void WOF_PrepareGame( WOF_Control *control )
{
	// Create a socket (for listening)
	SOCKET listen_socket;
	// USING:	AF_INET address family (IPv4),
	//			TCP socket type (STREAM),
	//			Transmission Control Protocol
	listen_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	{
		if (listen_socket == INVALID_SOCKET) {
			printf( "Error: Failed at socket() (err_code: %d).\n", WSAGetLastError() );
			// Cleanup
			closesocket( listen_socket ),	WSACleanup();
			exit( ERROR_3 );
		}
		else
			printf( "Successfully executed socket()!\n" );
	}

	// Create a sockaddr_in object
	//   containing the game server's connection details
	SOCKADDR_IN game_server;
	{
		// AF_INET: Internet address family
		// Socket is bound to IP address 127.0.0.1
		// Socket is bound to port 51234
		//   which is converted from host byte order
		//   to network byte order with htons()
		game_server.sin_family		= AF_INET;
		game_server.sin_addr.s_addr = inet_pton(AF_INET, "127.0.0.1", &(game_server.sin_addr));
		game_server.sin_port		= htons( 51234 );
	}
	// Binding + error handling
	{
		if (bind( listen_socket, (SOCKADDR *)&game_server, sizeof game_server )
					== SOCKET_ERROR) {
			printf( "Error: Failed at bind() (err_code: %d).\n", WSAGetLastError() );
			// Cleanup
			closesocket( listen_socket ),	WSACleanup();
			exit( ERROR_4 );
		}
		else
			printf( "Successfully executed bind()!\n" );
	}
	// Listening + error handling
	{
		if (listen( listen_socket, SOMAXCONN ) == SOCKET_ERROR)
			printf( "Error: Failed at listen() (err_code: %d).\n", WSAGetLastError() );
		else
			printf( "Successfully executed listen()! Waiting for connections...\n\n" );
	}

	// Setting limit
	int max_player = 0;
	{
		while (1) {
			printf( "Enter players limit: " );
		
			char numbuf[ 10 ] = "";
			fgets( numbuf, 10, stdin );
			max_player = atoi( numbuf );

			if (max_player <= 0)
				printf( "max_player > 0 only.\n\n" );
			else
				break;
		}
	}

	// Allocate sockets (for server <-> client connection)
	control->com_socket = (SOCKET *)
		malloc( max_player * sizeof(SOCKET) );
	

	// Allocate sockaddr_in objects
	//   used to store game clients' connection details
	control->game_client = (SOCKADDR_IN *)
		malloc( max_player * sizeof(SOCKADDR_IN) );
	

	printf( "\nTIP: Server is ready. Run your client program(s).\n\n");

	// This entire block is for providing all clients
	//   with enough sockets
	{
		// addrlen for accept()
		int addr_len = sizeof(SOCKADDR_IN);
		int i = 0;
		char str[INET_ADDRSTRLEN];

		while (i < max_player) {

			control->com_socket[ i ] = SOCKET_ERROR;

			while (control->com_socket[ i ] == SOCKET_ERROR) {
				// Accepting a connection
				// An actual server <-> client connection is established
				//   on the new socket return by accept()
				control->com_socket[ i ] = 
					accept( listen_socket,
							(SOCKADDR *)&control->game_client[ i ],
							&addr_len );
			}
			printf( "Client %d connected!\n"
					"Details: "
					"client_ip_addr(%s), "
					"client_port(%u)\n\n",
					i,
					//inet_ntoa( control->game_client[ i ]
								//.sin_addr )
					inet_ntop(AF_INET, &(control->game_client[i].sin_addr), /*string here*/str, addr_len),
					ntohs( control->game_client[ i ]
								.sin_port ));
			++i;
		}
	}
	// No more new clients
	closesocket( listen_socket );

	// Prep a welcoming message for all clients
	{
		int i = 0;
		while (i < max_player)
			send( control->com_socket[ i ],
				  "[WELCOME TO WHEEL OF FORTUNE]",
				  64, 0 ), ++i;
	}
	control->num_of_players = max_player;
}
//================================================================
static void WOF_PlayGame( WOF_Control *control )
{
	// Allocate all players stats
	WOF_Player *player = (WOF_Player *)
		malloc( control->num_of_players * sizeof(WOF_Player) );

	// Initialise stats
	{
		int i = 0;
		int num_of_players = control->num_of_players;
		
		while (i < num_of_players) {

			player[ i ].id		= i;
			player[ i ].score	= 0;

			char buf_received[ 500 ]	= "";

			int dup_detected;
			do {
				// No duplicate (for now)
				dup_detected = 0;

				// Request a nickname
				send( control->com_socket[ i ],
					  "Enter your nickname: ",
					  64, 0 );

				// Receive a nickname
				recv( control->com_socket[ i ],
					  buf_received,
					  64, 0 );
			
				// Check for duplication with previous nickname(s)
				int idup = 0;
				while (idup < i) {
					
					if (strcmp( buf_received, player[ idup ].nickname ) == 0) {
						send( control->com_socket[ i ],
							  "[DUP]", 64, 0 );
						// Whoops
						dup_detected = 1;
						break;
					}
					++idup;
				}
			}
			while (dup_detected != 0);

			// No duplicate (for real)
			send( control->com_socket[ i ],
							  "[OK]", 64, 0 );

			strcpy_s( player[ i ].nickname, buf_received );
			++i;
		}
	}

	free( player );
}
//================================================================
static void WOF_CallThisLast( WOF_Control *control )
{
	free( control->com_socket );
	free( control->game_client );

	WSACleanup();
}