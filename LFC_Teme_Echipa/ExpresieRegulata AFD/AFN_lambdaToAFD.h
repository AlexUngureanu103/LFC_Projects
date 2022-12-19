#pragma once
#include "Grammar.h"
#include "AutomatLambdaTranzitii.h"
#include <stack>


void findNextStates(std::string& initial, const std::vector<std::tuple<std::string, char, std::string>>& transitionsLambda, const char alphabetCharacter);

Finite_Automaton fromAFNLambdaToAFD(const AutomatLambdaTranzitii& automatLambda);
