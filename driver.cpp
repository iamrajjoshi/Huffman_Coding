#include "Header.h"

int freqTable[255] = { 0 };
priority_queue < node*, vector<node*>, Comparator> pq;
codeContainer* container;

string fileData; //entire content of file
string inFileName;
string outFileName;

int numCodes;

int main(int argc, char* argv[]) {
	if (argc == 4) {
		string arg = argv[1];
		inFileName = argv[2];
		outFileName = argv[3];
		if (arg == "-e") {
			cout << "Starting Compression..." << endl << endl;
			auto start = high_resolution_clock::now();
			compress();
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<seconds>(stop - start);
			cout << "Compression Completed!" << endl << endl;
			cout << "Execution Time: " << duration.count() << " seconds" << endl << endl;
		}
		else if (arg == "-d") {
			decompress();
		}
	}
	else {
		cout << "Incorrect Use! " << endl;
		cout << "Usage: " << "-e/d inputFileName outputFileName" << endl;
		return -1;
	}
	return 0;
}