#include <stdlib.h>
#include "hash_table.h"
using namespace std;

//this is the hash function
int hash_table::hash(string key) {
	// Implemented from:
	// http://www.partow.net/programming/hashfunctions/
	int hash = 1315423911;
	for (unsigned int i = 0; i < key.length(); i++) {
		hash ^= ((hash << 5) + key[i] + (hash >> 2));
	}
	return hash;
}

//add a certain key 
void hash_table::add(string key, int position) {
	int hash_value = hash(key) % table.size();
	int deviation = 0;
	int temp;
	//step into a loop to check deviations of the hash value
	while (deviation<table.size()) {
		temp = (hash_value + deviation) % table.size();
		//if the deviation spot is empty or stores a same key
		//break the loop
		if (table[temp].first == key || table[temp].first == "") {
			
			if (table[temp].first == "") {
				table[temp].first = key;
				occupied++;
			}
			break;
		}
		deviation++;
	}
	table[temp].second.push_back(position);
	if (get_occupancy() > max_o) expand(2 * table.size());
}

//expand the table if occupancy is over the max value
void hash_table::expand(int size) {
	//create a new table
	vector<pair<string, vector<int> > > new_table =
		vector<pair<string, vector<int> > >
		(size, make_pair("", vector<int>()));
	//put everything in the previous table into the new one
	for (int i = 0; i < table.size(); i++) {
		if (table[i].first != "") {
			int hash_value = hash(table[i].first) % new_table.size();
			int deviation = 0;
			int temp;
			//since the size is different, the deviation of collisions should
			// be remade
			while (deviation < new_table.size()) {
				temp = (hash_value + deviation) % new_table.size();
				if (new_table[temp].first == "") {
					new_table[temp].first = table[i].first;
					new_table[temp].second = table[i].second;
					break;
				}
				deviation++;
			}
		}
	}
	table = new_table;
}

//read through the string to create a complete table
void read(hash_table& table, string genome) {
	//to make sure there's a complete kmer
	if (genome.size() >= table.get_kmer()) {
		//notice the largest value of i is size - length of kmer
		for (int i = 0; i < genome.size() - table.get_kmer(); i++) {
			//cut the string by a start position and kmer length
			string temp = "";
			for (int j = 0; j < table.get_kmer(); j++) temp += genome[j + i];
			table.add(temp, i);
		}
	}
}

//search for a specific key to see if it's in the genome
vector<int> hash_table::search(string key) {
	vector<int> positions;
	int hash_value = hash(key) % table.size();
	int deviation = 0;
	int temp;
	//step into a loop
	while (deviation < table.size()) {
		temp = (hash_value + deviation) % table.size();
		//if deviation of hash_value matches the correct key
		if (table[temp].first == key) {
			positions = table[temp].second;
			break;
		}
		//if there's no wrong key but a empty spot
		if (table[temp].first == "") break;
		deviation++;
	}
	return positions;
}

//search for a certain piece
void query(string word, hash_table& table, string& genome, int mismatch) {
	bool found = 0;
	string temp = "";
	for (int i = 0; i < table.get_kmer(); i++) temp += word[i];
	cout << "Query: " << word << endl;
	vector<int> positions = table.search(temp);
	//go to every recorded position to check if it matches
	for (int i = 0; i < positions.size(); i++) {
		int count = 0;// count mismatches
		int length = table.get_kmer();
		//check every character in the piece  
		for (int j = positions[i] + table.get_kmer(); j < genome.size(); j++) {
			//length starts with the length of kmer and ends with the length of key word
			if (length >= word.size()) break;
			if (genome[j] != word[length]) count++;
			length++;
		}
		//if number of mismatches is in bound 
		if (count <= mismatch) {
			found = 1;
			//print out the original piece of the genome
			string original = "";
			for (int x = 0; x < word.size(); x++)
				original += genome[x + positions[i]];
			cout << positions[i] << " " << count << " " << original << endl;
		}
	}
	if (!found) cout << "No Match" << endl;
}

int main() {
	string genome = "";
	int table_size ;
	float occupancy;
	int kmer;
	int mismatch;
	string temp;
	hash_table table(100);
	while (cin >> temp) {
		if (temp == "genome") {
			//read the target txt file name and convert it to char*
			cin >> temp;
			ifstream input_genome(temp.c_str());
			string temp2;
			//read genome file, create a string of genome
			while (input_genome >> temp2) {
				for (int i = 0; i < temp2.size(); i++) genome += temp2[i];
			}
		}
		else if (temp == "table_size") {
			cin >> temp;
			table_size = atoi(temp.c_str());
			table.expand(table_size);
		}
		else if (temp == "occupancy") {
			cin >> occupancy;
			table.set_max_o(occupancy);
			if (table.get_occupancy() > occupancy) table.expand(2 * table.get_size());
		}
		else if (temp == "kmer") {
			cin >> kmer;
			table.set_kmer(kmer);
			read(table, genome);
		}
		else if (temp == "query") {
			cin >> mismatch;
			cin >> temp;
			query(temp,table, genome, mismatch);
		}
		else if (temp == "quit") break;
	}
	return 0;
}