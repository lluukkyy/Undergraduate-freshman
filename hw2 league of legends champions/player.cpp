
#include "player.h"
player::player() { Count = 1; Kills = 0; Win = 0; Assists = 0; Deaths = 0; }
float kdr(player player1) {
	int x = player1.kills(); int y = player1.deaths();
	if (y == 0) { y = 1; };
	return float(x) / float(y);
}
float kda(player player1) {
	float kills = player1.kills(); 
	float assists = player1.assists(); 
	float deaths = player1.deaths();
	if (deaths == 0) { deaths = 1; }
	return (kills + assists)/deaths; 
}

bool hashigherkda(player player1, player player2) {
	if (kda(player1) > kda(player2)) {
		return 1;
	}
	else {
		if (kda(player1) == kda(player2))
		{
			if (player1.kills() > player2.kills()) { return 1; }
			if (player1.kills() == player2.kills()) {
				if (player1.deaths() < player2.deaths()) { return 1; }
				if (player1.deaths() == player2.deaths()) { 
					if (player1.assists() > player2.assists()) { return 1; }
					if (player1.assists() == player2.assists()
						&& player1.name() < player2.name())
					{ return 1; }
				}
			}
		}
	}
	return 0;
}

bool hashigherkdr(player player1, player player2){ 
	if (kdr(player1) > kdr(player2)) {
		return 1; 
	}
	else { 
		if (kdr(player1) == kdr(player2))
		{
			if (player1.kills() > player2.kills()) { return 1; }
			if (player1.kills() == player2.kills()) {
				if (player1.deaths() < player2.deaths()) { return 1; }
				if (player1.deaths() == player2.deaths()
				&& player1.name() < player2.name())
				{ return 1; }
				}
		}
	}
	return 0; 
}
bool playsmore(player player1, player player2)
{
	if (player1.count() > player2.count()) { return 1; }
	if (player1.count() == player2.count() && player1.name() < player2.name()) 
	{ return 1; }
	return 0;
}

bool winsmore(player player1, player player2)
{ if (player1.win() > player2.win()) { return 1; } return 0; }