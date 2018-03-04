#include <vector>
using namespace std;

class puzzle {
public:
	puzzle(int row, int col) ;
	int row() { return Row; }
	int col() { return Col; }
	void print_puzzle();
	vector<vector<string> > data(){ return Data; }
	void put_it_at(string word, vector<int> start_point, vector<int> end_point);
	void set(int row, int col, string word);
	bool is_rword(string word);
	bool contains(string word);
	bool is_same(string word, vector<int> start, vector<int> end);
	bool compare(string word, vector<int> start, vector<int> end);
	vector<vector<int> > available_positions(string word);
	puzzle operator=(puzzle p);
	
private:
	vector<vector<string> > Data;
	int Row;
	int Col;
};
//check if two puzzles are the same
bool operator==(puzzle p1, puzzle p2) {
	if (p1.row() != p2.row() || p1.col() != p2.col()) return 0;
	for (int i = 0; i < p1.row(); i++) {
		for (int j = 0; j < p1.col(); j++) {
			if (p1.data()[i][j] != p2.data()[i][j]) return 0;
		}
	}
	return 1;
}