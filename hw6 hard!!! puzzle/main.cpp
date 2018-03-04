#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <algorithm>
#include "puzzle.h"
//create a vector of the alphabet which turns an int to a matching string 
//of character otherwise there would be a char/string problem, i have no idea how
//to fix that
string alphabet(int i) {
	vector<string> a;
	a.push_back("a"); a.push_back("b"); a.push_back("c"); a.push_back("d");
	a.push_back("e"); a.push_back("f"); a.push_back("g"); a.push_back("h");
	a.push_back("i"); a.push_back("j"); a.push_back("k"); a.push_back("l");
	a.push_back("m"); a.push_back("n"); a.push_back("o"); a.push_back("p");
	a.push_back("q"); a.push_back("r"); a.push_back("s"); a.push_back("t");
	a.push_back("u"); a.push_back("v"); a.push_back("w"); a.push_back("x");
	a.push_back("y"); a.push_back("z");
	return a[i];
}
//the default constructor of the puzzle class
//it creats a 2D vector containing string(".")s
puzzle::puzzle(int row, int col) {
	Row = row;
	Col = col;
	for (int i = 0; i < row; i++) {
		vector<string> temp;
		for (int j = 0; j < col; j++) {
			temp.push_back(".");
		}
		Data.push_back(temp);
	}
}
//it's an essential mechanic which takes a word, a start point and an end point.
// the word will be put from the start point to the end point one character by one character.
void puzzle::put_it_at(string word, vector<int> start_point, vector<int> end_point) {
	int dcol; int drow;
	int temp[2];
	temp[0] = start_point[0]; temp[1] = start_point[1];
	if (start_point[0] == end_point[0]) drow = 0;
	else {
		if (start_point[0] < end_point[0]) { drow = 1; }
		else { drow = -1; }
	}
	if (start_point[1] == end_point[1]) dcol = 0;
	else {
		if (start_point[1] < end_point[1]) { dcol = 1; }
		else { dcol = -1; }
	}
	for (int i = 0; i < word.size(); i++) {
		Data[temp[0]][temp[1]] = word[i];
		temp[0] += drow;
		temp[1] += dcol;
	}
}
//check if the word is in the form of "aba"
bool puzzle::is_rword(string word) {
	int half_length = word.size() / 2;
	bool result = 1;
	for (int i = 0; i < half_length; i++) {
		result *= (word[i] == word[word.size() - 1 - i]);
	}
	return result;
}
//the largerst function of this program. it takes a word and return all 
// available positions it can be put at in the current puzzle.
// these positions form the branches of the following recursion 
vector<vector<int> > puzzle::available_positions(string word) {
	vector<vector<int> > result;
	bool is_r = is_rword(word);
	int length = word.size();
	//c,r is the number of posible positions in a col or row
	// to put a three-letter word in 2*3 puzzle, c would be 1, r would be 0
	int c = Col - length + 1;
	int r = Row - length + 1;
	vector<int> start(2,0); vector<int> end(2,0);
	if (c >= 1) {
		// if c>=1 , it means we can put the word horizontally
		for (int i = 0; i < c; i++) {
			for (int j = 0; j < Row; j++) {
				start[0]= j; start[1]= i;
				end[0] = j; end[1]= i + length - 1;
				//before actually push it to the vector, 
				//we should first check if it fits the puzzle
				// in other words, if there's any conflicts of letters
				// between current puzzle and the word we are to put in.
				if (compare(word, start, end)) {
					result.push_back(start);
					result.push_back(end);
				}
				//reverse the position and also push it to the vector.
				//if the word is "aba", then it unnecessary to reverse the 
				// position
				if (!is_r) {
					if (compare(word, end, start)) {
						result.push_back(end);
						result.push_back(start);
					}
				}
			}
		}
	}
	if (r >= 1) {
		// it r>=1, it means we can put the word vertically
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < Col; j++) {
				start[0]=i; start[1]=j;
				end[0]=i + length -1; end[1]=j;
				if (compare(word, start, end)) {
					result.push_back(start);
					result.push_back(end);
				}
				if (!is_r) {
					if (compare(word, end, start)) {
						result.push_back(end);
						result.push_back(start);
					}
				}
			}
		}
	}
	if (r >= 1 && c>=1) {
		// if r>=1 and c>=1, it means we can put the word diagonally
		for (int i = 0; i < r; i++) {
			for (int j = 0; j < c; j++) {
				start[0]=i; start[1]=j;
				end[0]=i + length - 1; end[1]=j+length-1;
				if (compare(word, start, end)) {
					result.push_back(start);
					result.push_back(end);
				}
				if (!is_r) {
					if (compare(word, end, start)) {
						result.push_back(end);
						result.push_back(start);
					}
				}
				// remember there are two diagonals in a square
				// so we should do it again similarly
				start[0]=i; start[1]=j+length-1;
				end[0]=i + length - 1; end[1]=j;
				if (compare(word, start, end)) {
					result.push_back(start);
					result.push_back(end);
				}
				if (!is_r) {
					if (compare(word, end, start)) {
						result.push_back(end);
						result.push_back(start);
					}
				}
			}
		}
	}
	return result;
}
// it's the = constructor of puzzle class
// which creates a new puzzle object the same as a current one
puzzle puzzle::operator=(puzzle p) {
	Row = p.row();
	Col = p.col();
	for (int i = 0; i < Row; i++) {
		vector<string> temp;
		for (int j = 0; j < Col; j++) {
			temp.push_back(p.data()[i][j]);
		}
		Data.push_back(temp);
	}
}
//check if the word is EXACTLY the same as the thing on the position
bool puzzle::is_same(string word,vector<int> start, vector<int> end) {
	bool result = 0;
	if (start[0] >= 0 && start[0] < Row
		&& start[1] >= 0 && start[1] < Col
		&& end[0] >= 0 && end[0] < Row
		&& end[1] >= 0 && end[1] < Col) {
		int dcol; int drow;
		int temp[2];
		temp[0] = start[0]; temp[1] = start[1];
		if (start[0] == end[0]) drow = 0;
		else {
			if (start[0] < end[0]) { drow = 1; }
			else { drow = -1; }
		}
		if (start[1] == end[1]) dcol = 0;
		else {
			if (start[1] < end[1]) { dcol = 1; }
			else { dcol = -1; }
		}
		result = 1;
		for (int i = 0; i < word.size(); i++) {
			if(Data[temp[0]][temp[1]][0] != word[i]) result = 0;
			temp[0] += drow;
			temp[1] += dcol;
		}
	}
	return result;
}
// this function takes a word and it will tell you if the word is in
// the puzzle
bool puzzle::contains(string word){
	int length = word.size() - 1;
	bool result = 0;
	vector<int> start(2,0); vector<int> end(2, 0);
	for (int i = 0; i < Data.size(); i++) {
		for (int j = 0; j < Data[i].size(); j++) {
			start[0] = i; start[1] = j;
			if (Data[i][j][0] == word[0]) {
			//we first go through the puzzle and find a position which has
			// the same letter of the word's first letter
			// then we check 8 directions. if any of it fits the word, return true
				end[0] = i; end[1] = j + length;
				if (is_same(word, start, end)) result = 1;
				end[0] = i; end[1] = j - length;
				if (is_same(word, start, end)) result = 1;
				end[0] = i+length; end[1] = j;
				if (is_same(word, start, end)) result = 1;
				end[0] = i-length; end[1] = j;
				if (is_same(word, start, end)) result = 1;
				end[0] = i+length; end[1] = j + length;
				if (is_same(word, start, end)) result = 1;
				end[0] = i+length; end[1] = j - length;
				if (is_same(word, start, end)) result = 1;
				end[0] = i-length; end[1] = j + length;
				if (is_same(word, start, end)) result = 1;
				end[0] = i-length; end[1] = j - length;
				if (is_same(word, start, end)) result = 1;
			}
		}
	}
	return result;
}
//this function tells you if the word fits the position
//by "fits", it means every letter of the word is the same as the letter  
//in the puzzle or the position in the puzzle is empty (is ".")
bool puzzle::compare(string word,vector<int> start, vector<int> end){
	bool result = 1;
	int dcol; int drow;
	int temp[2];
	temp[0] = start[0]; temp[1] = start[1];
	if (start[0] == end[0]) drow = 0;
	else {
		if (start[0] < end[0]) { drow = 1; }
		else { drow = -1; }
	}
	if (start[1] == end[1]) dcol = 0;
	else {
		if (start[1] < end[1]) { dcol = 1; }
		else { dcol = -1; }
	}
	for (int i = 0; i < word.size(); i++) {
		if (Data[temp[0]][temp[1]][0] != word[i] && Data[temp[0]][temp[1]] != ".") 
			result = 0;
		temp[0] += drow;
		temp[1] += dcol;
	}
	return result;
}
//print_puzzle() is used to print the puzzle
//it's not used in the final version but important for testing during the
//proccess of programing
void puzzle::print_puzzle() {
	for (int i = 0; i < Data.size(); i++) {
		for (int j = 0; j < Data[i].size(); j++) {
			cout << Data[i][j] << " ";
		}
		cout <<endl;
	}
}
//simply set the postition of the puzzle to a string
void puzzle::set(int row, int col, string word) {
	Data[row][col] = word;
}
//it's a small recursion included by the general one
//it solves the cases in which all required words are filled 
// but still are some empty spots there in the puzzle
void add_letters(puzzle& p,bool& done,vector<puzzle>& l,vector<string> banned_words) {
	bool has_spot = 0;
	for (int n = 0; n < p.row(); n++) {
		for (int m = 0; m < p.col(); m++) {
			//first find the empty spot
			if (p.data()[n][m] == ".") {
				has_spot = 1;
				done = 1;
			//then go through the alphabet to fill it
				for (int t = 0; t < 26; t++) {
					p.set(n, m, alphabet(t));
					//once filled, check the validity of the puzzle
					bool found = 0;
					for (int s = 0; s < banned_words.size(); s++) {
						if (p.contains(banned_words[s])) { found = 1; break; }
					}
					//if failed, set it back
					if (found) p.set(n, m, ".");
					//if successed, go on to fill the next spot
					else add_letters(p, done,l,banned_words);
				}
			}
		}
	}
	//if we don't have any empty positions or we've already filled them all
	// push the puzzle to the result list
	if (!has_spot) { l.push_back(p); }
}
//it's the helper function of run() recursion, the main recursion and also the 
// pivot connecting all other functions
// it takes an empty puzzle and some words. Then it does some changes to the puzzle,
// specifically, adding words. once it fits the required condition, it push the current
// puzzle to the result list
void sub_run(puzzle p, vector<string> words, vector<string> banned_words, int i, 
	int j, vector<vector<int> >& al, vector<puzzle>& l,int counter) {
	//first we have to garantee there's a word to use
	if (i < words.size()) {
		//then we calculate all the posible positions about current situation
		al = p.available_positions(words[i]);
		//we should also ensure that there are available positions
		if (j < al.size()) {
			//record the current puzzle for the recover in some case
			puzzle temp_p = p;
			//now we put a word in it
			p.put_it_at(words[i], al[j], al[j + 1]);
			//we check the validity
			bool found = 0;
			for (int k = 0; k < banned_words.size(); k++) {
				if (p.contains(banned_words[k])) {
					found = 1; break;
				}
			} 
			//success 
			//take next two steps are to make a branch in the recursion
			if (!found) {
				//one side, go ahead put the next word
				sub_run(p, words, banned_words, i + 1, 0, al, l,counter+1);
				//on the other side, recover the puzzle and put the word in another position 
				sub_run(temp_p, words, banned_words, i, j + 2, al, l,counter);
			}
			//fail 
			//recover the puzzle and try another position for the word
			else {
				sub_run(temp_p, words, banned_words, i, j + 2, al, l,counter);
			}
		}
		//there are some small differences in several
		//situations when we ran out of available positions.
		//But basically it cause the program go on to put the next word
		else {
			if (i + 1 >= words.size()) {
				sub_run(p, words, banned_words, i + 1, j, al, l,counter);
			}
			else {
				if (i == 0 && j >=al.size()) 
					sub_run(p, words, banned_words, i + 1, j, al, l,counter);
				else sub_run(p, words, banned_words, i + 1, 0, al, l,counter);
			}
		}
	}
	//if we ran out of words
	else {
		// check if we have already done all the works
		//  or it's just simply running out of words
		if (j < al.size() && counter == words.size()) {
			//yes! we got all the works done
			//but to remember that we need to check if there're
			//still any positions left empty
			bool done = 0;
			//if there are, we fill them with letters
			add_letters(p, done, l, banned_words);
			//if there are not, push the final puzzle to the result list
			if(!done) l.push_back(p);
		}
	}
}
//the problem is: there may be some repeat of puzzles after we filled up the blank
//spots by random letters
//so we should cancel these adundant ones out of the list
vector<puzzle> unique(vector<puzzle> l) {
	vector<puzzle> result;
	for (int i = 0; i < l.size(); i++) {
		bool found = 0;
		for (int j = 0; j < result.size(); j++) {
			if (result[j] == l[i]) found = 1;
		}
		if (!found) { result.push_back(l[i]); }
	}
	return result;
}
//this is the driver function of the recrusion
//it initials variables and executes the helper
vector<puzzle> run(vector<string> words, vector<string> banned_words, int row, int col) {
	puzzle p(row, col);
	vector<vector<int> > al;
	vector<puzzle> l;
	sub_run(p,words,banned_words,0,0,al,l,0);
	vector<puzzle> result;
	result = unique(l);
	return result;
}
//the bool is used to string sorting
bool longer_than(string s1, string s2) {
	return s1.size() > s2.size();
}

int main(int argc, char* argv[]) {
	ifstream input(argv[1]);
	ofstream output(argv[2]);
	string command(argv[3]);
	vector<string> words;
	vector<string> banned_words;
	string text;
	int row; int col;
	input >> col; input >> row;
	//take in inputs
	while (input >> text) {
		if (text == "+") {
			input >> text;
			words.push_back(text);
		}
		if (text == "-") {
			input >> text;
			banned_words.push_back(text);
		}
	}
	//sort the words from long to short
	//this step helps shorten the running time
	sort(words.begin(), words.end(), longer_than);
	vector<puzzle> result;
	result =run(words, banned_words, row, col);
	//following codes control how may solutions to output
	int times;
	if (command == "all_solutions") {
		output << result.size() << " solution(s)\n";
		times = result.size();
	}
	if (command == "one_solution")  times = 1; 
	//print out puzzles
	for (int i = 0; i < times; i++) {
		output << "Board:\n";
		for (int m = 0; m < result[i].data().size(); m++) {
			output << "  ";
			for (int n = 0; n < result[i].data()[m].size(); n++) {
				output << result[i].data()[m][n];
			}
			output << "\n";
		}
	}
	return 0;
}