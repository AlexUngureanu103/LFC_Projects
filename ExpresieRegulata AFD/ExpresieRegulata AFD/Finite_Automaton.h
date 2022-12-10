#pragma once
#include<vector>
#include <string>
#include <iostream>

class Finite_Automaton
{
	std::vector< char>m_State;
	std::vector<char> entry_alphabet;
	char initial_state;
	std::vector<char> Fin_state;
	std::vector<std::tuple<char, char,char>> transition;
public:
	bool verifyAutomaton();
	void setQ(const std::vector<char>& State);
	void setEntryAlphabet(const std::vector<char>& alphabet);
	void setInitialState(const char& state);
	void setFinState(const std::vector<char>& state);
	void setTransition(const std::vector<std::tuple<char, char, char>>& transition);
	friend std::ostream& operator <<(std::ostream& out, const  Finite_Automaton& automat);
	bool checkWord(std::string word);
	bool isDeterministic();
	~Finite_Automaton() = default;
};

