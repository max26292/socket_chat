#include "stdafx.h"
#include "magichat.h"


int magichat::scoreword(char word)
{
	int size = ans[reverse_ans].size();
	int score = 0;
	for (unsigned int i = 0; i < usedwords.size(); i++)
	{
		if (word == usedwords[i])
		{
			cout << "The word you guessed is already used" << endl;
			return 0;
		}
	}
	usedwords.push_back(word);
	for (int i = 0; i < size; i++)
	{
		if (word == ans[reverse_ans][i])
		{
			sentence[score] = word;
			score++;
		}
		if (sentence == ans[reverse_ans]) { --reverse_ans; }
	}
	return score;
}

int magichat::scoresentence(string sentence)
{
	if (sentence == ans[reverse_ans]) { return ans[reverse_ans].size(); }
	usedwords.clear();
	return 0;
}

int magichat::spin()
{
	default_random_engine generator((unsigned int)time(NULL));
	uniform_int_distribution<int>spin(0, 100);
	return spin(generator);
}

void magichat::loadans(string name)
{
	string line;
	file.open(name);
	if (file.fail()) { perror("Cannot open file!"); exit(1); }
	while (getline(file, line)) { ans.push_back(line); rounds++; }
	file.close();
}

void magichat::loadques(string name)
{
	string line;
	file.open(name);
	if (file.fail()) { perror("Cannot open file"); exit(1); }
	while (getline(file, line)) { question.push_back(line); }
	file.close();
}

int magichat::checkanswer(char word)
{
	int score = scoreword(word);
	return score;
}

int magichat::checkanswer(string sentence)
{
	int score = scoresentence(sentence);
	if (score == 0 && strlen(sentence.c_str()) == 1) { return checkanswer(sentence[0]); }
	else { --reverse_quest; --reverse_ans; return score; }
}

int magichat::getnumberquestion() {	return reverse_quest; }

void magichat::init()
{
	rounds = 0;
	loadans("listanswer.txt");
	loadques("listquestion.txt");
	reverse_ans = ans.size();
	reverse_quest = question.size();
	reverse_ans--; reverse_quest--;
}

string magichat::ask() { return question[reverse_quest]; }

string magichat::gethint()
{
	string hint;
	for (int i = 0; i < ans[reverse_ans].size(); i++) { hint.append("-"); }
	return hint;
}
