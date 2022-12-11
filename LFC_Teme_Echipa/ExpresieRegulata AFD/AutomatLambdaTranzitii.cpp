#include "AutomatLambdaTranzitii.h"

AutomatLambdaTranzitii::AutomatLambdaTranzitii()
{
}

AutomatLambdaTranzitii::AutomatLambdaTranzitii(const std::string& initialState, const char& entryAlphabet, const std::string& finalState)
	:m_initialState{ initialState }, m_FinState{ finalState }
{
	this->m_entryAlphabet.push_back(entryAlphabet);
}

void AutomatLambdaTranzitii::setQ(const std::vector<std::string>& State)
{
	this->m_State = State;
}

std::vector<std::string> AutomatLambdaTranzitii::getQ() const
{
	return this->m_State;
}

void AutomatLambdaTranzitii::setEntryAlphabet(const std::vector<char>& alphabet)
{
	this->m_entryAlphabet = alphabet;
}

std::vector<char> AutomatLambdaTranzitii::getEntryAlphabet() const
{
	return this->m_entryAlphabet;
}


void AutomatLambdaTranzitii::setInitialState(const std::string& state)
{
	this->m_initialState = state;
}

std::string AutomatLambdaTranzitii::getInitialState() const
{
	return this->m_initialState;
}

void AutomatLambdaTranzitii::setFinState(const std::string& state)
{
	this->m_FinState = state;
}

std::string AutomatLambdaTranzitii::getFinState() const
{
	return this->m_FinState;
}

void AutomatLambdaTranzitii::setTransition(const std::vector<std::tuple<std::string, char, std::string>>& transition)
{
	this->m_transition = transition;
}

std::vector<AutomatLambdaTranzitii::Transition> AutomatLambdaTranzitii::getTransition() const
{
	return this->m_transition;
}

AutomatLambdaTranzitii& ConcatenateAutomates(AutomatLambdaTranzitii& firstAutomate, AutomatLambdaTranzitii& secondAutomate)
{
	AutomatLambdaTranzitii newAutomatLambdaTransiti;
	std::string newInitialState = firstAutomate.
}
