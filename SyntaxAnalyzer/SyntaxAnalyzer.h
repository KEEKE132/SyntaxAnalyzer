#pragma once
#include <string>

using namespace std;

void stat();
float expr();
float term();
float factor();
float term_tail();
float factor_tail();
void lookupSyntax();
void iden(string);
void error(string);
void warning(string);