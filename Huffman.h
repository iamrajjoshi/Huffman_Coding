#pragma once
#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

struct huffman_node {
	char id;
	int freq;
	string code;
	huffman_node* left;
	huffman_node* right;
	huffman_node() {
		left = nullptr;
		right = nullptr;
	}
};

class Huffman {
private:
	huffman_node* freq_table[256];
	fstream inFile, outFile;
	huffman_node* child;
	huffman_node* parent;
	huffman_node* root;
	char id;
	string inFileName, outFileName;
	struct Comparator {
		bool operator()(const huffman_node* c1, const huffman_node* c2) {
			return c1->freq > c2->freq;
		}
	};
	priority_queue<huffman_node*, vector<huffman_node*>, Comparator> pq;
	
	void makeFQ();
	void makePQ();
	void makeTree();
	void makeCodes(huffman_node*, string);
	int binaryToDecimal(string);
	string decimalToBinary(int);
	void makeBranch(string&, char);
	void remakeTree();

public:
	Huffman(string, string);
	void writeCompress();
	void writeDecompress();
};