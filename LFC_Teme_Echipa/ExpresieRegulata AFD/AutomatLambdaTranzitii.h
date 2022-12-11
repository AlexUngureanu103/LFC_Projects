#pragma once
#include <vector> 
#include <string>
class AutomatLambdaTranzitii
{
	const char lambda = '#';
	std::vector< std::string>m_State;
	std::vector<char> m_entryAlphabet;
	std::string m_initialState;
	std::string m_FinState;
	std::vector<std::tuple<std::string, char, std::string>> m_transition;

public:
	AutomatLambdaTranzitii();
	AutomatLambdaTranzitii(const std::string& initialState, const char& entryAlphabet, const std::string& finalState);
	void setQ(const std::vector<std::string>& State);
	void setEntryAlphabet(const std::vector<char>& alphabet);
	void setInitialState(const std::string& state);
	void setFinState(const std::string& state);
	void setTransition(const std::vector<std::tuple<std::string, char, std::string>>& transition);
	
};

AutomatLambdaTranzitii& ConcatenateAutomates(AutomatLambdaTranzitii& firstAutomate, AutomatLambdaTranzitii& secondAutomate);

