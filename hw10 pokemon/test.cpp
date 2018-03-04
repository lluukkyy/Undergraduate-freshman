#include "pokemon.h"

int main() {
	map<string, vector<string> > facts1;
	facts1["label"] = vector<string>(1, "infernape");
	facts1["eggGroups"] = vector<string>(1, "pokemon");
	Infernape* x = new Infernape(facts1);
	map<string, vector<string> > facts2;
	facts2["label"] = vector<string>(1, "sealeo");
	facts2["eggGroups"] = vector<string>(1, "pokemon");
	Sealeo* y = new Sealeo(facts2);
	cout << x->getLabel()<<endl;
	cout << y->getLabel() << endl;
	cout << x->SharesEggGroup(y);
	delete x;
	delete y;
	return 0;
}