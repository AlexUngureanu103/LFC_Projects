#pragma once
#include<vector>
#include <string>
#include <iostream>
#include <queue>
#include <stack>

class Push_Down_Automaton
{
	std::vector<char>m_States, m_stackStates;
	std::vector<char> entry_alphabet;
	char initial_state, initialStackState, m_lambda;
	std::vector<char> Fin_state;
	std::vector<std::tuple<char, char, char>> transition;
	std::vector<std::pair<char, std::string>> transitionResults;

public:
	using Transition = std::tuple<char, char, char>;
	using TransitonResults = std::pair<char, std::string>;
	void setEntryAlphabet(const std::vector<char>& alphabet);
	void setInitialState(const char& state);
	void setFinState(const std::vector<char>& state);
	void setStates(const std::vector<char>& states);
	void setTransition(const std::vector<std::tuple<char, char, char>>& transition);
	void setTransitionResults(const std::vector<std::pair<char, std::string>>& transitionResults);
	void setStackStates(const std::vector<char>& stackStates);
	void setLambda(const char& lambda);
	void setInitialStackState(const char& initialStackState);

	friend std::ostream& operator <<(std::ostream& out, Push_Down_Automaton& automat);
	bool checkWord(std::string word);
	bool isDeterministic();
	~Push_Down_Automaton() = default;
};

