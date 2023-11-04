#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include "LexicalAnalzer.h"
#include "SyntaxAnalzer.h"

using namespace std;

#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define SEMICOLON 27

extern int nextToken;
extern string token_string;

typedef struct id {
	string name;
	int value;
}id;

vector<id*> ids;
int idCount = 0;
int targetid = 0;

void lookupSyntax() {
    stat();
	if (nextToken == SEMICOLON) {
		lexical();
	}
}

int findValue(string data) {
    if (isalpha(data[0])) {
        for (id* k : ids ){
            if (k->name == data) {
                return k->value;
            }
        }
        cout << "no exist return 0" << endl;
        return 0;
    }
    else if (all_of(data.begin(), data.end(), isdigit)) {
        return stoi(data);
    }
    else {
        cout << "wrong input return 0";
        return 0;
    }
}

void stat() {
	iden(token_string);
    lexical();
    if (nextToken != ASSIGN_OP) {
        cout << "error" << endl;
    }
    lexical();
    ids[targetid]->value = expr();
}

void iden(string na) {
    for (int i = 0; i<ids.size() ; i++) {
        if (ids[i]->name == na) {
            targetid = i;
            return;
        }
    }
    ids.push_back(new id);
    ids[idCount]->name = na;
    targetid = idCount;
    idCount++;
}

int expr() {
    return term()+term_tail();
}

int term() {
    return factor() * factor_tail();
}


int term_tail() {
    int temp = 0;
    if (nextToken == ADD_OP) { 
        lexical();
        temp+=term();
        temp+=term_tail();
    }
    else if (nextToken == SUB_OP) {
        lexical();
        temp -= term();
        temp -= term_tail();
    }
    return temp;
}

int factor() {
    if (nextToken == LEFT_PAREN) {
        lexical();
        int temp = expr();
        if (nextToken == RIGHT_PAREN) {
            lexical();
        }
        return temp;
    }
    
    else if (nextToken == IDENT) {  
        int temp = findValue(token_string);
        lexical();
        return temp;
    }
    else { // CONST
        int temp = findValue(token_string);
        lexical();
        return temp;
    }
}

int factor_tail() {
    int temp = 1;
    if (nextToken == MULT_OP) {
        lexical();
        temp *= factor();
        temp *= factor_tail();
    }
    else if (nextToken == DIV_OP) {
        
    }
    return temp;
}