#pragma once
#include "standard.h"

class magichat
{
	int rounds, maxscore;
	int reverse_ans;
	int reverse_quest;

	vector<char>usedwords;
	vector<string>ans;
	vector<string>question;

	string sentence;

	fstream file;
public:
	int scoreword(char word);
	int scoresentence(string sentence);
	int spin();
	int checkanswer(char word);
	int checkanswer(string sentence);
	int getnumberquestion();

	string ask();
	string gethint();

	void loadans(string name);
	void loadques(string name);
	void init();
};

