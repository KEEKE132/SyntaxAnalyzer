#include <iostream>
#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include <string>
#include <fstream>

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

using namespace std;

int charClass;
string lexeme;
string token_string;
char nextChar;
int lexLen;
int token;
int nextToken;
ifstream file;

int lookup(char ch) {
	switch (ch) {
	case '(':
		addChar();
		nextToken = LEFT_PAREN;
		break;
	case ')':
		addChar();
		nextToken = RIGHT_PAREN;
		break;
	case '+':
		addChar();
		nextToken = ADD_OP;
		break;
	case '-':
		addChar();
		getChar();
		if (charClass == DIGIT) {
			while (charClass == DIGIT) {
				addChar();
				getChar();
			}
			nextToken = INT_LIT;
		}
		else {
			nextToken = SUB_OP;
		}
		break;
	case '*':
		addChar();
		nextToken = MULT_OP;
		break;
	case '/':
		addChar();
		nextToken = DIV_OP;
		break;
	case ':':
		addChar();
		getChar();
		if (nextChar == '=') {
			addChar();
			nextToken = ASSIGN_OP;
		}
		else {
			warning("잘못된 Assignment OP(:) -> := 으로 대체");
			lexeme.push_back('=');
			lexLen++;
			nextToken = ASSIGN_OP;
		}
		break;
	case '=':
		warning("잘못된 Assignment OP(=) -> := 으로 대체");
		lexeme.push_back(':');
		lexLen++;
		addChar();
		getChar();
		nextToken = ASSIGN_OP;
		break;
	case ';':
		addChar();
		nextToken = SEMICOLON;
		break;
	default:
		addChar();
		nextToken = UNKNOWN;
		break;
	}
	return nextToken;
}

void addChar() {
	lexeme.push_back(nextChar);
	lexLen++;
}

void getChar() {
	file.get(nextChar);
	if (!file.eof()) {
		if (isalpha(nextChar)) {
			charClass = LETTER;
		}
		else if (isdigit(nextChar)) {
			charClass = DIGIT;
		}
		else {
			charClass = UNKNOWN;
		}
	}
	else {
		charClass = EOF;
	}
}

void getNonBlank() {
	while (nextChar <= 32) {
		getChar();
		if (charClass == EOF) {
			nextToken = EOF;
			return;
		}
	}
}

int lexical() {
	if (nextToken == EOF) {
		return EOF;
	}
	lexLen = 0;
	lexeme.clear();
	getNonBlank();
	switch (charClass) {
	case LETTER:
		addChar();
		getChar();
		while (charClass == LETTER || charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = IDENT;
		break;
	case DIGIT:
		addChar();
		getChar();
		while (charClass == DIGIT) {
			addChar();
			getChar();
		}
		nextToken = INT_LIT;
		break;
	case UNKNOWN :
		lookup(nextChar);
		getChar();
		break;
	case EOF:
		nextToken = EOF;
		lexeme.append(" ");
		break;
	}
	token_string = lexeme;
	return nextToken;
}