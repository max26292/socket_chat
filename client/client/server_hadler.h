#pragma once
#include "standard.h"

class server_hadler
{
	CSocket client, *clients;
	const int nclient = 3;
	char recv[500], send[500];
	const char*recvmsg = "received";

public:
	void play();
	void init();
	void printrule();
	void signin();
	void close();
	void recveive_question();
	void stay();
	void answer(int index);
	void endgame(int index);
};

