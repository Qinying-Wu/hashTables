//Qinying Wu
//20783213
//q227wu
//ECE250 P2-Hashing
//OrderedHT Header for SEPARATE CHAINING

#include<string>
#include <vector>
using std::string;
using std::vector;

//the nodes of the chaining hash table (can be extended to a linked list)
class ChainNode {
public:
	ChainNode* chain_next_node; //pointer to the next node in the linked list
	long long phone; //key
	string caller; //caller string
	ChainNode(long long p, string c, ChainNode* next);
	~ChainNode();
};

//the chaining hash table
class OrderedHT {
public:
	//defines size (m) of the hash table
	void n(int m);
	//inserts the key k and the associated caller
	//success if the insertion was successful
	//failure id the insertion was unable to complete since the table was full or the key was already there
	void i(long long k, string caller);
	//searches for the key k in the table
	//returns the index of the key in the chain
	string s(long long k);
	//deletes the key k from the table
	//success if the deletion was successful
	//failure if the deletion was unable to complete due to value not found
	void d(long long k);
	//prints the chain of keys that starts at position i
	//keys in the chain are separated by one space
	//print nothing if chain is empty
	void p(int i);
	OrderedHT(int max, int current);
	~OrderedHT();
private:
	int max_size; //max size variable
	int current_size;
	vector<ChainNode*> ht; //the hash table
};

