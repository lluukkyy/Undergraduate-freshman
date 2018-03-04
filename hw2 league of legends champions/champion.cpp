#include "champion.h"
champion::champion() { Count = 1; Minion_kills = 0; }
float wr(champion champion1) {
	return  float(champion1.win()) / float(champion1.count());
}

bool hashigherwr(champion champion1, champion champion2)
{ 
	if (wr(champion1) > wr(champion2)) 
	{ return 1; }
	else{
		if (wr(champion1) == wr(champion2)) {
			if (champion1.win() > champion2.win()) { return 1; }
			if (champion1.win() == champion2.win()
				&& champion1.name() < champion2.name())
			{ return 1; }
		}
	}
	return 0; 
}
