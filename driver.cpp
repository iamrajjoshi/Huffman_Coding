#include <chrono>
#include <iostream>
#include <string>

using namespace std::chrono;
using namespace std;
void compress();

int main(int argc, char* argv[]) {
	if (argc == 2) {
		string arg = argv[1];
		if (arg == "-e") {
			cout << "Starting Compression..." << endl << endl;
			auto start = high_resolution_clock::now();
			compress();
			auto stop = high_resolution_clock::now();
			auto duration = duration_cast<seconds>(stop - start);
			cout << "Compression Completed!" << endl << endl;
			cout << "Execution Time: " << duration.count() << " seconds" << endl << endl;
		}
		else if (argv[1] == "-d") {
			;
		}
	}
	else {
		cout << "Incorrect Number of Arguments! " << endl;
		return -1;
	}
	return 0;
}