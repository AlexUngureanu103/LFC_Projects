#pragma once
#include <vector> 
#include <string>
class AutomatLambdaTranzitii
{
	const char k_lambda = '#';
	std::vector< std::string>m_State;
	std::vector<char> m_entryAlphabet;
	std::string m_initialState;
	std::string m_FinState;
	std::vector<std::tuple<std::string, char, std::string>> m_transition;

public:
	using Transition = std::tuple<std::string, char, std::string>;
	AutomatLambdaTranzitii();
	AutomatLambdaTranzitii(const std::string& initialState, const char& entryAlphabet, const std::string& finalState);
	char  getLambda() const;
	void setStates(const std::vector<std::string>& State);
	std::vector<std::string> getStates() const;
	void setEntryAlphabet(const std::vector<char>& alphabet);
	std::vector<char> getEntryAlphabet() const;
	void setInitialState(const std::string& state);
	std::string getInitialState() const;
	void setFinState(const std::string& state);
	std::string getFinState() const;
	void setTransition(const std::vector<std::tuple<std::string, char, std::string>>& transition);
	std::vector<Transition> getTransition() const;

};

AutomatLambdaTranzitii ConcatenateAutomates(const AutomatLambdaTranzitii& firstAutomate, const  AutomatLambdaTranzitii& secondAutomate);
AutomatLambdaTranzitii InchidereaKleene(const AutomatLambdaTranzitii& automate, uint16_t& cntStari, const std::string& q_statePattern);
