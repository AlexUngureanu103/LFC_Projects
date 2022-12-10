#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include "Finite_Automaton.h"

class Grammar
{
private:
	std::vector<char> VNonterminal;
	std::vector<char> VTerminal;
	char Start;
	char m_lambda;
	std::vector<std::pair<std::string, std::string>> Productions;
public:
	Grammar();
	bool checkGrammar();
	std::string generateWord();
	friend std::istream& operator >> (std::istream& in, Grammar& gram);
	friend std::ostream& operator <<(std::ostream& out, const  Grammar& gram);
	bool isRegular();
	Finite_Automaton createAFN();
	~Grammar();
};

