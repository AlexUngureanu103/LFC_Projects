#pragma once
#include <unordered_set>
#include <fstream>
#include "AutomatLambdaTranzitii.h"
#include <stack>

uint16_t prec(const char& character);
bool verifAlphaNum(const char& character);
std::vector<char > CitireFormaPoloneza(std::string expresieRegulata);

AutomatLambdaTranzitii transformareFormaPolonezaInAutomatLambdaTranzitii(std::vector<char>& formaPoloneza);

