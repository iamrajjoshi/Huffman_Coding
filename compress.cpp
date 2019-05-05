#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <cstring>
#include <bitset>//for binary printing

using namespace std;

struct node {
	char c;
	unsigned int freq;
	node* L;
	node* R;
};

struct codeContainer {

	int code[126];
	char ch;
	int len;
};

struct Comparator {
	bool operator()(const node* x, const node* y) {
		return x->freq > +y->freq;
	}
};

int freqTable[255] = { 0 };
priority_queue < node*, vector<node*>, Comparator> pq;
codeContainer* container;
string fileData; //entire content of file
int numCodes;

void setFreq();
node* createNode(int, int);
node* createNode(node*, node*);
node* createTree();
void generateCode(node*, int);
void processCompression(int, ofstream&);
void writeCompressedMessage(unsigned int, ofstream&);
void compressionStat();

void compress() {
	ofstream outFile;
	outFile.open("C:\\Users\\geeky\\MEGA\\Workspace\\Visual Studio 2019\\Huffman_Coding\\output.bin", ios::binary | ios::out | ios::trunc);

	
	setFreq(); //builds frequency table
	node* master = createTree(); //builds the binary tree + sets the value of numCodes
	container = (codeContainer*)malloc(numCodes * sizeof(codeContainer));
	generateCode(master, 0);
	processCompression(numCodes, outFile);
	outFile.close();
	compressionStat();
	return;
}

void setFreq() {
	ifstream inFile;
	inFile.open("C:\\Users\\geeky\\MEGA\\Workspace\\Visual Studio 2019\\Huffman_Coding\\input.txt");
	int amt = 0;
	stringstream buffer;
	buffer << inFile.rdbuf();
	fileData = buffer.str();
	for (unsigned int i = 0; i < fileData.length(); i++)
		freqTable[fileData[i]]++;
	inFile.close();
}

node* createNode(int ch, int f) {
	node* n = (node*)malloc(sizeof(node));

	if (n) {
		n->c = ch;
		n->freq = f;
		n->L = NULL;
		n->R = NULL;
	}

	return n;
}

node* createNode(node * nl, node * nr) {
	node* n = (node*)malloc(sizeof(node));

	if (n) {
		n->freq = nl->freq + nr->freq;
		n->L = nl;
		n->R = nr;
		n->c = NULL;
	}
	return n;
}

node* createTree() {

	for (int i = 0; i < 255; i++) {
		if (freqTable[i] != 0) {
			node* n = createNode(i, freqTable[i]);
			pq.push(n);
			numCodes++;
		}
	}

	while (pq.size() != 1) {
		node* n1 = pq.top();
		pq.pop();
		node* n2 = pq.top();
		pq.pop();
		node* add = createNode(n1, n2);
		pq.push(add);
	}

	return pq.top();
}

void generateCode(node * master, int level) {
	static int found = 0;
	static int code[126];

	if (master->L != NULL) {
		code[level] = 0;
		generateCode(master->L, level + 1);
	}

	if (master->R != NULL) {
		code[level] = 1;
		generateCode(master->R, level + 1);
	}

	if (master->c != NULL) {
		//cout << master->c << ": ";
		container[found].ch = master->c;
		for (int i = 0; i < level; i++) {
			//cout << code[i] << " ";
			container[found].code[i] = code[i];
		}
		container[found].len = level;
		//cout << ": " << level << endl;
		//cout << endl;
		found++;
		free(master);
	}
	return;
}

void processCompression(int amt, ofstream& outFile) {
	
	int field = 0, bitCount = 0, byteCount = 0, numBits = 0;
	unsigned int payload = 0;

	for (int i = 0; i < fileData.length(); i++) {//for entire file
		for (int j = 0; j < amt; j++) { //for every code possible
			if (container[j].ch == fileData[i]) { //if the character is the same
				numBits += container[j].len;
				for (int k = 0; k < container[j].len; k++) {
					field = (field << 1) + container[j].code[k];
					bitCount++;
					if (bitCount == 8) {
						payload = (payload << 8);
						memcpy(&payload, &field, 1);
						byteCount++;
						if (byteCount == 4) {
							writeCompressedMessage(payload, outFile);
							byteCount = 0;
							payload = 0;
						}
						bitCount = 0;
						field = 0;
					}
				}
				break;
			}
		}
	}

	if (field != 0) {
		while (bitCount != 8) {
			field = (field << 1) + 1;
			bitCount++;
		}
		if (byteCount != 0) {
			payload = (payload << 8);
			memcpy(&payload, &field, 1);
			writeCompressedMessage(payload, outFile);
			for (int i = 0; i < (4 - (byteCount++)); i++)
				payload = (payload << 8);
		}
		else {
			memcpy(&payload, &field, 1);
			for (int i = 0; i < 24; i++)
				payload = (payload << 1) + 1;
			writeCompressedMessage(payload, outFile);
		}
	}

	else if (payload != 0) {
		for (int i = 0; i < (4 - byteCount); i++)
			payload = (payload << 8);

		for (int i = 0; i < (4 - byteCount); i++)
			for(int j = 0; j < 8; j++)
				payload = (payload << 1) + 1;
		writeCompressedMessage(payload, outFile);
	}
	outFile.close();
	return;
}

void writeCompressedMessage(unsigned int data, ofstream& outFile) {
	//std::string binary = std::bitset<32>(data).to_string(); //to binary
	//std::cout << binary << " ";
	//cout << data << "d" << endl;
	
	outFile.write(reinterpret_cast <const char*> (&data), sizeof(data));
	
}

void compressionStat() {
	ifstream inFile;
	inFile.open("C:\\Users\\geeky\\MEGA\\Workspace\\Visual Studio 2019\\Huffman_Coding\\input.txt");
	inFile.seekg(0, ios::end);
	unsigned long int in = inFile.tellg();
	cout << "Initial File Size: " << in << " bytes" << endl << endl;
	inFile.close();
	inFile.open("C:\\Users\\geeky\\MEGA\\Workspace\\Visual Studio 2019\\Huffman_Coding\\output.bin");
	inFile.seekg(0, ios::end);
	unsigned long int out = inFile.tellg();
	cout << "Compressed File Size: " << out << " bytes" << endl << endl;
	cout << "Compression Ratio: " << (double)(100 * out) / in << "%" << endl << endl;
	inFile.close();
}