#include "stdafx.h"
#include "server_hadler.h"


void server_hadler::close() { for (int i = 0; i < nclient; i++) { clients[i].Close(); } }

void server_hadler::recveive_question()
{
	bool loop = false;
	int temp = nclient;
	temp--;
	for (int i = 0; i < nclient; i++)
	{
		do
		{
			int count = 0;
			int len = clients[i].Receive(recv, 500, 0);
			recv[len] = 0;
			if (strcmp(recv, "loop") == 0) { loop = true; }
			if (strcmp(recv, "answer") == 0)
			{
				do
				{
					if (count > 0)
					{
						len = clients[i].Receive(recv, 500, 0);
						recv[len] = 0;
						if (strcmp(recv, "end") == 0)
						{
							clients[i].Send(recvmsg, strlen(recvmsg), 0);
							endgame(i);
						}
						cout << recv << endl;
					}

					answer(i);
					len = clients[i].Receive(recv, 500, 0);
					recv[len] = 0;

					if (strcmp(recv, "right") == 0) { loop = true; count++; }
					else { loop = false; }

					clients[i].Send(recvmsg, strlen(recvmsg), 0);
					len = clients[i].Receive(recv, 500, 0);
					recv[len] = 0;
					cout << recv << endl;
				} while (loop == true);
			}
			else if (strcmp(recv, "answer") != 0)
			{
				cout << "client " << i << " receiving message from server:\n" << recv << endl;
				clients[i].Send(recvmsg, strlen(recvmsg), 0);
			}
		} while (loop == true);
		if (i == temp)
		{
			int len = clients[i].Receive(recv, 500, 0);
			recv[len] = 0;
			if (strcmp(recv, "end") != 0) { i = 0; }
		}
	}
}

void server_hadler::stay()
{
}

void server_hadler::answer(int index)
{
	cout << "\nYour answer: ";
	gets_s(send);
	clients[index].Send(send, strlen(send), 0);
}

void server_hadler::endgame(int index)
{
	int len = clients[index].Receive(recv, 500, 0);
	recv[len] = 0;
	cout << recv << endl;
	close();
	exit(1);
}


void server_hadler::play()
{
}

void server_hadler::init()
{

	clients = new CSocket[nclient];
	AfxSocketInit();
	for (int i = 0; i < nclient; i++)
	{
		clients[i].Create();
		clients[i].Connect(_T("127.0.0.1"), 1234);
		cout << "Connection established!" << endl;
	}

	return;
}

void server_hadler::printrule()
{
	cout << "WELCOME TO THE MAGIC HAT" << endl;
	cout << "Description: This is a limited edition of the original magic hat game" << endl;
	cout << "Rules:";
	cout << "	+ Each player can guess as many times as they like if only they both spins a number greater than 0 and get it right." << endl;
	cout << "	+ Don't worry about spining the wheel cuz it automatically spins for you." << endl;
	cout << "	+ Each player looses their turn if they take the wrong guess or get 0 score in spining turn." << endl;
	cout << "	+ The winner will be one that has the most scores in their hands." << endl;
	cout << "Have fun (^_^)" << endl;
	return;
}

void server_hadler::signin()
{

	for (int i = 0; i < nclient; i++)
	{
		do
		{
			cout << "Please sign in to commence the battle" << endl;
			cout << "Enter your username here: "; gets_s(send);
			clients[i].Send(send, strlen(send), 0);

			int len = clients[i].Receive(recv, 500, 0);
			recv[len] = 0;
			cout << recv << endl;
		} while (strcmp(recv, "Your username is taken!") == 0);
	}
}
