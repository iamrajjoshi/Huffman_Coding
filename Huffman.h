#pragma once

#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

struct huffman_node {
	char id;//character 
	int freq;//frequency of the character
	string code;//huffman code for the character
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
	void createPQ();
	void setCode(huffman_node*, string);//traverse the huffman tree and get huffman code for a character
	int binaryToDecimal(string);//convert a 8-bit 0/1 string of binary code to a decimal integer 
	string decimalToBinary(int);//convert a decimal integer to a 8-bit 0/1 string of binary code
	void buildBranch(string&, char);//build the huffman tree according to information from file 
	void makeTree();
	void remakeTree();
public:
	Huffman(string, string);
	void writeCompress();
	void writeDecompress();
};