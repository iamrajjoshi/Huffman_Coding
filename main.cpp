#include "Huffman.h"
#include <chrono>
using namespace std::chrono;

void decode(string arg1, string arg2) {
	Huffman h(arg1, arg2);
	h.writeDecompress();
	return;
}

void encode(string arg1, string arg2) {
	Huffman h(arg1, arg2);
	h.writeCompress();
	return;
}

int main(int argc, char* argv[]) {
	if (argc == 4) {
		string arg = argv[1];
		string inFileName = argv[2];
		string outFileName = argv[3];
		if (arg == "-e") {
			cout << "Starting Compression..." << endl;
			auto start = high_resolution_clock::now();
			encode(inFileName, outFileName);
			auto stop = high_resolution_clock::now();
			auto dur = stop - start;
			auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
			cout << "Compression Completed!" << endl;
			cout << "Execution Time: " << f_secs.count() << " seconds" << endl;
		}
		else if (arg == "-d") {
			cout << "Starting Decompression..." << endl;
			auto start = high_resolution_clock::now();
			decode(inFileName, outFileName);
			auto stop = high_resolution_clock::now();
			auto dur = stop - start;
			auto f_secs = std::chrono::duration_cast<std::chrono::duration<float>>(dur);
			cout << "Decompression Completed!" << endl;
			cout << "Execution Time: " << f_secs.count() << " seconds" << endl;
		}
	}
	else {
		cout << "Incorrect Use! " << endl;
		cout << "Usage: " << "-e/d inputFileName outputFileName" << endl;
		return -1;
	}
	return 0;
}