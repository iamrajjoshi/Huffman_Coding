#include "Huffman.h"

void Huffman::create_freq_table() {
	for (int i = 0; i < 256; i++) {
		freq_table[i] = new huffman_node;
		freq_table[i]->id = i;
		freq_table[i]->freq = 0;
	}
	return;
}

void Huffman::traverse(huffman_node* node, string code) {
	if (node->left == nullptr && node->right == nullptr)
		node->code = code;
	else {
		traverse(node->left, code + '0');
		traverse(node->right, code + '1');
	}
	return;
}

int Huffman::binary_to_decimal(string in) {
	int result = 0;
	for (int i = 0; i < in.size(); i++)
		result = result * 2 + in[i] - '0';
	return result;
}

string Huffman::decimal_to_binary(int in) {
	string temp = "";
	string result = "";
	while (in) {
		temp += ('0' + in % 2);
		in /= 2;
	}
	result.append(8 - temp.size(), '0');//append '0' ahead to let the result become fixed length of 8
	for (int i = temp.size() - 1; i >= 0; i--)
		result += temp[i];
	return result;
}

void Huffman::build_tree(string& path, char a_code) {//build a new branch according to the input code and ignore the already existed branches
	huffman_node* current = root;
	for (int i = 0; i < path.size(); i++) {
		if (path[i] == '0') {
			if (current->left == nullptr)
				current->left = new huffman_node;
			current = current->left;
		}
		else if (path[i] == '1') {
			if (current->right == nullptr)
				current->right = new huffman_node;
			current = current->right;
		}
	}
	current->id = a_code;//attach id to the leaf
	return;
}

Huffman::Huffman(string in, string out) {
	inFileName = in;
	outFileName = out;
	create_freq_table();
}

void Huffman::create_pq() {
	inFile.open(inFileName, ios::in);
	inFile.get(id);
	while (!inFile.eof()) {
		freq_table[id]->freq++;
		inFile.get(id);
	}
	inFile.close();
	for (int i = 0; i < 256; i++)
		if (freq_table[i]->freq)
			pq.push(freq_table[i]);
	return;
}

void Huffman::create_huffman_tree() {
	priority_queue<huffman_node*, vector<huffman_node*>, Comparator> temp(pq);
	while (temp.size() > 1) {//create the huffman tree with highest frequecy characher being leaf from bottom to top
		root = new huffman_node;
		root->freq = 0;
		root->left = temp.top();
		root->freq += temp.top()->freq;
		temp.pop();
		root->right = temp.top();
		root->freq += temp.top()->freq;
		temp.pop();
		temp.push(root);
	}
	return;
}

void Huffman::calculate_huffman_codes() {
	traverse(root, "");
	return;
}

void Huffman::coding_save() {
	inFile.open(inFileName, ios::in);
	outFile.open(outFileName, ios::out | ios::binary);
	string in = "", s = "";

	in += (char)pq.size();//the first byte saves the size of the priority queue
	priority_queue<huffman_node*, vector<huffman_node*>, Comparator> temp(pq);
	while (!temp.empty()) {//get all characters and their huffman codes for output
		huffman_node* current = temp.top();
		in += current->id;
		s.assign(127 - current->code.size(), '0');//set the codes with a fixed 128-bit string form[000бнбн1 + real code]
		s += '1';//'1' indicates the start of huffman code
		s.append(current->code);
		in += (char)binary_to_decimal(s.substr(0, 8));
		for (int i = 0; i < 15; i++) {//cut into 8-bit binary codes that can convert into saving char needed for binary file
			s = s.substr(8);
			in += (char)binary_to_decimal(s.substr(0, 8));
		}
		temp.pop();
	}
	s.clear();

	inFile.get(id);
	while (!inFile.eof()) {//get the huffman code
		s += freq_table[id]->code;
		while (s.size() > 8) {//cut into 8-bit binary codes that can convert into saving char needed for binary file
			in += (char)binary_to_decimal(s.substr(0, 8));
			s = s.substr(8);
		}
		inFile.get(id);
	}
	int count = 8 - s.size();
	if (s.size() < 8)//append number of 'count' '0' to the last few codes to create the last byte of text
		s.append(count, '0');
	in += (char)binary_to_decimal(s);//save number of 'count' at last
	in += (char)count;

	outFile.write(in.c_str(), in.size());
	outFile.close();

	inFile.close();
	inFile.open(inFileName);
	inFile.seekg(0, ios::end);
	long unsigned int in_size = (long unsigned int)inFile.tellg();
	cout << "Initial File Size: " << in_size << " bytes" << endl << endl;
	inFile.close();
	
	inFile.open(outFileName);
	inFile.seekg(0, ios::end);
	long unsigned int out_size = (long unsigned int)inFile.tellg();
	cout << "Compressed File Size: " << out_size << " bytes" << endl << endl;
	double ratio = double(out_size) / in_size;
	cout << "Compression Ratio: " << ratio * 100 << "%" << endl;
	inFile.close();
	return;
}

void Huffman::recreate_huffman_tree() {
	inFile.open(inFileName, ios::in | ios::binary);
	unsigned char size;//unsigned char to get number of node of humman tree
	inFile.read(reinterpret_cast<char*>(&size), 1);
	root = new huffman_node;
	for (int i = 0; i < size; i++) {
		char a_code;
		unsigned char h_code_c[16];//16 unsigned char to obtain the binary code
		inFile.read(&a_code, 1);
		inFile.read(reinterpret_cast<char*>(h_code_c), 16);
		string h_code_s = "";
		for (int i = 0; i < 16; i++)//obtain the oringinal 128-bit binary string
			h_code_s += decimal_to_binary(h_code_c[i]);
		int j = 0;
		while (h_code_s[j] == '0')//delete the added '000бнбн1' to get the real huffman code
			j++;
		h_code_s = h_code_s.substr(j + 1);
		build_tree(h_code_s, a_code);
	}
	inFile.close();
	return;
}

void Huffman::decoding_save() {
	inFile.open(inFileName, ios::in | ios::binary);
	outFile.open(outFileName, ios::out);
	unsigned char size;//get the size of huffman tree
	inFile.read(reinterpret_cast<char*>(&size), 1);
	inFile.seekg(-1, ios::end);//jump to the last one byte to get the number of '0' append to the string at last
	char count0;
	inFile.read(&count0, 1);
	inFile.seekg(1 + 17 * size, ios::beg);//jump to the position where text starts

	vector<unsigned char> text;
	unsigned char textseg;
	inFile.read(reinterpret_cast<char*>(&textseg), 1);
	while (!inFile.eof()) {//get the text byte by byte using unsigned char
		text.push_back(textseg);
		inFile.read(reinterpret_cast<char*>(&textseg), 1);
	}
	huffman_node* current = root;
	string path;
	for (int i = 0; i < text.size() - 1; i++) {//translate the huffman code
		path = decimal_to_binary(text[i]);
		if (i == text.size() - 2)
			path = path.substr(0, 8 - count0);
		for (int j = 0; j < path.size(); j++) {
			if (path[j] == '0')
				current = current->left;
			else
				current = current->right;
			if (current->left == nullptr && current->right == nullptr) {
				outFile.put(current->id);
				current = root;
			}
		}
	}
	inFile.close();
	outFile.close();
	return;
}
