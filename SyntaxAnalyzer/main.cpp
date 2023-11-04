#include <iostream>
#include <fstream>
#include "LexicalAnalzer.h"
#include "SyntaxAnalzer.h"

using namespace std;

extern ifstream file;
extern int nextToken;

int main(int argc, char* argv[]) {
	//string filename = argv[1];
	file.open("test.txt");
	if (!file.is_open()) {
		cout << "ERROR - file is not open" << endl;
	}
	else {
		getChar();
		do {
			lookupSyntax();
		} while (nextToken != EOF);
	}
	return 0;
}