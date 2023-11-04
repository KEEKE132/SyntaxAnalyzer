#pragma once
#include <string>

using namespace std;

void stat();
int expr();
int term();
int factor();
int term_tail();
int factor_tail();
void lookupSyntax();
void iden(string);