#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <fstream>
using namespace std;

vector<vector<string> > upgrade_list;

vector<vector<string> > initseats(char* filename) {
	vector<vector<string> > seats; ifstream file(filename); string read; vector<string> temp_vector; int temp = -1;
	while (file >> read) {
		if (read == "F" || read == "B" || read == "C") { seats.push_back(temp_vector); temp++; }
		seats[temp].push_back(read);
	} 
	return seats;
}

void prt(int flight_num, vector<vector<string> > seats) {
	cout << "Seating Chart for Flight " << flight_num << endl;
	cout << "\tA    B    C    D    E    F    G    Seating Class" << endl;
	for (int i = 0; i < seats.size(); ++i) {
		cout << i + 1<<"\t";
		for (int j = 1; j < seats[i].size(); j++) {
			if (seats[i][j] == "!" || seats[i][j] == ".") { cout << seats[i][j] << seats[i][j] << seats[i][j] << seats[i][j] << " "; }
			else { cout << seats[i][j] << " " ; }
		}
		cout << seats[i][0] << endl;
	}

}

vector<vector<string> > srn(vector<vector<string> > seats, string name, string Class, string freq) {	
	vector<vector<string> > temp_seats=seats; 
	if (Class == "F" || Class == "B" || Class == "C") { 
		vector<vector<int> > sorted_seats; vector<vector<string> > seatsinfo = seats; vector<int> coordinate;
		for (unsigned int i = 0; i < temp_seats.size(); i++) {
			if (temp_seats[i][0] == Class) {
				for (unsigned int j = 1; j < temp_seats[i].size(); j++) {
					if (temp_seats[i][j] == "." && (j == 1 || j == temp_seats[i].size() - 1 || temp_seats[i][j + 1] == "!" || temp_seats[i][j - 1] == "!"))
					{
						coordinate.clear(); coordinate.push_back(i); coordinate.push_back(j);
						sorted_seats.push_back(coordinate); seatsinfo[i][j] = "*";
					}
				}
			}
		}
		for (unsigned int i = 0; i < seatsinfo.size(); i++) {
			if (seatsinfo[i][0] == Class) {
				for (unsigned int j = 1; j < seatsinfo[i].size(); j++) {
					if (seatsinfo[i][j] == ".") {
						coordinate.clear(); coordinate.push_back(i); coordinate.push_back(j); sorted_seats.push_back(coordinate);
					}
				}
			}
		}
		for (unsigned int i = 0; i < temp_seats.size(); i++) {
			if (temp_seats[i][0] == Class) {
				for (unsigned int j = 0; j < sorted_seats.size(); j++) {
					if (temp_seats[sorted_seats[j][0]][sorted_seats[j][1]] == ".") {
						temp_seats[sorted_seats[j][0]][sorted_seats[j][1]] = string(name);
						cout << "Passenger " << name << " was given seat " <<sorted_seats[j][0]+1<<(char)(sorted_seats[j][1]+64)<< "."<<endl;
						return temp_seats;
					}
				}
			}	
		}
		vector<string> upinfo;
		if (freq == "Y") 
		{ upinfo.clear(); upinfo.push_back(name); upinfo.push_back(Class); upinfo.push_back(freq); upgrade_list.push_back(upinfo); }
		return temp_seats;
	}
	else { cout << "Invalid seating class - " << Class << endl; }
	return temp_seats;
}

vector<vector<string> > ssr(vector<vector<string> > seats, string name, string seat, string freq) {
	vector<vector<string> > temp_seats = seats;
	string temp_seat=seat; int col = (int)(temp_seat[temp_seat.size() - 1] - 64); 
	temp_seat = temp_seat.substr(0,temp_seat.size() - 1); int row = atoi(temp_seat.c_str())-1;
	if (row >= seats.size() || col >= seats[0].size()||seats[row][col]=="!")
	{ cout << "Passenger " << name << " requested an invalid seat " <<seat<<"."<<endl; return temp_seats; }
	if (temp_seats[row][col] != ".") { 
		cout << "Passenger " << name << " requested an occupied seat " << seat << "." << endl; 
		temp_seats = srn(temp_seats, name, temp_seats[row][0], freq);
		return temp_seats; 
	}
	temp_seats[row][col] = name; 
	cout << "Passenger " << name << " was given seat " << seat << "." << endl;
	return temp_seats;
}

