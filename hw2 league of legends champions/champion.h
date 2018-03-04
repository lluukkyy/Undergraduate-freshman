#ifndef __champion_h__
#define __champion_h__
#include <iostream>
#include <string>
using namespace std;

class champion {
public:
	string &name() { return Name; }
	int &count() { return Count; }
	int &win() { return Win; }
	int &minion_kills() { return Minion_kills; }
	champion();
private:
	int Minion_kills;
	string Name;
	int Count;
	int Win;
};
float wr(champion champion1);
bool hashigherwr(champion champion1, champion champion2);
#endif // !__player_h__#pragma once
