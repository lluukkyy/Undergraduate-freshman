// -----------------------------------------------------------------
// HOMEWORK 7 WORD FREQUENCY MAPS
//
// You may use all of, some of, or none of the provided code below.
// You may edit it as you like (provided you follow the homework
// instructions).
// -----------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstdlib>
using namespace std;
typedef  map<string, map<string, int> >  MY_MAP;



// Custom helper function that reads the input stream looking for
// double quotes (a special case delimiter needed below), and white
// space.  Contiguous blocks of alphabetic characters are lowercased &
// packed into the word.
bool ReadNextWord(std::istream &istr, std::string &word) {
  char c;
  word.clear();
  while (istr) {
    // just "peek" at the next character in the stream
    c = istr.peek();
    if (isspace(c)) {
      // skip whitespace before a word starts
      istr.get(c);
      if (word != "") {
	// break words at whitespace
	return true;
      }
    } else if (c == '"') {
      // double quotes are a delimiter and a special "word"
      if (word == "") {
	istr.get(c);
	word.push_back(c);
      }
      return true;
    } else if (isalpha(c)) {
      // this a an alphabetic word character
      istr.get(c);
      word.push_back(tolower(c));
    } else {
      // ignore this character (probably punctuation)
      istr.get(c);
    }
  }
  return false;
}


// Custom helper function that reads the input stream looking a
// sequence of words inside a pair of double quotes.  The words are
// separated by white space, but the double quotes might not have
// space between them and the neighboring word.  Punctuation is
// ignored and words are lowercased.
std::vector<std::string> ReadQuotedWords(std::istream &istr) {
  // returns a vector of strings of the different words
  std::vector<std::string> answer;
  std::string word;
  bool open_quote = false;
  while (ReadNextWord(istr,word)) {
    if (word == "\"") {
      if (open_quote == false) { open_quote=true; }
      else { break; }
    } else {
      // add each word to the vector
      answer.push_back(word);
    }
  }
  return answer;
}

// Loads the sample text from the file, storing it in the map data
// structure Window specifies the width of the context (>= 2) of the
// sequencing stored in the map.  parse_method is a placeholder for
// optional extra credit extensions that use punctuation.
void LoadSampleText(map<string,map<string,map<string,int> > > &data1,MY_MAP &data, const std::string &filename, int window, const std::string &parse_method) {
  // open the file stream
  std::ifstream istr(filename.c_str());
  if (!istr) { 
    std::cerr << "ERROR cannot open file: " << filename << std::endl; 
    exit(1);
  } 
  // verify the window parameter is appropriate
  if (window < 2) {
    std::cerr << "ERROR window size must be >= 2:" << window << std::endl;
  }
  // verify that the parse method is appropriate
  bool ignore_punctuation = false;
  if (parse_method == "ignore_punctuation") {
    ignore_punctuation = true;
  } else {
    std::cerr << "ERROR unknown parse method: " << parse_method << std::endl;
    exit(1);
  }
  //there are two situations:
  //one is when window is 2 the other is then window = 3
  // the basic structure is using loop to repeatedly add the current word 
  //to the last word's map
  // the corner situation is the last 1 or 2 words of the txt
  // just add it/them sperately
  //-------------------------------------------------------window  = 2---------------------------------------------------
  if (window == 2) {
	  string word;
	  string last_word;
	  bool first = 1;
	  while (ReadNextWord(istr, word)) {
		  // skip the quotation marks (not used for this part)
		  if (word == "\"") continue;
		  if (first) first = 0;//dont add the fisrt word
		  else data[last_word][word]++;//use operator[]
		  last_word = word;
	  }
	  data[last_word][""] = 1;
  }
  //-------------------------------------------------------window = 3--------------------------------------------------
  if (window == 3) {
	  string word;
	  string last1;
	  string last2;
	  int count = 0;
	  while (ReadNextWord(istr, word)) {
		  // skip the quotation marks (not used for this part)
		  if (word == "\"") continue;
		  count++;
		  if (count>2) data1[last2][last1][word]++;
		  last2 = last1; last1 = word;
	  }
	  data1[last1][word][""] = 1;
	  data1[word][""][""] = 1;
  }
}

