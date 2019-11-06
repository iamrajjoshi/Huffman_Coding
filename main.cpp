#include "Huffman.h"
#include <chrono>
using namespace std::chrono;

int decode(string arg1, string arg2) {
	Huffman h(arg1, arg2);
	h.recreate_huffman_tree();
	h.decoding_save();
	cout << endl;
	return 0;
}

int encode(string arg1, string arg2) {
	Huffman h(arg1, arg2);
	h.create_pq();
	h.create_huffman_tree();
	h.calculate_huffman_codes();
	h.coding_save();
	cout << endl;
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc == 4) {
		string arg = argv[1];
		string inFileName = argv[2];
		string outFileName = argv[3];
		if (arg == "-e") {
			cout << "Starting Compression..." << endl;
			cout << "---------------------------------" << endl;
			auto start = high_resolution_clock::now();
			encode(inFileName, outFileName);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(stop - start);
			cout << "Compression Completed!" << endl;
			cout << "---------------------------------" << endl;
			cout << "Execution Time: " << duration.count() << " milliseconds" << endl << endl;
		}
		else if (arg == "-d") {
			cout << "Starting Decompression..." << endl;
			cout << "---------------------------------";
			auto start = high_resolution_clock::now();
			decode(inFileName, outFileName);
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<milliseconds>(stop - start);
			cout << "Decompression Completed!" << endl;
			cout << "---------------------------------" << endl;
			cout << "Execution Time: " << duration.count() << " milliseconds" << endl << endl;
		}
	}
	else {
		cout << "Incorrect Use! " << endl;
		cout << "Usage: " << "-e/d inputFileName outputFileName" << endl;
		return -1;
	}
	return 0;
}