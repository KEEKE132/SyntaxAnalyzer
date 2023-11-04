#include <iostream>
#include <fstream>
#include <vector>
#include "LexicalAnalzer.h"
#include "SyntaxAnalzer.h"

using namespace std;

extern ifstream file;
extern int nextToken;
typedef struct id {
	string name;
	int value;
}id;
extern vector<id*> ids;

int main(int argc, char* argv[]) {
	//string filename = argv[1];
	file.open("test.txt");
	if (!file.is_open()) {
		cout << "ERROR - file is not open" << endl;
	}
	else {
		getChar();
		lexical();
		do {
			lookupSyntax();
		} while (nextToken != EOF);
	}
	for (id* k : ids) {
		cout << k->name << ":" << k->value << endl;
	}
	return 0;
}