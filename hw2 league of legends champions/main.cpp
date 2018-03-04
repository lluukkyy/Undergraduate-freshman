#include "player.h"
#include "champion.h"
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <iomanip>
vector<string> match_id;
vector<player> all_players; vector<player> players;
vector<champion> all_champions; vector<champion> champions;

void statistic() {
//this function is used to cross out multi-records of a same players in the list and
//accumulate other useful data
	for (unsigned int i = 0; i < all_champions.size(); i++) {
		bool found = 0;
		for (unsigned int j = 0; j < champions.size(); j++) {
			if (all_champions[i].name() == champions[j].name())
			{
				found = 1;
				champions[j].win() += all_champions[i].win(); 
				champions[j].minion_kills() += all_champions[i].minion_kills();
				champions[j].count()++; break;
			}
		}
		if (found == 0) { champions.push_back(all_champions[i]); }
	}
	for (unsigned int i = 0; i < all_players.size(); i++) {
		bool found_player = 0; bool found_champion = 0;
		for (unsigned int j = 0; j < players.size(); j++) {
			if (all_players[i].name() == players[j].name()) {
				found_player = 1;
				players[j].win() += all_players[i].win();
				players[j].kills() += all_players[i].kills();
				players[j].deaths() += all_players[i].deaths();
				players[j].count()++;
				for (unsigned int u = 0; u < players[j].champions().size(); u++) {
					if (players[j].champions()[u] == all_players[i].champion())
					{
						found_champion = 1; break;
					}
				}
				if (found_champion == 0)
				{ players[j].champions().push_back(all_players[i].champion()); }
				break;
			}
		}
		if (found_player == 0) { players.push_back(all_players[i]); }
	}
}

void read_win_lose(ifstream &file, string &read,int win) {
	//record the winning and losing info for a match
	file >> read; 
	if (read == "WINNING") { win = 1; }
	if (read == "LOSING") { win = 0; }
	file >> read;
	for (unsigned int i = 0; i < 5; i++) {
		player temp_player; champion temp_champion;
		file >> read;
		temp_player.name() = read;
		for (unsigned int i = 0; i < 3; i++) { file >> read; }
		temp_champion.name() = read;
		temp_champion.win() = win;
		all_champions.push_back(temp_champion);
		temp_player.champion() = read;
		temp_player.champions().push_back(read);
		temp_player.win() = win;
		all_players.push_back(temp_player);
	}
}

void Event(ifstream &file, string &read) {
	while (1) {
		file >> read;
		if (read != "@") { break; }
		if (read == "@") { 
			for (unsigned int i = 0; i < 2; i++) { file >> read; }
			for (unsigned int i = 0; i < all_players.size(); i++) { 
				if (all_players[i].name() == read) 
				{ all_players[i].kills() += 1;  break;}
			}
			if (read =="minion") {
				
				for (unsigned int i = 0; i < 2; i++) { file >> read; }
				for (unsigned int i = 0; i < all_players.size(); i++) {
					if (all_players[i].name() == read) {
						all_players[i].deaths() += 1;
						for (unsigned int j = 0; j < all_champions.size(); j++) {
							if (all_champions[j].name() == all_players[i].champion())
							{ all_champions[j].minion_kills() += 1; break;}
						}
						break;
					}
				}
			}
			else {
				file >> read;
				if (read == "[") {
					while (1) {
						file >> read;
						if (read == "]") { break; }
						for (unsigned int i = 0; i < all_players.size(); i++) {
							if (all_players[i].name() == read)
							{
								all_players[i].assists() += 1;  break;
							}
						}
					}
					file >> read;
				}
				file >> read;
				for (unsigned int i = 0; i < all_players.size(); i++) {
					if (all_players[i].name() == read)
					{
						all_players[i].deaths() += 1;  break;
					}
				}
			}
		}	
		
	}
}

