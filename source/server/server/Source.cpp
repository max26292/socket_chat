#include "stdafx.h"
/*
CCauhoi::CCauhoi()
{
	strcpy(cauhoi, "NA");
	strcpy(cautraloi, "NA");
	stt = 0;
}
CCauhoi::~CCauhoi()
{
}

CCauhoi CQuestion::getdatai(int i)
{
	return data[i];
}
//operator nhap CCauhoi
istream& operator >> (istream&i, CCauhoi&a)
{
	return i;
}

//operator  doc file CCauhoi
fstream& operator >>(fstream&i, CCauhoi &a)
{
	i >> a.stt;
	i << endl;
	i.getline(a.cauhoi,100);
	i << endl;
	i.getline(a.cautraloi, 100);
	i << endl;
	return i;
}
*/



//operator doc file CQuestion
fstream& operator >>(fstream&i, CQuestion&a)
{
	i>>a.sl;
	a.data = new CCauhoi[a.sl];
	for (int j = 0; j < a.sl; j++)
	{
		
		i >> a.data[j].stt;
		i.ignore(1);		
		i.getline(a.data[j].cauhoi, 50,'\n');	
		i.getline(a.data[j].cautraloi, 100, '\n');
		
	}
	return i;
}


//---------------------------------
// -------Operator nhap xuat ----- 
ostream& operator << (ostream&o, CQuestion &a)
{
	cout << "So luong cau hoi: " << a.sl << endl;
	cout << "Cac cau hoi da luu tru:" << endl;
	for (int i = 0; i < a.sl; i++)
	{
		cout << "Stt: " << a.data[i].stt << endl;
		cout << a.data[i].cauhoi << endl;
		cout << a.data[i].cautraloi << endl;
	}
	return o;
}

//Ham doc file
CQuestion read(char*name)
{
	fstream pRead;
	pRead.open(name, ios::in);
	CQuestion a;
	if (!pRead)
	{
		cout << "Cant not open file or file isn't exist";
		return a;
	}	
	pRead >> a;
	//cout << a;
	return a;
}
//ham random diem
int random()
{
	
		int x = rand() % 100 + 1;
		return x;
	
}

// kiem tra cau hoi dung hay sai va tra lai vi tri
char* checkcautl(char* question, char* answer)
{
	int* vitri;
	answer = strupr(answer);
	int len = strlen(question);
	//len++;
	vitri = new int[len+1];
	int count=0;
	int j = 0;
	for (int i = 0; i < len; i++)
	{
		if (answer[0] == question[i])
		{
			count++;
			vitri[j] = i;
			j++;
		}
	}
	char result[100];
	if (count != 0)
	{
		ostringstream temp;
		temp << "Co " << count << " chu cai " << answer << " tai vi tri ";
		for (int i = 0; i < j; i++)
		{
			temp << vitri[i]+1 << " ";
		}
		temp << endl;
		string str;
		str = temp.str();
		const char* tp = str.c_str();
		strcpy_s(result, 100, tp);
		return result;
	}
	else
	{
		strcpy_s(result, 100, "NA");
	}
	return result;
}