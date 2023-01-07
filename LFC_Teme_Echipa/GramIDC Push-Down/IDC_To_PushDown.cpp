#include "IDC_To_PushDown.h"

Push_Down_Automaton TransformIDCGrammarToPushDownAutomaton(Grammar IDC_Grammar)
{
	if (IDC_Grammar.isIDC() == false)
		throw std::invalid_argument("Grammar is not IDC");
	IDC_Grammar.simplifyGrammar();
	IDC_Grammar.transformGrammarToFNG();
	Push_Down_Automaton PD_Automaton;
	PD_Automaton.setEntryAlphabet(IDC_Grammar.getVTerminal());
	PD_Automaton.setInitialState(IDC_Grammar.getStart());
	std::vector<char> fin_state{ 'F' };
	PD_Automaton.setFinState(fin_state);
	PD_Automaton.setLambda(IDC_Grammar.getLambda());
	PD_Automaton.setStates(fin_state);
	PD_Automaton.setStackStates(IDC_Grammar.getVNonterminal());

	auto transitions = IDC_Grammar.getProductions();

	Push_Down_Automaton::Transition PD_transition;
	auto& [T_state, T_alphabet, T_stackState] = PD_transition;
	Push_Down_Automaton::TransitonResults PD_transitionResult;
	auto& [TR_state, TR_result] = PD_transitionResult;
	std::vector<Push_Down_Automaton::Transition> PD_transitions;
	std::vector<Push_Down_Automaton::TransitonResults> PD_transitionResults;
	for (auto& transition : transitions)
	{
		T_state = fin_state[0];
		T_alphabet = transition.second[0];
		T_stackState = transition.first[0];
		PD_transitions.push_back(PD_transition);
		TR_state = T_state;
		TR_result = std::string(transition.second.substr(1));
		PD_transitionResults.push_back(PD_transitionResult);
	}
	PD_Automaton.setTransition(PD_transitions);
	PD_Automaton.setTransitionResults(PD_transitionResults);
	return PD_Automaton;
}
