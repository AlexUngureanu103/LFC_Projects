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

void AutomatLambdaTranzitii::setEntryAlphabet(const std::vector<char>& alphabet)
{
	this->m_entryAlphabet = alphabet;
}

void AutomatLambdaTranzitii::setInitialState(const std::string& state)
{
	this->m_initialState = state;
}

void AutomatLambdaTranzitii::setFinState(const std::string& state)
{
	this->m_FinState = state;
}

void AutomatLambdaTranzitii::setTransition(const std::vector<std::tuple<std::string, char, std::string>>& transition)
{
	this->m_transition = transition;
}

AutomatLambdaTranzitii& ConcatenateAutomates(AutomatLambdaTranzitii& firstAutomate, AutomatLambdaTranzitii& secondAutomate)
{
	AutomatLambdaTranzitii newAutomatLambdaTransiti;

}