int main () {
  // ASSIGNMENT: THE MAIN DATA STRUCTURE
  MY_MAP data;
  map<string, map<string, map<string, int> > > data1;
  // Parse each command
  std::string command;
  int window;  
  while (std::cin >> command) {
    if (command == "load") {
      std::string filename;
      std::string parse_method;
      std::cin >> filename >> window >> parse_method;      
	  LoadSampleText(data1,data, filename, window, parse_method);
	  cout << "Loaded " << filename << " with window = " << window 
		  << " and parse method = " << parse_method << endl << endl;
    } 
    // print the portion of the map structure with the choices for the
    // next word given a particular sequence.
    else if (command == "print") {
		std::vector<std::string> sentence = ReadQuotedWords(std::cin);
		//=========================================================sentence = 1==============================================================
		if (sentence.size() == 1) {
			//--------------------------------------------------------window = 2---------------------------------------------------------
			if (window == 2) {
				int sum = 0;
				map<string, int> i = data[sentence[0]];
				map<string, int>::iterator j;
				// this loop is used to calculate the sum of the values in the map
				for (j = i.begin(); j != i.end(); j++)sum += j->second;
				cout << sentence[0] << " (" << sum << ")" << endl;
				//this loop prints out each of the element in the map
				for (j = i.begin(); j != i.end(); j++) {
					if (j->first != "")
						cout << sentence[0] << " " << j->first
						<< " (" << j->second << ")" << endl;
				}
				cout << endl;
			}
			//--------------------------------------------------------window = 3 ----------------------------------------------------------------
			if (window == 3) {
				map<string, map<string, int> >j = data1[sentence[0]];
				map<string, map<string,int> >::iterator k;
				int sum = 0;
				//go through all maps to calculate the total sum
				for (k = j.begin(); k != j.end(); k++) {
					map<string, int>::iterator i;
					for(i = k->second.begin();i!=k->second.end();i++)
						sum += i->second;
				}
				cout << sentence[0] << " (" << sum << ")" << endl;
				//for every map of maps 
				for (k = j.begin(); k != j.end(); k++) {
					if (k->first != "")
						sum = 0; map<string, int>::iterator i;
					//calculate the sum of values of each element
						for (i = k->second.begin(); i != k->second.end(); i++)
							sum += i->second;
						cout << sentence[0] << " "  << k->first
						<< " (" << sum << ")" << endl;
				}
				cout << endl;
			}
		}
		//=======================================================sentence = 2========================================================
		if (sentence.size() == 2) {
			//almost the same as when sentence = 1 window = 2
			// just change the level of the map iterators
			map<string, int> j = data1[sentence[0]][sentence[1]];
			map<string, int>::iterator k;
			int sum = 0;
			for (k = j.begin(); k != j.end(); k++) sum += k->second;
			cout << sentence[0] << " " << sentence[1] << " (" << sum << ")" << endl;
			for (k = j.begin(); k != j.end(); k++) {
				if (k->first != "")
					cout << sentence[0] << " " << sentence[1] << " " << k->first
					<< " (" << k->second << ")" << endl;
			}
			cout << endl;
		}
    }

    // generate the specified number of words 
    else if (command == "generate") {
      std::vector<std::string> sentence = ReadQuotedWords(std::cin);
      // how many additional words to generate
      int length;
      std::cin >> length;
      std::string selection_method;
      std::cin >> selection_method;
      bool random_flag;
	  //my solutions are generally, first print out a word,
	  //then find the next word depend on some mechanics 
	  //then turn the next word into first word and loop again.
	  //the word searching mechanics for "most common" solution is go through the map 
	  // and find the word with the biggest value
	  //the mechanics for "random" is to create a vector of words in the map 
	  //the words would appear repeatedly according to its value
	  //then i use the function rand() to get a random index of the vector
	  // for the next word
	  //***********************************************************random*************************************************************
      if (selection_method == "random") {
		  //============================================window = 2============================================================
		  if (window == 2) {
			  random_flag = true;
			  int counter = 0;
			  MY_MAP::iterator i;
			  vector<string> words;
			  int index;
			  i = data.find(sentence[0]);
			  while (i != data.end() && counter < length) {
				  counter++;
				  cout << i->first << " ";
				  map<string, int>::iterator j;
				  words.clear();
				  //go through the map and create the vector of appeared words
				  for (j = i->second.begin(); j != i->second.end(); j++)
					  words.push_back(j->first);
				  index = rand() % words.size();
				  i = data.find(words[index]);
			  }
			  cout << words[index];
			  cout << endl << endl;
		  }
		  //============================================================window = 3===================================================================
		  if (window == 3) {
			  //--------------------------------------------------------sentence = 1-----------------------------------------------------------
			  if (sentence.size() == 1) {
				  cout << sentence[0] << " ";
				  string first_word = sentence[0];
				  string second_word;
				  for (int counter = 0; counter < length; counter++) {
					  vector<string> words;
					  //when generating the first word, it will always be a special situation
					  if (counter == 0) {
						  MY_MAP::iterator i;
						  for (i = data1[sentence[0]].begin();
							  i != data1[sentence[0]].end(); i++) {
							  map<string, int> ::iterator j;
							  for (j = i->second.begin();
								  j != i->second.end(); j++)
								  words.push_back(i->first);
						  }
						  int index = rand() % words.size();
						  second_word = words[index];
						  cout << second_word<<" ";
					  }
					  else{
						  map<string, int> ::iterator i;
						  for (i = data1[first_word][second_word].begin();
							  i != data1[first_word][second_word].end(); i++)
						  {
							  for(int x = 0; x <i->second;x++) words.push_back(i->first);
						  }
							  
						  int index = rand() % words.size();
						  first_word = second_word;
						  second_word = words[index]; 
						  cout << second_word;
						  if(counter!= length-1) cout<< " ";
 					  }
				  }
				  cout << endl << endl;
			  }
			  //----------------------------------------------------------sentence  = 2---------------------------------------------------
			  if (sentence.size() == 2) {
				  cout << sentence[0] << " "<<sentence[1]<<" ";
				  string first_word = sentence[0];
				  string second_word = sentence[1];
				  for (int counter = 0; counter < length; counter++) {
					  vector<string> words;
					  map<string, int> ::iterator i;
					  //the vital different here appears when creating the vector
					  //should go through the 2D map , using 2 for loops 
					  for (i = data1[first_word][second_word].begin();
						  i != data1[first_word][second_word].end(); i++)
					  {
						  for (int x = 0; x <i->second; x++) words.push_back(i->first);
					  }
					  int index = rand() % words.size();
					  first_word = second_word;
					  second_word = words[index];
					  cout << second_word;
					  if (counter != length - 1) cout << " ";
				  }
				  cout << endl << endl;
			  }
		  }
      } 
		else {//*****************************************************common*************************************************************
		  random_flag = false;
		  //===================================================window = 2================================================================
		  if (window == 2) {
			  int counter = 0;
			  MY_MAP::iterator i;
			  i = data.find(sentence[0]);
			  string biggest_word;
			  while (i != data.end() && counter < length) {
				  cout << i->first << " ";
				  counter++;
				  map<string, int>::iterator j;
				  int biggest = 0;
				  //the loop here finds the word with a biggest value
				  for (j = i->second.begin(); j != i->second.end(); j++) {
					  if (j->second > biggest) {
						  biggest = j->second;
						  biggest_word = j->first;
					  }
				  }
				  i = data.find(biggest_word);
			  }
			  cout << biggest_word;
			  cout << endl << endl;
		}
		  //=========================================================window  = 3============================================================
		if (window == 3) {
			//-----------------------------------------------------sentence = 1 ---------------------------------------------------------
			if (sentence.size() == 1) {
				int counter = 0;
				cout << sentence[0] << " ";
				string biggest_word;
				string first_word = sentence[0];
				string temp;
				for (; counter < length; counter++) {
					//also, when treating the first word, should make some subtle difference 
					if (counter == 0) {
						MY_MAP::iterator i;
						int biggest = 0;
						//2D for loop to find the biggest value
						for (i = data1[sentence[0]].begin();
							i != data1[sentence[0]].end(); i++) {
							map<string, int> ::iterator j;
							int sum = 0;
							for (j = i->second.begin();
								j != i->second.end(); j++)
								sum += j->second;
							if (sum > biggest) {
								biggest = sum;
								biggest_word = i->first;
							}
						}
						cout << biggest_word << " "; temp = biggest_word;
					}
					else {
						map<string, int> ::iterator j;
						int biggest = 0;
						//2D for loop to find the biggest value
						for (j = data1[first_word][temp].begin();
							j != data1[first_word][temp].end(); j++) {
							if (j->second > biggest) {
								biggest = j->second;
								biggest_word = j->first;
							}
						}
						cout << biggest_word ;
						if (counter != length - 1) cout << " ";
						first_word = temp; temp = biggest_word;
					}
				}
				cout << endl << endl;
			}
			//----------------------------------------------------------sentence = 2-------------------------------------------------------
			if (sentence.size() == 2) {
				int counter = 0;
				cout << sentence[0] << " "<<sentence[1]<<" ";
				string biggest_word;
				string first_word = sentence[0];
				string temp = sentence[1];
				for (; counter < length; counter++) {
					map<string, int> ::iterator j;
					int biggest = 0;
					//2D for loop to find the biggest value
					for (j = data1[first_word][temp].begin();
						j != data1[first_word][temp].end(); j++) {
						if (j->second > biggest) {
							biggest = j->second;
							biggest_word = j->first;
						}
					}
					cout << biggest_word ;
					if (counter != length - 1) cout << " ";
					first_word = temp; temp = biggest_word;
				}
				cout << endl << endl;
			}
		}
	}
    }
    else if (command == "quit") {
      break;
    } else {
      std::cout << "WARNING: Unknown command: " << command << std::endl;
    }
  }
}
