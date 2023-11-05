#include <iostream>
#include <fstream>
#include <vector>
#include "LexicalAnalzer.h"
#include "SyntaxAnalzer.h"

using namespace std;

extern ifstream file;
extern int nextToken;
extern int IDnum, OPnum, CONSTnum;
extern bool isError;
extern bool isWarn;
extern string warnMesaage;

typedef struct id {
	string name;
	int value;
	bool unknown;
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
			try {
				lookupSyntax();
				cout << endl;
			}
			catch (string message) {
				cout << endl << "(ERROR) " << message;
			}
			
			if (isWarn && !isError) {
				cout << endl;
			}
			else if (!isWarn && !isError) {
				cout << endl << "(OK)";
			}
			cout << endl << "ID: " << IDnum << "; CONST: " << CONSTnum << "; OP: " << OPnum << ";";
			cout << endl;
		} while (nextToken != EOF);
	}
	cout << "Result -=> ";
	for (id* k : ids) {
		if (!k->unknown)
			cout << k->name << ":" << k->value << "; ";
		else {
			cout << k->name << ": UNKNOWN; ";
		}
	}
	return 0;
}