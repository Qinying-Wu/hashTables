//Qinying Wu
//20783213
//q227wu
//ECE250 P2-Hashing
//orderedhttest.cpp SEPARATE CHAINING
#include "OrderedHT.h"
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

int main() {
	OrderedHT chain_table(0,0); //initialization of table	
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
			chain_table.n(std::stoi(parameter));
		}
		else if(command=="i"){//the insert case
			int semi_colon = parameter.find(';');
			long long k= std::stoll(parameter.substr(0, semi_colon));
			string caller = parameter.substr(semi_colon+1,parameter.length());
			chain_table.i(k,caller);
		}
		else if (command == "s") {
			string pos=chain_table.s(std::stoll(parameter));
			if (pos == "-1") {
				cout << "not found" << endl;
			}
			else {
				int semi_colon = pos.find(';');
				cout << "found " << pos.substr(semi_colon+1,pos.length()) << " in " << pos.substr(0,semi_colon) << endl;
			}
			
		}
		else if (command == "d") {
			chain_table.d(std::stoll(parameter));
		}
		else if (command == "p") {
			chain_table.p(std::stoi(parameter));
		}
	}
	return 0;
}

//#pragma region OrderedNode
//
//OrderedNode::OrderedNode() {
//	head_c_node = nullptr;
//}
//
////constructor for the table elements in the CHAINING hash table
////t_n contains the row array of the hash table
////head contains the head node of the chain
////back contains the back node of the chain
//OrderedNode::OrderedNode(ChainNode* head){
//	head_c_node = head;
//}
//
////destructor for the table elements in the CHAINING hash tbale
//OrderedNode::~OrderedNode() {
//	head_c_node = nullptr;//deallocate the pointer to the next element in the chain
//	
//}
//#pragma endregion

#pragma region Chain Node

//constructor for the nodes stored in the chain of the chaining hash table
//p carries the phone data, c carries the caller name, and next is the pointer pointing to the next node in line
ChainNode::ChainNode(long long p, string c, ChainNode* next) {
	phone = p;
	caller = c;
	chain_next_node = next;
}

ChainNode::~ChainNode() {

	chain_next_node = nullptr;
}
#pragma endregion

#pragma region OrderedHT
//constructor for the ordered CHAINING hash table
//max carries the max_size, current carries the current_size
OrderedHT::OrderedHT(int max, int current) {
	max_size = max;
	current_size = current;
}

//destructor for the ordered CHAINING hash table
OrderedHT::~OrderedHT(){
	ht.clear();
	ht.resize(0);
}
#pragma endregion

//function n to assign the max capacity of the hash table
//outputs success afterwards
void OrderedHT::n(int m) {
	ht.clear();
	ht.resize(m);
	current_size = 0;
	max_size = m;
	cout << "success" << endl;
}

//inserts the key k and the associated caller
//success if the insertion was successful
//failure id the insertion was unable to complete since the table was full or the key was already there
void OrderedHT::i(long long k, string caller) {
	ChainNode* temp=new ChainNode(k,caller,nullptr); //make temporary node object to be stored in the table
	int h_1 = k % max_size; //primary function

	//case 1: if duplicated key or full table, do not insert and exit
	if (max_size==current_size||s(k)!="-1") {
		cout << "failure" << endl;
	}
	//case 2:  can insert key
	else {
		ChainNode* temp2 = ht.at(h_1); //temp node to store the head of the linked list
		//find the position for insertion
		//case 1: insert as head node
		if (temp2 == nullptr || temp2->phone > k) {
			temp->chain_next_node = temp2; //new node's next pointer is the next pointer
			ht.at(h_1) = temp; //the new node is now the head of the chain
		}
		else {
			while (temp2->phone < k && temp2->phone != 0) {
				//case 2: reached the end node
				if (temp2->chain_next_node == nullptr) {
					temp2->chain_next_node = temp; //add to the end of the linked list
					break;
				}
				//case 3: if the next node of the current traversed node has a larger phone number, insert now
				if (temp2->chain_next_node->phone > temp->phone) {
					temp->chain_next_node = temp2->chain_next_node; //switch the current's next node now to be the new node's next node
					temp2->chain_next_node = temp; //the current's next node is the new node
					break;
				}
				
				
				temp2 = temp2->chain_next_node;
			}
		}
		temp2 = nullptr;
		++current_size;
		
		cout << "success" << endl;
	}
}

//searches for the key k in the table
//returns the index of the key in the chain
string OrderedHT::s(long long k) {
	int h_1 = k % max_size; //find the index on the hash table
	ChainNode* temp = ht.at(h_1);
	//find it in the linked list
	while (temp!= nullptr) {
		//case 1: found
		if (temp->phone == k) {
			return std::to_string(h_1)+";"+temp->caller;
		}
		temp = temp->chain_next_node;//traverse to the next node if not yet found
	}
	//case 2: not found
	return "-1";
}
//deletes the key k from the table
//success if the deletion was successful
//failure if the deletion was unable to complete due to value not found
void OrderedHT::d(long long k) {
	//case 1: key not found
	string p =s(k);
	if (p=="-1") {
		cout << "failure" << endl;
	}
	//case 2:key found
	else {
		//find the key
		int h_1 = k % max_size;
		//case 2.1: delete the head node
		if (ht.at(h_1)->phone==k) {
			//case 2.1a: only one head node in the chain
			if (ht.at(h_1)->chain_next_node==nullptr) {
				delete ht.at(h_1);
				ht.at(h_1) = nullptr;
			}
			//case 2.1b: more than one node in the chain
			else {
				ChainNode* temp = ht.at(h_1);
				ht.at(h_1) = ht.at(h_1)->chain_next_node;
				delete temp;
				temp = nullptr;

			}
			
		}
		//case 2.2: delete a node within the chain
		else {
			//find the node before the node to be deleted
			ChainNode* temp = ht.at(h_1);
			while (temp->phone != k) {
				//case 1: found the key
				if (temp->chain_next_node->phone == k) {
					ChainNode* temp2 = temp->chain_next_node->chain_next_node;//store the next node's next node
					delete temp->chain_next_node; //delete the node
					temp->chain_next_node = temp2; //link to the new node
					temp2 = nullptr; //make temp2 to nullptr if not already
					break;
				}
				//case 2: key not found, keep iterating
				temp = temp->chain_next_node;
			}
		}
		--current_size;
		cout<<"success"<<endl;
	}
}

//prints the chain of keys that start at position i, keys in the chain are separated by one space
//if chain is empty, print nothing
void OrderedHT::p(int i) {
	//print nothing if empty table or node
	//also print empty when index out of bound
	if (i >= max_size||ht.size() == 0|| ht.at(i)==nullptr) {
		return;
	}
	else {
		ChainNode* temp = ht.at(i);//dummy node variable for traversing the chain
		while (temp != nullptr) {
			cout << temp->phone;
			temp = temp->chain_next_node;
			if (temp != nullptr) {
				cout << " ";
			}
		}
		cout << endl;
	}
}