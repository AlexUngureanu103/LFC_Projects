#include "AutomatLambdaTranzitii.h"

AutomatLambdaTranzitii::AutomatLambdaTranzitii()
{
}

AutomatLambdaTranzitii::AutomatLambdaTranzitii(const std::string& initialState, const char& entryAlphabet, const std::string& finalState)
	:m_initialState{ initialState }, m_FinState{ finalState }
{
	this->m_entryAlphabet.insert(entryAlphabet);
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

void AutomatLambdaTranzitii::setEntryAlphabet(const std::unordered_set<char>& alphabet)
{
	this->m_entryAlphabet = alphabet;
}

std::unordered_set<char> AutomatLambdaTranzitii::getEntryAlphabet() const
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

AutomatLambdaTranzitii ConcatenateAutomates(const AutomatLambdaTranzitii& firstAutomate, const AutomatLambdaTranzitii& secondAutomate)
{
	AutomatLambdaTranzitii newAutomatLambdaTransiti;
	std::string newInitialState = firstAutomate.getInitialState();
	std::string newFinalState = secondAutomate.getFinState();

	newAutomatLambdaTransiti.setInitialState(newInitialState); // initial state is first automate initial state
	newAutomatLambdaTransiti.setFinState(newFinalState); // final state is second automate final state

	std::vector< std::string> newStates; //states are the union of the two automates states
	newStates.reserve(firstAutomate.getStates().size() + secondAutomate.getStates().size() - 1);
	for (uint16_t index = 0; index < firstAutomate.getStates().size() - 1; index++)
	{
		newStates.push_back(firstAutomate.getStates()[index]);
	}
	std::string IntermediateState = firstAutomate.getFinState() + secondAutomate.getInitialState();
	newStates.push_back(IntermediateState);
	for (uint16_t index = 1; index < secondAutomate.getStates().size(); index++)
	{
		newStates.push_back(secondAutomate.getStates()[index]);
	}
	newAutomatLambdaTransiti.setStates(newStates);

	std::vector<AutomatLambdaTranzitii::Transition> newTransitionVector; // transitions are every transition of the first automate and the second automate 
	newTransitionVector = firstAutomate.getTransition();
	for (const auto& transition : secondAutomate.getTransition())
	{
		newTransitionVector.push_back(transition);
	}
	for (auto& transition : newTransitionVector)
	{
		auto& [firstState, character, secondState] = transition;
		if (secondState == firstAutomate.getFinState())
		{
			secondState = IntermediateState;
		}
		if (firstState == secondAutomate.getInitialState())
		{
			firstState = IntermediateState;
		}
	}
	newAutomatLambdaTransiti.setTransition(newTransitionVector);

	std::unordered_set<char> newEntryAlphabet;	//entry alphabet is the union of the two automates entry alphabets
	newEntryAlphabet = firstAutomate.getEntryAlphabet();
	for (const auto& entryAlphabet : secondAutomate.getEntryAlphabet())
	{
		newEntryAlphabet.insert(entryAlphabet);
	}
	newAutomatLambdaTransiti.setEntryAlphabet(newEntryAlphabet);
	return newAutomatLambdaTransiti;
}

AutomatLambdaTranzitii InchidereaKleene(const AutomatLambdaTranzitii& automate, uint16_t& cntStari, const std::string& q_statePattern)
{
	AutomatLambdaTranzitii newAutomatLambdaTransiti;
	std::string q_state = q_statePattern + std::to_string(cntStari);
	cntStari++;
	std::string q_state2 = q_statePattern + std::to_string(cntStari);
	cntStari++;

	std::string newInitialState = q_state;
	std::string newFinalState = q_state2;
	newAutomatLambdaTransiti.setInitialState(newInitialState); // initial state is q_state
	newAutomatLambdaTransiti.setFinState(newFinalState); // final state is q_state2

	std::vector< std::string> newStates; //states are the union of the automate states and q_state and q_state2
	newStates.reserve(automate.getStates().size() + 2);
	newStates.push_back(q_state);
	for (uint16_t index = 0; index < automate.getStates().size(); index++)
	{
		newStates.push_back(automate.getStates()[index]);
	}
	newStates.push_back(q_state2);
	newAutomatLambdaTransiti.setStates(newStates);

	// transitions are every transition of the automate +
	// the lambda transitions from q_state to the initial state of the automate  , with the lambda transitions +
	// from the final state of the automate to q_state2 ,with lambda transitions +
	// from q_state to q_state2 , with lambda transitions +
	// from automate final state to automate initial state , with lambda transitions
	std::vector<AutomatLambdaTranzitii::Transition> newTransitionVector;
	newTransitionVector = automate.getTransition();
	newTransitionVector.push_back({ q_state, newAutomatLambdaTransiti.getLambda(), automate.getInitialState() });
	newTransitionVector.push_back({ automate.getFinState(), newAutomatLambdaTransiti.getLambda(), q_state2 });
	newTransitionVector.push_back({ q_state, newAutomatLambdaTransiti.getLambda(), q_state2 });
	newTransitionVector.push_back({ automate.getFinState(), newAutomatLambdaTransiti.getLambda(), automate.getInitialState() });
	newAutomatLambdaTransiti.setTransition(newTransitionVector);

	newAutomatLambdaTransiti.setEntryAlphabet(automate.getEntryAlphabet());	//entry alphabet is automate entry alphabet

	return newAutomatLambdaTransiti;
}

AutomatLambdaTranzitii OperatorOr(const AutomatLambdaTranzitii& firstAutomate, const AutomatLambdaTranzitii& secondAutomate, uint16_t& cntStari, const std::string& q_statePattern)
{
	AutomatLambdaTranzitii newAutomatLambdaTransiti;
	std::string q_state = q_statePattern + std::to_string(cntStari);
	cntStari++;
	std::string q_state2 = q_statePattern + std::to_string(cntStari);
	cntStari++;

	std::string newInitialState = q_state;
	std::string newFinalState = q_state2;
	newAutomatLambdaTransiti.setInitialState(newInitialState); // initial state is q_state
	newAutomatLambdaTransiti.setFinState(newFinalState); // final state is q_state2
	
	std::vector< std::string> newStates; //states are the union of the two automates states and q_state and q_state2
	newStates.reserve(firstAutomate.getStates().size() + secondAutomate.getStates().size() + 2);
	newStates.push_back(q_state);
	for (uint16_t index = 0; index < firstAutomate.getStates().size(); index++)
	{
		newStates.push_back(firstAutomate.getStates()[index]);
	}
	for (uint16_t index = 0; index < secondAutomate.getStates().size(); index++)
	{
		newStates.push_back(secondAutomate.getStates()[index]);
	}
	newStates.push_back(q_state2);
	newAutomatLambdaTransiti.setStates(newStates);
	
	// transitions are every transition of the first automate and the second automate +
	// the lambda transitions from q_state to the initial state of the first automate  , with the lambda transitions +
	// from q_state to the initial state of the second automate  , with the lambda transitions +
	// from the final state of the first automate to q_state2 ,with lambda transitions +
	// from the final state of the second automate to q_state2 ,with lambda transitions
	std::vector<AutomatLambdaTranzitii::Transition> newTransitionVector;
	newTransitionVector = firstAutomate.getTransition();
	for (const auto& transition : secondAutomate.getTransition())
	{
		newTransitionVector.push_back(transition);
	}
	newTransitionVector.push_back({ q_state, newAutomatLambdaTransiti.getLambda(), firstAutomate.getInitialState() });
	newTransitionVector.push_back({ q_state, newAutomatLambdaTransiti.getLambda(), secondAutomate.getInitialState() });
	newTransitionVector.push_back({ firstAutomate.getFinState(), newAutomatLambdaTransiti.getLambda(), q_state2 });
	newTransitionVector.push_back({ secondAutomate.getFinState(), newAutomatLambdaTransiti.getLambda(), q_state2 });
	newAutomatLambdaTransiti.setTransition(newTransitionVector);

	//entry alphabet is the union of the two automates entry alphabet
	std::unordered_set< char> newEntryAlphabet = firstAutomate.getEntryAlphabet();
	for (const auto& entry : secondAutomate.getEntryAlphabet())
	{
		newEntryAlphabet.insert(entry);
	}
	newAutomatLambdaTransiti.setEntryAlphabet(newEntryAlphabet);
	
	return newAutomatLambdaTransiti;
}
