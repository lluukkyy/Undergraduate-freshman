#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

class hash_table {
public:
	//constructor
	hash_table(int size) {
		occupied = 0; 
		kmer = 0;
		max_o = 0.5;
		table = vector<pair<string, vector<int> > >(size, make_pair("",vector<int>()));
	}

	//accessor
	float get_occupancy() { return float(occupied)/table.size(); }
	int get_kmer() { return kmer; }
	int get_max_o() { return max_o; }
	int get_size() { return table.size(); }

	//modifier
	void set_kmer(int value) { kmer = value; }
	void set_max_o(float value) { max_o = value; }

	//functions
	void add(string key, int position);
	void expand(int size);
	vector<int> search(string key);
	int hash(string key);

private:
	int occupied;
	int kmer;
	float max_o;
	vector<pair<string, vector<int> > > table;//a hash table
};

#pragma once
