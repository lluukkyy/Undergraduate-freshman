#ifndef __player_h__
#define __player_h__
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class player {
public:
	int &kills() { return Kills; }
	int &deaths() { return Deaths; }
	string &name() { return Name; }
	vector<string> &champions() { return Champions; }
	string &champion() { return Champion; }
	int &count() { return Count; }
	int &win() { return Win; }
	int &assists() { return Assists; }
	player();
private:
	string Champion;
	int Kills;
	int Deaths;
	string Name;
	vector<string> Champions;
	int Win;
	int Count;
	int Assists;
};

bool hashigherkdr(player player1, player player2);
bool hashigherkda(player player1, player player2);
bool playsmore(player player1, player player2);
bool winsmore(player player1, player player2);
float kdr(player player1);
float kda(player player1);
#endif // !__player_h__


