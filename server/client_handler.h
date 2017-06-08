#pragma once
#include "magichat.h"

class client_handler
{
	char recv[500], send[5000], **name;
	CSocket *clients, server, finder;
	
	magichat game;

	const int port = 1234;
	int nclient, *score, turn;

	const char *match = "Your username is taken!";
	const char *success = "You have successfully signed in";
	const char *empty = "Your username is empty!";
	const char *winner = "Congratulation! You are the winner";
	const char *looser = "You loose!";
	const char*rightans = "Congrats, you're right";
	const char*wrongans = "You're wrong horribly!!!";
	const char*notyourturn = "Your turn is yet to come, please wait ^_^";
	const char*lostyourturn = "You spinned 0, you lost your turn";
	const char*first = "Congratulation! You've won the first prize";
	const char*second = "Congratulation! Youe've won the second prize";
	const char*last = "You've given your best . . . I think :)";
	const char*wongansflag = "wrong";
	const char*rightansflag = "right";
	const char*ansflag = "answer";
	const char*loopflag = "loop";
	const char*endgameflag = "end";
public:
	
	void init();
	void accept();
	void gatherusername();
	void findclient();
	void print(int index);
	void swap(string source, int index);
	void play();
	void waitturn(int turn);
	void announce();
	void close();


	bool checkdupplication(string name, int index);
	bool checkinvalid(char*name);

	int checkspin();
	int switchturn();
	int theunchosen();
};

