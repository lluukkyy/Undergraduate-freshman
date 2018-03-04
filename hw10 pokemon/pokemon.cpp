#include "pokemon.h"
string Pokemon::getLabel() { return get_facts()["label"][0]; }

map<string, vector<string> > Pokemon::get_facts() { return facts; }

bool Pokemon::SharesEggGroup(Pokemon*const& a) {
	vector<string> eggGroups1 = get_facts()["eggGroups"];
	vector<string> eggGroups2 = a->get_facts()["eggGroups"];
	for (unsigned int i = 0; i < eggGroups1.size(); i++) {
		for (unsigned int j = 0; j < eggGroups2.size(); j++) {
			if (eggGroups1[i] == eggGroups2[j]) return 1;
		}
	}
	return 0;
}

bool Pokemon::hasEggGroup(string eggGroup) {
	vector<string> eggGroups = get_facts()["eggGroups"];
	for (unsigned int i = 0; i < eggGroups.size(); i++) {
		if (eggGroups[i] == eggGroup) return 1;
	}
	return 0;
}