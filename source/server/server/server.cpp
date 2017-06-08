// server.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "server.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.

			CSocket server;
			CSocket *client;
			char s_str[1000];
			char r_str[1000];
			AfxSocketInit(NULL);

			//banner


			cout << "------------------------" << endl;
			cout << "TRO CHOI: CHIEC NON KY DIEU " << endl;
			cout << "------------------------" << endl;

			//create server
			server.Create(1234);
			server.Listen();
			//so luong nguoi choi
			int n_client;
			//int len;
			Cplayer member;




			cout << "Nhap so luong nguoi choi: ";
			cin >> n_client;
			member.setsl(n_client); // dua sl nguoi choi vao class
			cout << endl;
			cout << "Dang cho nguoi choi..." << endl;
			client = new CSocket[n_client];


			//Danh sach ten nguoi choi			
			//char **name_client;
			//name_client = new char *[n_client];
			for (int i = 0; i < n_client; i++)
			{
				if (server.Accept(client[i]))
				{
					cout << "Da ket noi voi client " << i + 1 << "!!!!!" << endl;
					}
			}
			for (int i = 0; i < n_client; i++)
			{
					char name[100];
				checkname:;
					client[i].Receive(r_str, 100, 0); // nhan ten tu client
					//len = strlen(r_str);
					//r_str[len] = '\n';
					strcpy_s(name, 100, r_str);
					if (member.checkname(name) == true) //check trung
					{
						strcpy_s(s_str, 100, "false");
						//cout << s_str << endl; //check kq gui di
						client[i].Send(s_str, 100, 0);
						goto checkname;
					}
					else if (member.checkname(name) == false)
					{
						member.setnamei(i, name);
						cout << "Ten client " << i + 1 << "da dang ky: " << member.getnamei(i) << endl;
						strcpy_s(s_str, 100, "true");
						//cout << s_str << endl;//check kq dc gui dio
						client[i].Send(s_str, 100, 0);
					}
			}
					//get cau hoitu file
					CQuestion question;
					question = read("cauhoi.txt");
					//cout << question;

					//server chon cau hoi
					int cauhoii;
					cout << "chon cau hoi muon su dung (1-" << (question.getsl()) << "): ";
					cin >> cauhoii;
					CCauhoi cauhoi;
					cauhoi = question.getcauhoii(cauhoii-1);
					cout << "Cau hoi se su dung: " << cauhoi.cauhoi << endl;
					strcpy_s(s_str, 100, cauhoi.cauhoi);
					int *temppts;
					temppts = new int[n_client];
					
					for (int i = 0; i < n_client; i++)
					{
						client[i].Send(s_str, 100, 0);
						client[i].Receive(r_str, 100, 0);
					}
					char cautl[100];
					strcpy_s(cautl, 100, cauhoi.cautraloi);
					int dodai = strlen(cautl);
					itoa(dodai, s_str, 10);
					for (int i = 0; i < n_client; i++)
					{
						client[i].Send(s_str, 100, 0);
						
					}
					//client.Send(s_str, 100, 0);
					//dang chinh sua khuc nay 
					
					
					/*
						client[i].Receive(r_str, 100, 0);
						//cout << r_str << endl;
						if (strcmp(r_str, "get") == 0)
						{
							temppts[i] = random();
							//cout << temppts << endl;
							char tempc[10];
							itoa(temppts[i], tempc, 10);
							cout << tempc << endl;;
							strcpy_s(s_str, 100, tempc);
							client[i].Send(s_str, 100, 0);
						}
						else
						{
							temppts[i] = 0;
							continue;
						}
							
					}
					*/
					for (int i = 0; i < strlen(cauhoi.cautraloi); i++)
					{
						
						for (int j = 0; j < member.getsl(); j++)
						{
							//bao luot cho client
							client[j].Receive(r_str, 100, 0);
							cout << member.getnamei(j) << ": " << r_str << "....."<< endl;
							strcpy(s_str, "Den luot ban doan: ");
							client[j].Send(s_str, 100, 0);
							//tinh diem cho client
							temppts[j] = random();
							//cout << temppts << endl;
							char tempc[10];
							itoa(temppts[j], tempc, 10);
							//cout << tempc << endl;;
							strcpy_s(s_str, 100, tempc);
							client[j].Send(s_str, 100, 0);
							//nhan tin tu client sau khi nhan diem
							client[j].Receive(r_str, 100, 0);
							if (strcmp(r_str, "get") == 0)
							{
								client[j].Receive(r_str, 100, 0);
								cout << member.getnamei(j) << ": " << r_str<<endl;
								char check[100];
								//cout << checkcautl(cauhoi.cautraloi, r_str) << endl;
								strcpy_s(check, 100, checkcautl(cauhoi.cautraloi, r_str));
								//cout << check << endl;

								if (strcmp(check, "NA") == 0)
								{
									strcpy(s_str, "Sai");
									client[j].Send(s_str, 100, 0);
								}
								else
								{
									strcpy_s(s_str, 100, check);
									client[j].Send(s_str, 100, 0);
									member.setpointi(j, temppts[j]);
								}
							}
							else
							{
								temppts[i] = 0;
								continue;
							}
						}
					}
					/// tuyong duoi voi khuc chinh sua ben client
				
					for (int i = 0; i < n_client; i++)
					{
						client[i].Receive(r_str, 100, 0);
						//cout << r_str;
						int diem = member.getpointi(i);
						itoa(diem, s_str, 10);
						//strcpy_s(s_str, 100, "end");
					    client[i].Send(s_str, 100, 0);
					}
				
				


			
			//Dong tat ca client
			for (int j = 0; j < n_client; j++){
				client[j].Close();
			}
			//Dong server
			server.Close();
		
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}
