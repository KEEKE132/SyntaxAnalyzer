#include <iostream>
#include <fstream>
#include <vector>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"

using namespace std;

extern ifstream file;
extern int nextToken;
extern int IDnum, OPnum, CONSTnum;
extern bool isError;
extern bool isWarn;
extern string warnMesaage;

typedef struct id {
	string name;
	float value;
	bool unknown;
}id;
extern vector<id*> ids;

int main(int argc, char* argv[]) {
	string filename = argv[1];
	file.open(filename);
	if (!file.is_open()) {
		cout << "ERROR - file is not open" << endl;
		return 0;
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
				cout << endl;
				if (isWarn) {
					cout << "(Warning)" << warnMesaage << "; ";
				}
				cout << "(ERROR) " << message;
			}
			
			if (isWarn && !isError) {
				cout << "(Warning)" << warnMesaage;
			}
			else if (!isWarn && !isError) {
				cout << "(OK)";
			}
			cout << endl << "ID: " << IDnum << "; CONST: " << CONSTnum << "; OP: " << OPnum << ";";
			cout << endl;
		} while (nextToken != EOF);
	}
	cout << "Result -=> ";
	for (id* k : ids) {
		if (!k->unknown)
			cout << k->name << ":" << ((int)(k->value*100))/(float)100 << "; ";
		else {
			cout << k->name << ": UNKNOWN; ";
		}
	}
	return 0;
}