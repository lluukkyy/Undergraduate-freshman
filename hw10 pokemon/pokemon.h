#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;
//================================================the base class=====================================================
class Pokemon {
public:
	Pokemon(map<string, vector<string> > input) { facts = input; }//constructor
	virtual  ~Pokemon(){}//destructor

	map<string, vector<string> > get_facts();//ruturn the private facts

	string getLabel();//return the label

	bool SharesEggGroup(Pokemon* const& a);

	//used to check if the eggGroups vector has a specific eggGroup
	bool hasEggGroup(string eggGroup);
private:
	map<string, vector<string> > facts;
};
//===================================================================================================================
//=================================================egg groups========================================================
//-------------------used in this homework--------------------
class Monster : public Pokemon {
public:
	Monster(map<string, vector<string> > input):Pokemon(input){
		if (!hasEggGroup("Monster")) throw 1;
	}
};

class Grass : public Pokemon {
public:
	Grass(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Grass")) throw 1;
	}
};

class HumanLike : virtual public Pokemon {
public:
	HumanLike(map<string, vector<string> > input) : Pokemon(input) {
		if (!hasEggGroup("HumanLike")) throw 1;
	}
};

class Field : virtual public Pokemon {
public:
	Field(map<string, vector<string> > input) : Pokemon(input) {
		if (!hasEggGroup("Field")) throw 1;
	}
};

class Water1 : virtual public Pokemon {
public:
	Water1(map<string, vector<string> > input) : Pokemon(input) {
		if (!hasEggGroup("Water1")) throw 1;
	}
};
//------------------------------------------------------------------
class Water2 : public Pokemon {
public:
	Water2(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Water2")) throw 1;
	}
};

class Water3 : public Pokemon {
public:
	Water3(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Water3")) throw 1;
	}
};

class Ditto : public Pokemon {
public:
	Ditto(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Ditto")) throw 1;
	}
};

class Undiscovered : public Pokemon {
public:
	Undiscovered(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Undiscovered")) throw 1;
	}
};

class Mineral : public Pokemon {
public:
	Mineral(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Mineral")) throw 1;
	}
};

class Flying : public Pokemon {
public:
	Flying(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Flying")) throw 1;
	}
};

class Bug : public Pokemon {
public:
	Bug(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Bug")) throw 1;
	}
};

class Fairy : public Pokemon {
public:
	Fairy(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Fairy")) throw 1;
	}
};

class Dragon : public Pokemon {
public:
	Dragon(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Dragon")) throw 1;
	}
};

class Amorphous : public Pokemon {
public:
	Amorphous(map<string, vector<string> > input) :Pokemon(input) {
		if (!hasEggGroup("Amorphous")) throw 1;
	}
};
//==========================================================================================================
//=======================================pokemons and evolutions============================================
//------------------------------------------------------------------
class Sunkern :public Grass {
public:
	Sunkern(map<string, vector<string> > input) : Grass(input) {
		if (!((get_facts()["bodyStyle"][0]=="Pokemon consisting only of a head"&&
			get_facts()["height"][0]=="0.300000")||
			(get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailless form"&&
				get_facts()["height"][0] == "0.800000"))) throw 1;
	}
};

class Sunflora :public Sunkern {
public:
	Sunflora(map<string, vector<string> > input) : Sunkern(input) {
		if (!(get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailless form"&&
			get_facts()["height"][0] == "0.800000")) throw 1;
	}
};
//-----------------------------------------------------------------

class Amaura :public Monster {
public:
	Amaura(map<string, vector<string> > input) : Monster(input){
		if (!((get_facts()["bodyStyle"][0] == "Pokemon with a quadruped body"&&
			get_facts()["height"][0] == "1.300000") ||
			(get_facts()["bodyStyle"][0] == "Pokemon with a quadruped body"&&
				get_facts()["height"][0] == "2.700000"))) throw 1;
	}
};

class Aurorus : public Amaura {
public:
	Aurorus(map<string, vector<string> > input):Amaura(input) {
		if (!(get_facts()["bodyStyle"][0] == "Pokemon with a quadruped body"&&
			get_facts()["height"][0] == "2.700000")) throw 1;
	}
};
//-----------------------------------------------------------------

class Chimchar :public HumanLike,public Field {
public:
	Chimchar(map<string, vector<string> > input) : 
		Pokemon(input),HumanLike(input),Field(input) {
		if (!((get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailed form"&&
			get_facts()["height"][0] == "0.500000") ||
			(get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailed form"&&
				get_facts()["height"][0] == "0.900000")||
				(get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailed form"&&
					get_facts()["height"][0] == "1.200000"))) throw 1;
	}
};

class Monferno : public Chimchar {
public:
	Monferno(map<string, vector<string> > input) :Pokemon(input),Chimchar(input) {
		if (!((get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailed form"&&
			get_facts()["height"][0] == "0.900000") ||
			(get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailed form"&&
				get_facts()["height"][0] == "1.200000"))) throw 1;
	}
};

class Infernape : public Monferno {
public:
	Infernape(map<string, vector<string> > input) :Pokemon(input),Monferno(input) {
		if (!(get_facts()["bodyStyle"][0] == "Pokemon with a bipedal tailed form"&&
			get_facts()["height"][0] == "1.200000")) throw 1;
	}
};
//-----------------------------------------------------------------
class Spheal :public Field,public Water1 {
public:
	Spheal(map<string, vector<string> > input) : 
		Pokemon(input),Field(input),Water1(input) {
		if (!((get_facts()["bodyStyle"][0] == "Pokemon with fins"&&
			get_facts()["height"][0] == "0.800000") ||
			(get_facts()["bodyStyle"][0] == "Pokemon with fins"&&
				get_facts()["height"][0] == "1.100000") ||
				(get_facts()["bodyStyle"][0] == "Pokemon with a quadruped body"&&
					get_facts()["height"][0] == "1.400000"))) throw 1;
	}
};

class Sealeo : public Spheal {
public:
	Sealeo(map<string, vector<string> > input) :Pokemon(input), Spheal(input) {
		if (!((get_facts()["bodyStyle"][0] == "Pokemon with fins"&&
			get_facts()["height"][0] == "1.100000") ||
			(get_facts()["bodyStyle"][0] == "Pokemon with a quadruped body"&&
				get_facts()["height"][0] == "1.400000"))) throw 1;
	}
};

class Walrein : public Sealeo {
public:
	Walrein(map<string, vector<string> > input) :Pokemon(input), Sealeo(input) {
		if (!(get_facts()["bodyStyle"][0] == "Pokemon with a quadruped body"&&
			get_facts()["height"][0] == "1.400000")) throw 1;
	}
};