void upl() {
	vector<string> business, coach;
	for (unsigned int i = 0; i < upgrade_list.size(); i++) {
		if (upgrade_list[i][1] == "B") { business.push_back(upgrade_list[i][0]); }
		if (upgrade_list[i][1] == "C") { coach.push_back(upgrade_list[i][0]); }
	}
	cout << "The following passengers are scheduled to be upgraded to first class:" << endl; for (unsigned int i = 0; i < business.size(); i++) { cout << business[i] << endl; }
	cout << "The following passengers are scheduled to be upgraded to business class:" << endl; for (unsigned int i = 0; i < coach.size(); i++) { cout << coach[i] << endl; }
}

string upclass(string Class) { if (Class == "B") { return "F"; } if (Class == "C") { return "B"; } }

string classname(string Class) { if (Class == "F") { return "first"; } if (Class == "B") { return "business"; } if (Class == "C") { return "coach"; } }

vector<vector<string> > upp(vector<vector<string> > seats) {
	vector<vector<string> > temp_seats = seats;  vector<vector<string> > unsuccess_upgrade;
	for (unsigned int i = 0; i < upgrade_list.size(); i++) {
		temp_seats = srn(seats, upgrade_list[i][0], upclass(upgrade_list[i][1]), upgrade_list[i][2]);
		if (upgrade_list[upgrade_list.size() - 1][0] == upgrade_list[i][0]&&(upgrade_list.size()!=1)) {
			cout << "Could not find a seat in the " << upgrade_list[i][1] << " section for passenger " << upgrade_list[i][0] << endl;
			upgrade_list.pop_back(); unsuccess_upgrade.push_back(upgrade_list[i]);
		}
		else { cout << "Passenger " << upgrade_list[i][0] << " was upgraded to the " << classname(upclass(upgrade_list[i][1])) << " class section." << endl; }
	}
	upgrade_list = unsuccess_upgrade;
	return temp_seats;
}


vector<vector<string> > cxl(vector<vector<string> > seats, string name) {
	vector<vector<string> > temp_seats = seats; vector<vector<string> > temp_upgrade_list;
	for (unsigned int i = 0; i < temp_seats.size(); i++) {
		for (unsigned int j = 1; j < temp_seats[i].size(); j++) {
			if (temp_seats[i][j] == name) { temp_seats[i][j] = "."; return temp_seats;}
		}
	}
	for (unsigned int n = 0; n < upgrade_list.size(); n++) {
		if (upgrade_list[n][0] != name) { temp_upgrade_list.push_back(upgrade_list[n]); }
	}
	if (temp_upgrade_list == upgrade_list) { cout << "Passenger " << name << " could not be found." << endl; return temp_seats; }
	upgrade_list = temp_upgrade_list;
	return temp_seats;
}



vector<vector<string> > execute(vector<vector<string> > seats, vector<string> command, int flight_num) {
	vector<vector<string> > temp_seats = seats;
	if (command[0] == "SRN") { temp_seats = srn(seats, command[1], command[2], command[3]); }
	if (command[0] == "SSR") { temp_seats = ssr(seats, command[1], command[2], command[3]); }
	if (command[0] == "CXL") { temp_seats = cxl(seats, command[1]); }
	if (command[0] == "UPL") { upl(); }
	if (command[0] == "UPP") { temp_seats = upp(seats); }
	if (command[0] == "PRT") { prt(flight_num, seats); }
	return temp_seats;
	/*this is where every order is read and led to a designated function to solve. update the seat every time after excuting an order.*/
}


void commands(char* filename, vector<vector<string> > seats) {
	ifstream file(filename); string read; int flight_num; vector<string> command; int count = -1; int gap = 4;
	while (file >> read) {
		count++;
		if (count == 0) { flight_num = atoi(read.c_str()); }
		else {
			command.push_back(read);
			if (read == "PRT" || read == "UPP" || read == "UPL") { gap = 1; }
			if (read == "CXL") { gap = 2; }
			if (count == gap) { count = 0;  seats = execute(seats, command, flight_num); command.clear(); gap = 4; }
		}
	}
	/*gap is length of a particular order. ssr,srn is 4, cxl is 2,etc. It helps the function to determine whether an orders is complete.
	commands() first reads the second file and once completes, puts all info it read into a vector,for example:
	{"ssr", "abcd", "4r", "y"} or {"prt"}, etc.
	and then execute it immediately through execute().*/
}


int main(int argc, char* argv[]) {
	vector<vector<string> > seats = initseats(argv[1]);
	/*function initseats() reads the text file and returns a 2D vector consists of every rows of the plane,for example:
	{"F", ".", "!", ".", ".", "!", ".", "."}. which means vector[n][0] would be the name of class.*/
	commands(argv[2], seats);
	/*commands() reads the second text file and executes every single command immediately after being read.
	This can make sure that all infomation is latest to update.*/
}