void read_game_record(char* filename) {
	ifstream file(filename); string read; 
	while (file >> read) {
		//if reads MATCH, record match id and operate read_win_lose() function
		if (read == "MATCH") {
			for (unsigned int i = 0; i < 2; i++) { file >> read;}
			match_id.push_back(read);
			read_win_lose(file,read,1);
			read_win_lose(file,read,0);
		}
		//read event info with Event() function
		if (read == "EVENTS") { 
			Event(file,read); 
		//once done with Event(), run statistic() immediately! This is so important
		//that the amount of calculation will be hugely reduced, compared to one
		//calculating after all records are read.
			statistic();
			all_champions.clear(); all_players.clear();
		}
	}
}

bool hassmallername(string name1, string name2) { if (name1 < name2)
{ return 1; }return 0; }

void print_players(char* filename) {
//this function prints players
	ofstream output(filename);
	if (output.is_open()) {
		sort(players.begin(), players.end(), hashigherkdr);
		output << setw(23) << left << "PLAYER NAME" << setw(5) << right << "KILLS";
		output << setw(8) << right << "DEATHS" << setw(8) << right << "KDR";
		output<< "  PLAYED WITH CHAMPION(S)" << "\n";
		for (unsigned int i = 0; i < players.size(); i++) {
			output << setw(23) << left << players[i].name() << setw(5) << right;
			output<< players[i].kills() << setw(8) << right << players[i].deaths();
			output << setw(8) << right << fixed << setprecision(2);
			output<< kdr(players[i]);
			output << "  ";
			sort(players[i].champions().begin(), 
				players[i].champions().end(), hassmallername);
			for (unsigned int j = 0; j < players[i].champions().size(); j++) {
				if (j >= 1) { output << ", "; }
				output << players[i].champions()[j]; 
			}
			output << "\n";
		}
		output.close();
	}
	
}

void print_champions(char* filename) {
// this function prints champion;
	ofstream output(filename);
	if (output.is_open()) {
		sort(champions.begin(), champions.end(), hashigherwr);
		output << setw(24) << left << "CHAMPION NAME" << setw(4) << right << "WINS";
		output << setw(8) << right << "LOSSES" << setw(8) << right << "WIN%";
		output << setw(16) << right << "MINION DEATHS" << "\n";
		for (unsigned int i = 0; i < champions.size(); i++) {
			output << setw(24) << left << champions[i].name() << setw(4) << right;
			output << champions[i].win() << setw(8) << right;
			output<< champions[i].count() - champions[i].win();
			output << setw(8) << right << fixed << setprecision(2);
			output<< float(champions[i].win()) / float(champions[i].count());
			output << setw(16) << right << champions[i].minion_kills() << "\n";
		}
		output.close();
	}
		
}

void chart(vector<float> list,vector<string> names,char* filename,string name) {
//chart() is a function used in custom(),which prints out a table of 10-ranked datas.
	ofstream output;
	output.open(filename, ios_base::app);
	if (output.is_open()) {
		output << "--------------------------------" << name;
		output<<"-------------------------------" << "\n";
		if (list[0] - list[list.size() - 1] >= 10) {
			output << "Collect all the data into 10 ranks(sorted by value):" << "\n";
			vector<vector<int> > numbers;  float gap; vector<int> temp;
			gap = (list[0] - list[list.size() - 1]) / 10;
			for (unsigned int i = 0; i < 10; i++) { numbers.push_back(temp); }
			for (unsigned int i = 0; i < list.size(); i++) {
				if (list[i] == list[list.size() - 1]) { numbers[0].push_back(i); }
				else {
					int index = int((list[i] - 0.0001 - list[list.size() - 1]) / gap);
					numbers[index].push_back(i);
				}
			}
			for (unsigned int i = 0; i < numbers.size(); i++) {
				output << "Rank " << setw(2) << right << i + 1;
				if (numbers[9 - i].size() > 0) {
					output << " ( ";
					output << setw(7) << right << list[numbers[9 - i][0]] << " -> ";
					output << setw(7) << right << list[numbers[9 - i][numbers[9 - i].size() - 1]];
					output << " )" << ": " << setw(10) << right << numbers[9 - i].size();
					if (numbers[9 - i].size() <= 10) {
						output << "   ";
						for (unsigned int j = 0; j < numbers[9 - i].size(); j++) {
							output << " " << names[numbers[9 - i][j]];
							if (j < numbers[9 - i].size() - 1) { output << ","; }
						}
					}
					else { output << "    Too many to print."; }
					output << "\n";
				}
				else { output << setw(24) << right << " Nothing in this rank" << "\n"; }

			}
		}
		else { output << "\n" << "Not enough range of data to be ranked." << "\n"; }
		output << "\n";
		output.close();
	}
}

