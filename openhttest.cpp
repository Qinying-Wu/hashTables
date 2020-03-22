//Qinying Wu
//20783213
//q227wu
//ECE250 P2-Hashing
//openhttest.cpp

#include <iostream>
#include "OpenHT.h"
using std::cout;
using std::cin;
using std::endl;
using std::string;

int main()
{
	OpenHT open_table(0,0);
	string input = "";
	while (getline(cin, input)) {
		int space = input.find(' ');
		string command = input.substr(0, space);
		string parameter = "";
		if (space != -1) {
			parameter = input.substr(space + 1, input.length());
		}
		//start executing commands
		if (command == "n") { //the set max size case
			open_table.n(std::stoi(parameter));
		}
		else if (command == "i") {//the insert case
			int semi_colon = parameter.find(';');
			long long k = std::stoll(parameter.substr(0, semi_colon));
			string caller = parameter.substr(semi_colon + 1, parameter.length());
			open_table.i(k, caller);
		}
		else if (command == "s") {
			string pos = open_table.s(std::stoll(parameter));
			if (pos == "-1") {
				cout << "not found" << endl;
			}
			else {
				int semi_colon = pos.find(';');
				cout << "found " << pos.substr(semi_colon + 1, pos.length()) << " in " << pos.substr(0, semi_colon) << endl;
			}

		}
		else if (command == "d") {
			open_table.d(std::stoll(parameter));
		}
	}
	return 0;
}


OpenNode::OpenNode(long long p, string c) {
	phone = p;
	caller = c;
}
OpenNode::~OpenNode() {

}

//Constructor for the hash table using double hashing
//max is the capacity of the table
//current is the current amount of keys contained in the table
//table is an array which symbolizes the hash table set up
OpenHT::OpenHT(int max, int current) {
	max_size = max;
	current_size = current;
}


//Destructor for the hash table
//deallocate the array containing the keys and set it to nullptr to prevent dangling pointer
OpenHT::~OpenHT() {
	ht.clear();
	ht.resize(0);
	ht.~vector();
}

//function n to assign the max capacity of the hash table
//outputs success afterwards
void OpenHT::n(int m) {
	ht.clear();
	ht.resize(m);
	current_size = 0;
	max_size = m;
	//start a new table
	cout << "success" << endl;
}

//inserts the key k and the associated caller
//success if the insertion was successful
//failure id the insertion was unable to complete since the table was full or the key was already there
void OpenHT::i(long long k, string caller) {
	OpenNode* temp=new OpenNode (k, caller); //make temporary node object to be stored in the table
	int h_1 = k % max_size; //first compute whethere the primary function can find a space to fit

	//case 1: if table is full, DO NOT INSERT and exit function
	//case 2: duplicated key
	if (max_size==current_size||s(k)!="-1") {
		cout << "failure" << endl;
	}
	else {
		//case 2: the slot is either never inserted before or something is erased before upon insertion
		if (ht.at(h_1)==nullptr||ht.at(h_1)->phone==ERASED_KEY) {
			ht.at(h_1) = temp;
		}
		//case 3" the slot is not empty upon insertion
		else {
			int i = 1; //probing initialization
			int probe = Double_Hash(h_1,i,max_size,k); //double hashing function

			//check that the node reached has something in there (whether filled with valid phone number or erase indicator)
			while (ht.at(probe)!=nullptr) {
				//exit the loop if reached a spot where a key is erased previously
				if (ht.at(probe)->phone == ERASED_KEY) {
					break;
				}
				++i; //increment the probe to do double hashing again
				probe = Double_Hash(h_1,i,max_size,k); //keep incrementing
			}
			ht.at(probe) = temp; //will eventually reach an empty slot after exiting the loop
			
		}
		++current_size;
		cout << "success" << endl;
	}
}

//searches for the key k in the table
//true if the desired key was found in the position p of the hash table
//false if the desired key was not found
string OpenHT::s(long long k) {
	int h_1 = k % max_size;//primary function to determine location
	//case 1: found in primary key
	if (ht.at(h_1) != nullptr && ht.at(h_1)->phone == k) {
		return std::to_string(h_1) + ";" + ht.at(h_1)->caller;
	}
	//case 2: not found in primary key, either because primary key was erased or its key does not match
	else if(ht.at(h_1)!=nullptr&&ht.at(h_1)->phone!=k) {
		int i = 1; //probing initialization
		//int h_2_reference= Double_Hash(h_1, i, max_size, k);
		//find another key using double hashing
		int p = Double_Hash(h_1, i, max_size, k);
		//case 2a: the second slot is either erased or filled
		//if it is never initialized, it must not exist, refer to the case with primary function
		while (ht.at(p) != nullptr)
		{
			//case 2b: the slot returned to the result of primary function
			//			stop and return not found because it would do the same loop again
			if (p==h_1) {
				break;
			}
			else if (ht.at(p)->phone ==k) {
				return std::to_string(p) + ";" + ht.at(p)->caller;
			}
			++i; //increment the probe to do double hashing again
			p = Double_Hash(h_1, i, max_size, k);
		
		}
	}
	//case 3: not found in primary key, and primary key is never filled before
	//			this means the key is never in the table
	return "-1";
}
//deletes the key k from the table
//success if the deletion was successful
//failure if the deletion was unable to complete due to value not found
void OpenHT::d(long long k) {
	string p = s(k);
	//case 1: key not found
	if (p=="-1") {
		cout<<"failure"<<endl;
	}
	//case 2:key found
	else {
		//identify the slot on the table
		ht.at(stoi(p.substr(0, p.find(';'))))->phone=ERASED_KEY;
		ht.at(stoi(p.substr(0, p.find(';'))))->caller = ERASED_CALLER;
		--current_size;
		//in the case where the last element in the table is deleted, start a fresh table
		if (current_size == 0) {
			ht.clear();
			ht.resize(max_size);
		}
		cout<<"success"<<endl;
	}
}

//function to perform the double hashing function
//h_1 is the primary function
//i is the probe
//m is the max_size for modulus operation
//k is the key
int Double_Hash(int h_1,int i, int m,long long k) {
	int h_2 = (k / m) % m; //secondary function
	h_2 = h_2 % 2 == 1 ? h_2 : h_2 + 1;//h_2 must be an odd number
	return (h_1 + i * h_2) % m; //double hashing function
}