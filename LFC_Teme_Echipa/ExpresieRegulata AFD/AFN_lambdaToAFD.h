#pragma once
#include "AutomatLambdaTranzitii.h"
#include "Deterministic_Finite_Automaton.h"
#include <stack>
#include <algorithm>

void findNextStates(std::string& initial, const std::vector<std::tuple<std::string, char, std::string>>& transitionsLambda, const char alphabetCharacter);

Deterministic_Finite_Automaton fromAFNLambdaToAFD(const AutomatLambdaTranzitii& automatLambda);