void custom(char* filename) {
	/*
	custom() collects data and divide them into 10 ranks in terms of value.
	for example: 100,95,82,80,75,1. These six values will give out the result of:
	Rank 1 :2 100, 95
	Rank 2 :1 82
	Rank 3 :1 80,75
	Rank 4 Nothing in this rank
	Rank 5 Nothing in this rank
	.
	.
	Rank 10:1
	
	and then print out the average of these data and top 10 data.
	*/
	float KDA = 0; float games = 0; vector<float> list; vector<string> names;
	ofstream output;
	output.open(filename); output.close();
	//------------------------number-of-games------------------------------------
	sort(players.begin(), players.end(), playsmore);
	for (unsigned int i = 0; i < players.size(); i++)
	{
		games += players[i].count();
		list.push_back(players[i].count());
		names.push_back(players[i].name());
	}
	games = games / players.size();
	chart(list, names, filename,"number-of-games");
	output.open(filename,ios_base::app);
	if (output.is_open()) {
		output << "Average number of games played by a player: " << games << "\n" ;
		output << "\n" << "10 players who play most:" << "\n";
		for (unsigned int i = 0; i < 10; i++)
		{
			output << setw(24) << left << players[i].name() << setw(5) << right;
			output << players[i].count() << "\n";
		}	
		output << "Least: " << players[players.size() - 1].count() << "\n";
		output.close();
	}
//------------------------kda----------------------------------------------------
	list.clear(); names.clear();
	sort(players.begin(), players.end(), hashigherkda);
	for (unsigned int i = 0; i < players.size(); i++)
	{
		KDA += kda(players[i]);
		list.push_back(kda(players[i]));
		names.push_back(players[i].name());
	}
	KDA = KDA / players.size();
	chart(list, names, filename, "kda");
	output.open(filename, ios_base::app);
	if (output.is_open()) {
		output << "Average KDA of a player is " << KDA << "\n";
		output << "\n" << "10 players of highest KDA:" << "\n";
		for (unsigned int i = 0; i < 10; i++)
		{
			output << setw(24) << left << players[i].name() << setw(5) << right;
			output << kda(players[i]) << "\n";
		}
		output << "Lowest: " << kda(players[players.size() - 1]) << "\n";
		output.close();
	}
//-----------------------------champion-winning-rate--------------------------------
	list.clear(); names.clear(); float WR = 0;
	sort(champions.begin(), champions.end(), hashigherwr);
	for (unsigned int i = 0; i < champions.size(); i++)
	{
		WR += wr(champions[i]);
		list.push_back(wr(champions[i]) * 100);
		names.push_back(champions[i].name());
	}
	WR = WR / champions.size();
	chart(list, names, filename,"champion-winning-rate");
	output.open(filename, ios_base::app);
	if (output.is_open()) {
		output << "Average winning rate of a champion is " << WR * 100 << "%" << "\n" ;
		output << "\n" << "10 champions of highest winning rate:" << "\n";
		for (unsigned int i = 0; i < 10; i++)
		{
			output << setw(24) << left << champions[i].name() << setw(5) << right;
			output << setprecision(2) << wr(champions[i]) << "\n";
		}
		output << "Lowest: " << wr(champions[champions.size() - 1]) << "\n";
		output.close();
	}
}

int main(int argc,char* argv[]) {
	read_game_record(argv[1]); //this function reads the game records
	if (string(argv[3]) == "players") { print_players(argv[2]); } 
	if (string(argv[3]) == "champions") { print_champions(argv[2]); }
	if (string(argv[3]) == "custom") { custom(argv[2]); }
	return 0;
} 