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
    bool unknown = true;
}id;

vector<id*> ids;
int idCount = 0;
int targetid = 0;
int IDnum, CONSTnum,  OPnum;
bool isError = false;
bool isWarn = false;
string warnMesaage;


void lookupSyntax() {
    IDnum = 0;
    CONSTnum = 0;
    OPnum = 0;
    isError = false;
    isWarn = false;
    warnMesaage.clear();
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
        warnMesaage.append("값이 존재하지 않은 식별자 -> 0으로 대체");
        return 0;
    }
    else if (all_of(data.begin(), data.end(), isdigit)) {
        return stoi(data);
    }
    else {
        warnMesaage.append("잘못된 요소 -> 0으로 대체");
        return 0;
    }
}

void stat() {
	iden(token_string);
    cout << token_string;
    IDnum++;
    lexical();
    OPnum++;
    if (nextToken != ASSIGN_OP) {
        warnMesaage.append("ASSIGN Operation이 존재하지않음 -> 삽입 후 진행");
        cout << ":=(W)" << endl;
    }
    else {
        cout << token_string;
        lexical();
    }
    cout << token_string;
    ids[targetid]->value = expr();
    ids[targetid]->unknown = false;
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
        OPnum++;
        lexical();
        cout << token_string;
        temp+=term();
        temp+=term_tail();
    }
    else if (nextToken == SUB_OP) {
        OPnum++;
        lexical();
        cout << token_string;
        temp -= term();
        temp -= term_tail();
    }
    return temp;
}

int factor() {
    if (nextToken == LEFT_PAREN) {
        lexical();
        cout << token_string;
        int temp = expr();
        if (nextToken == RIGHT_PAREN) {
            lexical();
            cout << token_string;
        }
        else {
            error("RIGHT_PAREN이 없음");
        }
        return temp;
    }
    
    else if (nextToken == IDENT) {  
        IDnum++;
        int temp = findValue(token_string);
        lexical();
        cout << token_string;
        return temp;
    }
    else if(nextToken == INT_LIT){
        CONSTnum++;
        int temp = findValue(token_string);
        lexical();
        cout << token_string;
        return temp;
    }
    else {
        warnMesaage.append("잘못된 요소(" + token_string + ") -> 제거후 진행");
    }
}

int factor_tail() {
    int temp = 1;
    if (nextToken == MULT_OP) {
        OPnum++;
        lexical();
        cout << token_string;
        temp *= factor();
        temp *= factor_tail();
    }
    else if (nextToken == DIV_OP) {
        OPnum++;
    }
    return temp;
}

void error(string message) {
    isError = true;
    while (nextToken != SEMICOLON && nextToken != EOF) {
        if (nextToken == IDENT) {
            IDnum++;
        }
        else if (nextToken == INT_LIT) {
            CONSTnum++;
        }
        else if (nextToken == ASSIGN_OP || nextToken == MULT_OP || nextToken == DIV_OP || nextToken == ADD_OP || nextToken == SUB_OP) {
            OPnum++;
        }
        cout << token_string;
        lexical();
    }
    lexical();
    throw message;
}

//해결과제: 나눗셈, Warn, error 세분화적용