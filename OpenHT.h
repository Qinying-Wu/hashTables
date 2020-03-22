//Qinying Wu
//20783213
//q227wu
//ECE250 P2-Hashing
//OpenHT Header for DOUBLE HASHING

#include<string>
#include<vector>
using std::vector;
using std::string;

//function to perform the double hashing function
//h_1 is the primary function
//i is the probe
//m is the max_size for modulus operation
//k is the key
int Double_Hash(int h_1, int i, int m, long long k);

const int ERASED_KEY = -1; //magic number to identify a spot with deleted key
const string ERASED_CALLER = ""; //nagic string to identify a spot with deleted key
//class object for hash table nodes using double hash function
class OpenNode {
public:
	//the phone number
	long long phone;
	//the caller
	string caller;
	OpenNode(long long p, string c); //constructor for the item
	~OpenNode(); //destructor for the item
};

class OpenHT {
public:
	//defines size (m) of the hash table
	void n(int m);
	//inserts the key k and the associated caller
	//success if the insertion was successful
	//failure id the insertion was unable to complete since the table was full or the key was already there
	void i(long long k, string caller);
	//searches for the key k in the table
	//true if the desired key was found in the position p of the hash table
	//false if the desired key was not found
	string s(long long k);
	//deletes the key k from the table
	//success if the deletion was successful
	//failure if the deletion was unable to complete due to value not found
	void d(long long k);
	OpenHT(int max, int current);
	~OpenHT();
private:
	int max_size;
	int current_size;
	vector<OpenNode*> ht;

};

