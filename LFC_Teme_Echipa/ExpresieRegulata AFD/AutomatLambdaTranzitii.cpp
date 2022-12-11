#include "AutomatLambdaTranzitii.h"

AutomatLambdaTranzitii::AutomatLambdaTranzitii()
{
}

AutomatLambdaTranzitii::AutomatLambdaTranzitii(const std::string& initialState, const char& entryAlphabet, const std::string& finalState)
	:m_initialState{ initialState }, m_FinState{ finalState }
{
	this->m_entryAlphabet.push_back(entryAlphabet);
	m_transition.emplace_back(std::make_tuple(initialState, entryAlphabet, finalState));
	m_State.push_back(initialState);
	m_State.push_back(finalState);
}

char AutomatLambdaTranzitii::getLambda() const
{
	return k_lambda;
}

void AutomatLambdaTranzitii::setStates(const std::vector<std::string>& State)
{
	this->m_State = State;
}

std::vector<std::string> AutomatLambdaTranzitii::getStates() const
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

AutomatLambdaTranzitii ConcatenateAutomates(const AutomatLambdaTranzitii& firstAutomate, const AutomatLambdaTranzitii& secondAutomate )
{
	//TO DO ?
	// Eliminat tanzitia cu lambda din automatul 
	// Si facut tranformarea starii finale din primul automat in starea initiala a celui de-al doilea automat
	// practic acele stari se unesc si devin una singura 
	
	AutomatLambdaTranzitii newAutomatLambdaTransiti;
	std::string newInitialState = firstAutomate.getInitialState();
	std::string newFinalState = secondAutomate.getFinState();
	
	AutomatLambdaTranzitii::Transition newTransition = std::make_tuple(firstAutomate.getFinState() , newAutomatLambdaTransiti.getLambda(), secondAutomate.getInitialState()); 
	
	newAutomatLambdaTransiti.setInitialState(newInitialState); // initial state is first automate initial state
	newAutomatLambdaTransiti.setFinState(newFinalState); // final state is second automate final state
	std::vector<AutomatLambdaTranzitii::Transition> newTransitionVector;
	
	//states are the union of the two automates states
	std::vector< std::string> newStates;
	newStates = firstAutomate.getStates();
	for (const auto& state : secondAutomate.getStates())
	{
		newStates.push_back(state);
	}
	newAutomatLambdaTransiti.setStates(newStates);
	
	// transitions are every transition of the first automate and the second automate 
	newTransitionVector = firstAutomate.getTransition();
	newTransitionVector.push_back(newTransition);
	for (const auto& transition : secondAutomate.getTransition())
	{
		newTransitionVector.push_back(transition);
	}
	newAutomatLambdaTransiti.setTransition(newTransitionVector);

	//entry alphabet is the union of the two automates entry alphabets
	std::vector<char> newEntryAlphabet;
	newEntryAlphabet = firstAutomate.getEntryAlphabet();
	for (const auto& entryAlphabet : secondAutomate.getEntryAlphabet())
	{
		newEntryAlphabet.push_back(entryAlphabet);
	}
	newAutomatLambdaTransiti.setEntryAlphabet(newEntryAlphabet);
	return newAutomatLambdaTransiti;
}
