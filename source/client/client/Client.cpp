// Client_Sum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "afxsock.h"
#include "stdio.h"
#include "conio.h"


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
			CSocket client;
			char s_str[1000];
			char r_str[1000];
			AfxSocketInit(NULL);
			client.Create();
			char name[100];
			int len;
			//int sum = 0; /*Tong diem*/
			//int diem = 0; /*Diem tung luot choi*/
			//bool flagq= true; //co 1 luot doan thoi 
			if(client.Connect(_T("127.0.0.1"),1234))
			{	
				cout << "Da ket noi voi server!!!" << endl;

				//dk ten
			dk:;
				cout << "Nhap ten nguoi choi muon dang ky:  " ;
				cin>>name;							
				client.Send(name,100,0);
				//printf("Da ket noi den server!");
				//char r_str[10],s_str[10];
				client.Receive(r_str,1000,0);				
				if (strcmp(r_str, "true")==0)
				{
					cout <<"Server: " <<"Ten dk da duoc chap nhan" << endl;
				}
				else if (strcmp(r_str, "false")==0)
				{
					cout <<"Server: "<< "Ten da ton tai vui long dk ten khac!!!" << endl;
					goto dk;
				}
				// cau hoi duoc gui qua tu server
				client.Receive(r_str, 1000, 0);
				cout <<"Server: " <<r_str << endl;
				strcpy_s(s_str, 100, "nhan");
				client.Send(s_str, 100, 0);
				client.Receive(r_str, 100, 0);
				cout << r_str << endl;
				int dodaich = atoi(r_str);
				cout << "Server: Cau tra loi co do dai " << dodaich << " chu cai." << endl;
				//dang chinh sua khuc nay
				//strcpy_s(s_str, 100, "nhan");
				//client.Send(s_str, 100, 0);
				//client.Receive(r_str, 100, 0);
				//doi diem tu server
			reset:;				
				cout << "Server: vui long doi toi luot ban....." << endl;
				//doi luot
				strcpy_s(s_str, 100, "waiting");
				client.Send(s_str, 100, 0);
				client.Receive(r_str, 100, 0);
				cout << "Server: " << r_str<<endl;
				client.Receive(r_str, 100, 0);

				int pts = atoi(r_str);
				cout << "Server: So diem khi doan dung: " << pts << endl;
				//quyet dinh tham gia luot hay ko?
				int luot;
				cout << "Server: Tham gia vong choi nay hay ko?\n (1 co, 2 khong)" << endl;
			
				cout <<name<< ": ";
				cin >> luot;
				while(luot < 1 || luot >2)
				{
					cout << "Server: Chi duoc chon 1 hoac 2!!!!!!!!!!" << endl;
					cout << name << ": ";
					cin >> luot;
				}
				if (luot == 1)
				{
					strcpy_s(s_str, "get");
					client.Send(s_str, 100, 0);
				}
				else
				{
					strcpy_s(s_str, "no");
					client.Send(s_str, 100, 0);
					goto reset;
				}


			
				//send tin ve server sau khi nhan diem
				//strcpy_s(s_str, 100, "Da nhan");
				//client.Send(s_str, 100, 0);
				//neu tham gia
				//client.Receive(r_str, 100, 0);
				//cout << r_str << endl;
				char tl[10];
			point:;
				cout << "Server: " << "Nhap ky tu muon doan: " << endl;
				cout << name << ": ";
				fflush(stdin);

				gets_s(tl);
				int c = strlen(tl);
				if (c > 1){
					cout << "Server: Sai, vui long nhap 1 ky tu moi lan doan." <<endl;
					goto point;
				}				
				else
				{
					strcpy_s(s_str, 100, tl);
					client.Send(s_str, 100, 0);

				}
				client.Receive(r_str, 100, 0);
				cout << r_str << endl;
				//client.Receive(r_str, 100, 0);
				//cout << r_str << endl;
				dodaich--;
				if (dodaich == 0){
					goto end;
				}
				else
					goto reset;
			end:;
				strcpy_s(s_str, 100, "f");
				client.Send(s_str, 100, 0);
				client.Receive(r_str, 100, 0);
				int dd = atoi(r_str);
				cout << endl;
				cout << "Tong diem sau luot choi :" << dd << endl;
				client.Close();				
			}
			else
				cout << "Khong the ket noi den server!" << endl;
			getch();
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
