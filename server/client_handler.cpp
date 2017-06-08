#include "stdafx.h"
#include "client_handler.h"
#pragma warning(disable : 4996)


int client_handler::switchturn()
{
	default_random_engine generator((unsigned int)time(NULL));
	uniform_int_distribution<int>turn(0, --nclient);
	return turn(generator);
}

int client_handler::theunchosen()
{
	int unchosen = -1;
	for (int i = 0; i < nclient; i++)
	{ 
		if (score[i] == 0) 
		{ 
			return i; 
		} 
	}
	return unchosen;
}

void client_handler::init()
{
	cout << "Enter max number of clients: "; cin >> nclient;
	clients = new CSocket[nclient];
	name = new char*[nclient];
	score = new int[nclient];

	game.init();
	AfxSocketInit();
	server.Create(port);
	server.Listen(1);
	for (int i = 0; i < nclient; i++) { score[i] = 0; }
	cout << "Waiting for client on port " << port << endl;
}

void client_handler::accept()
{
	for (int i = 0; i < nclient; i++)
	{
		if (server.Accept(clients[i]))
		{
			int temp = i, temp2 = nclient;
			cout << "Client " << ++temp << " connected!" << endl;
			cout << "There are " << temp2 - temp << " left need to be connected" << endl;
		}
	}
	cout << "All clients are connected" << endl;
}

void client_handler::gatherusername()
{
	bool loop = false;
	string temp;
	for (int i = 0; i < nclient; i++)
	{
		do
		{
			int len = clients[i].Receive(recv, 500, 0);
			recv[len] = 0;
			temp = recv;
			name[i] = new char[temp.size()];
			if (i == 0) { clients[i].Send(success, strlen(success), 0); }
			if (i > 0)
			{
				bool checkname = checkdupplication(temp, i);
				if (checkname == true)
				{
					clients[i].Send(match, strlen(match), 0);
					loop = true;
				}
				else { clients[i].Send(success, strlen(success), 0); loop = false; }
			}
		} while (loop == true);
		swap(temp, i);
		cout << "user "; print(i); cout << " has connected to server" << endl;
	}
}

void client_handler::findclient()
{
	_itoa_s(nclient, send, 10);
	cout << send << endl;
	system("PAUSE");
	do
	{
		cout << "Sending number of users required: " << send << endl;
		finder.Send(send, strlen(send), 0);
		cout << "Waiting for clients to respond . . ." << endl;
		int len = finder.Receive(recv, strlen(recv), 0);
		recv[len] = 0;
		cout << recv << endl;
		system("PAUSE");
	} while (strcmp(recv, "Preparation ready") != 0); finder.Close();
}

void client_handler::swap(string source, int index)
{
	for (unsigned int i = 0; i < source.size(); i++) { name[index][i] = source[i]; }
}


void client_handler::play()
{
	do
	{
		int tempscore = checkspin();
		turn = switchturn();
		waitturn(turn);
		int currentscore = 0;
		cout << "Sending question: " << game.ask() << endl;
		cout << "Sending hints: " << game.gethint() << endl;

		if (tempscore == 0) { clients[turn].Send(lostyourturn, strlen(lostyourturn), 0); }
		else
		{
			if (game.getnumberquestion() == 0) { turn = theunchosen(); }
			clients[turn].Send(loopflag, strlen(loopflag), 0);
			int len = clients[turn].Receive(recv, 500, 0);
			recv[len] = 0;

			strcpy_s(send, game.ask().c_str());
			clients[turn].Send(send, strlen(send), 0);
			strcpy(send, "\nHint: ");
			strcat(send, game.gethint().c_str());
			len = clients[turn].Receive(recv, 500, 0);
			recv[len] = 0;
			
			clients[turn].Send(send, strlen(send), 0);
			len = clients[turn].Receive(recv, 500, 0);
			recv[len] = 0;
			
			clients[turn].Send(ansflag, strlen(ansflag), 0);
			bool rightflag = false;
			do
			{
				if (rightflag == true)
				{
					strcpy_s(send, game.ask().c_str());
					strcat(send, "\nHint: ");
					strcat(send, game.gethint().c_str());
					clients[turn].Send(send, strlen(send), 0);
				}
				len = clients[turn].Receive(recv, 500, 0);
				recv[len] = 0;
				string holder(recv, 0, strlen(recv));
				currentscore += game.checkanswer(holder);
				if (currentscore == 0)
				{
					tempscore = 0;
					clients[turn].Send(wongansflag, strlen(wongansflag), 0);
					len = clients[turn].Receive(recv, 500, 0);
					recv[len] = 0;
					clients[turn].Send(wrongans, strlen(wrongans), 0);
					break;
				}
				else
				{
					tempscore += currentscore;
					clients[turn].Send(rightansflag, strlen(rightansflag), 0);
					len = clients[turn].Receive(recv, 500, 0);
					recv[len] = 0;
					clients[turn].Send(rightans, strlen(rightans), 0);
					rightflag = true;
				}
			} while (currentscore != 0 && game.getnumberquestion() > -1);
			score[turn] = tempscore;
		}
	} while (game.getnumberquestion() > -1);

	announce();

}

void client_handler::waitturn(int turn)
{
	for (int i = 0; i < nclient; i++)
	{
		if (i == turn) { return; }
		else
		{
			strcpy_s(send, notyourturn);
			strcat(send, name[i]);
			clients[i].Send(send, strlen(send), 0);
			cout << "Sending user "; print(i); cout << " waiting message!" << endl;
			int len = clients[i].Receive(recv, 500, 0);
			recv[len] = 0;
		}
	}
}

void client_handler::announce()
{
	int highest, range = nclient, index;

	for (int i = 0; i < range; i++)
	{
		for (int j = i; j <= nclient; j++)
		{
			if (score[j] > score[i]) { highest = score[j]; index = j; }
		}
	}
	char score[10];
	_itoa_s(highest, score, 10);

	clients[index].Send(endgameflag, strlen(endgameflag), 0);
	range = clients[index].Receive(recv, 500, 0);
	recv[range] = 0;
	clients[index].Send(first, strlen(first), 0);
	clients[index].Send(score, strlen(score), 0);
}

void client_handler::close() { for (int i = 0; i < nclient; i++) { clients[i].Close(); } }

bool client_handler::checkdupplication(string name, int index)
{
	for (int i = 0; i <= index; i++)
	{
		if (this->name[index][i] != name[i]) { return false; }
	}
	return true;
}

bool client_handler::checkinvalid(char * name)
{
	if (strcmp(name, " ") == 0) { return true; }
	else if (strcmp(name, "") == 0) { return true; }
	return false;
}

int client_handler::checkspin()
{
	int score = game.spin();
	if (score > 0) { return score; }
	return 0;
}

void client_handler::print(int index)
{
	for (int j = 0; j < sizeof(name[index]); j++) { cout << name[index][j]; }
}
