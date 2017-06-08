
#include <conio.h>
#include <ios>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <afxsock.h>
#include <sstream>
using namespace std;
//-------------------------------------------
// class de quan ly cau hoi
//-------------------------------------------
class CQuestion;
struct CCauhoi
{
	char cauhoi[100];
	char cautraloi[100];
	int stt;
};
typedef struct CCauhoi CCauhoi;

class CQuestion
{
private:
	int sl;
	CCauhoi *data;
public:
	CQuestion()
	{ 
		sl = 0;	
	}
	CQuestion(const CQuestion &a)
	{
		if (this != &a)
		{
			sl = a.sl;
			data = new CCauhoi[sl];
			for (int i = 0; i < sl; i++)
			{
				data[i].stt = a.data[i].stt;
				strcpy_s(data[i].cauhoi,100, a.data[i].cauhoi);
				strcpy_s(data[i].cautraloi, 100, a.data[i].cautraloi);
			}
		}
	}
	
	int getsl()
	{
		return sl;
	}
	CCauhoi* getcauhoi()
	{
		CCauhoi *temp;
		temp = new CCauhoi[sl];
		for (int i = 0; i < sl; i++)
		{
			temp[i].stt = data[i].stt;
			strcpy_s(temp[i].cauhoi, 100, data[i].cauhoi);
			strcpy_s(temp[i].cautraloi, 100, data[i].cautraloi);
		}
		return temp;
	}
	CCauhoi getcauhoii(int i)
	{
		CCauhoi temp;
		strcpy_s(temp.cauhoi,100,data[i].cauhoi);
		strcpy_s(temp.cautraloi, 100, data[i].cautraloi);
		temp.stt = data[i].stt;
		return temp;
	}


	//friend CCauhoi;
	friend ostream& operator << (ostream&o, CQuestion &a);
	friend fstream& operator >> (fstream &i, CQuestion&a);
	//CCauhoi getdatai(int n);
	
};
//-------------------------------------------
// lay cau hoi da soan trong file text
//-------------------------------------------
CQuestion read(char* name);

bool checkchar(char* x);

// tinh diem random cho player
int random();
//class player
class Splayer
{
public:
	char name[100];
	int point;	
	Splayer()
	{
		strcpy_s(name, 100, "NA");
		point = 0;
	}
};

class Cplayer
{
private:
	Splayer*player;	
	int n_player;
public:
	Cplayer()
	{
		//strcpy_s(player->name, 100, "NA");
		//player->point = 100;
		n_player = 0;
	}
	Cplayer(const Cplayer &x)
	{
		if (this != &x)
		{
			n_player = x.n_player;
			//p_player = x.p_player;
			player = new Splayer[n_player];
			for (int i = 0; i < n_player; i++)
			{
				strcpy_s(player[i].name,100, x.player[i].name);
			}
		}
	}

	bool checkname(char* name)
	{
			
		for (int i = 0; i < n_player;i++)
		{
			if (strcmp(player[i].name, name) == 0)
			{			
				return true;
			}
			
		} 
		return false;
	}
	char* getnamei(int i)
	{
		return player[i].name;
	}
	int getpointi(int i)
	{
		return player[i].point;
	}
	void setpointi(int i, int pts)
	{
		if (player[i].point != 0)
		{
			player[i].point = player[i].point + pts;
		}
		else
		{
			player[i].point = 0;
			player[i].point = player[i].point + pts;
		}
	}
	void setsl(int x)
	{
		n_player = x;
		player = new Splayer[n_player];
	}
	void setnamei(int i,char*x)
	{
		strcpy_s(player[i].name, 100, x);
	}
	int getsl()
	{
		return n_player;
	}
};
char* checkcautl(char* question, char* answer);
