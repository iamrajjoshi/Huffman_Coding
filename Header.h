#pragma once


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
#include <cstring>
#include <bitset>//for binary printing
#include <chrono>

using namespace std::chrono;
using namespace std;

struct node {
	char c;
	unsigned int freq;
	node* L;
	node* R;
};

struct Comparator {
	bool operator()(const node* x, const node* y) {
		return x->freq > +y->freq;
	}
};

struct codeContainer {

	int code[126];
	char ch;
	int len;
};

extern int freqTable[255];
extern priority_queue < node*, vector<node*>, Comparator> pq;
extern codeContainer* container;

extern string fileData; //entire content of file
extern string inFileName;
extern string outFileName;
extern int numCodes;


/*COMPRESSION*/
void compress();
void setFreq();
node* createNode(int, int);
node* createNode(node*, node*);
node* createTree();
void generateCode(node*, int);
void processCompression(int, ofstream&);
void writeCompressedMessage(unsigned int, ofstream&);
void compressionStat();
void writeFreqTable(ofstream&);

/*DECOMPRESSION*/
void decompress();