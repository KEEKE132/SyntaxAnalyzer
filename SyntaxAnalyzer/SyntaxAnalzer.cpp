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
    float value;
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
    else if (nextToken != SEMICOLON && nextToken != EOF) {
        warning("Semicolon이 없음 -> 삽입후 진행");
        cout << ";";
    }
}

float findValue(string data) {
    if (isalpha(data[0])) {
        for (id* k : ids ){
            if (k->name == data) {
                return k->value;
            }
        }
        cout << token_string;
        lexical();
        error("값이 존재하지 않은 식별자:" + data);
        return 0;
    }
    else if (all_of(data.begin(), data.end(), isdigit)) {
        return stoi(data);
    }
    else {
        cout << token_string;
        lexical();
        error("잘못된 요소:" + data);
        return 0;
    }
}

void stat() {
    if (nextToken != IDENT) {
        error("각 문장의 시작은 ID여야함");
    }
	iden(token_string);
    cout << token_string;
    IDnum++;
    lexical();
    OPnum++;
    if (nextToken != ASSIGN_OP) {
        warning("ASSIGN Operation이 존재하지않음 -> 삽입 후 진행; ");
        cout << ":=";
    }
    else {
        cout << token_string;
        lexical();
    }
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

float expr() {
    return term()+term_tail();
}

float term() {
    return factor() * factor_tail();
}


float term_tail() {
    float temp = 0;
    if (nextToken == ADD_OP) { 
        OPnum++;
        cout << token_string;
        lexical();
        temp+=term();
        temp+=term_tail();
    }
    else if (nextToken == SUB_OP) {
        OPnum++;
        cout << token_string;
        lexical();
        temp -= term();
        temp -= term_tail();
    }
    return temp;
}

float factor() {
    if (nextToken == LEFT_PAREN) {
        cout << token_string;
        lexical();
        float temp = expr();
        if (nextToken == RIGHT_PAREN) {
            cout << token_string;
            lexical();
        }
        else {
            error("RIGHT_PAREN이 없음");
        }
        return temp;
    }
    
    else if (nextToken == IDENT) {  
        IDnum++;
        float temp = findValue(token_string);
        cout << token_string;
        lexical();
        return temp;
    }
    else if(nextToken == INT_LIT){
        CONSTnum++;
        float temp = findValue(token_string);
        cout << token_string;
        lexical();
        return temp;
    }
    else if (nextToken == EOF) {
        error("완성되지 않은 문장");
    }
    else {
        warning("잘못된 요소(" + token_string + ") -> 제거후 진행; ");
        cout << token_string;
        lexical();
        factor();
    }
}

float factor_tail() {
    float temp = 1;
    float divOperand=1;
    if (nextToken == MULT_OP) {
        OPnum++;
        cout << token_string;
        lexical();
        temp *= factor();
        temp *= factor_tail();
    }
    else if (nextToken == DIV_OP) {
        OPnum++;
        cout << token_string;
        lexical();
        temp *= 100000;
        divOperand = factor();
        if (divOperand != 0) {
            temp /= divOperand;
        }
        else {
            warning("나눗셈 Operand가 0입니다 -> 1로 대체후 진행; ");
        }
        temp /= factor_tail();
        temp /= 100000;
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

void warning(string message) {
    warnMesaage.append(message);
    isWarn = true;
}

//해결과제: Warn, error 세분화적용