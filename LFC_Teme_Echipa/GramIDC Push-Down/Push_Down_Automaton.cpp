#include "Push_Down_Automaton.h"
#include <queue>
#include <stack>

void Push_Down_Automaton::setEntryAlphabet(const std::vector<char>& alphabet)
{
	this->entry_alphabet = alphabet;
}

void Push_Down_Automaton::setInitialState(const char& state)
{
	this->initial_state = state;
}

void Push_Down_Automaton::setFinState(const std::vector<char>& state)
{
	this->Fin_state = state;
}

void Push_Down_Automaton::setStates(const std::vector<char>& states)
{
	this->m_States = states;
}

void Push_Down_Automaton::setTransition(const std::vector<std::tuple<char, char, char>>& transition)
{
	this->transition = transition;
}

void Push_Down_Automaton::setTransitionResults(const std::vector<std::pair<char, std::string>>& transitionResults)
{
	this->transitionResults = transitionResults;
}

void Push_Down_Automaton::setStackStates(const std::vector<char>& stackStates)
{
	this->m_stackStates = stackStates;
}

void Push_Down_Automaton::setLambda(const char& lambda)
{
	this->m_lambda = lambda;
}

void Push_Down_Automaton::setInitialStackState(const char& initialStackState)
{
	this->initialStackState = initialStackState;
}

bool Push_Down_Automaton::checkWord(std::string word)
{
	char state = initial_state;

	if (word.empty())
	{
		for (const auto& [St_state, alph_ch, stack_state] : transition)
		{
			if (St_state == initial_state && alph_ch == m_lambda && stack_state == initialStackState)
			{
				std::cout << "Valid\n";
				return true;
			}
		}
		std::cout << "Invalid\n";
		return false;
	}

	std::stack<std::tuple<char, std::string, std::string>> stack;
	stack.push({ initial_state, word, std::string{initialStackState} });
	std::cout << '\n';

	while (!stack.empty())
	{
		std::tuple<char, std::string, std::string> current = stack.top();
		auto& [curent_state, curent_word, current_stackState] = current;
		std::cout << '(' << curent_state << ',' << curent_word << ',' << current_stackState << ')' << " -> ";
		if (curent_word.empty() && current_stackState.empty())
		{
			std::cout << "Valid";
			return true;
		}

		stack.pop();
		int index = 0;

		for (auto& st : transition)
		{
			auto& [St_state, alph_ch, stack_state] = st;
			if (St_state == curent_state && alph_ch == curent_word[0] && stack_state == current_stackState[0])
			{
				std::string next_stackState;
				if (std::get<1>(transitionResults[index])[0] == m_lambda)
				{
					next_stackState = current_stackState.substr(1);
				}
				else
				{
					std::string aux = current_stackState;
					next_stackState = aux.replace(aux.begin(), aux.begin() + 1, std::get<1>(transitionResults[index]));
				}

				stack.push({ std::get<0>(transitionResults[index]), curent_word.substr(1), next_stackState });
			}
			index++;
		}
	}
	std::cout << "Invalid";
	return false;
}

bool Push_Down_Automaton::isDeterministic()
{
	std::tuple<char, char, char> tuple1;
	std::tuple<char, char, char> tuple2;
	const auto& [first_state_0, alphabet_0, stack_state_0] = tuple1;
	const auto& [first_state_1, alphabet_1, stack_state_1] = tuple2;
	bool existcharinitial = false;
	bool existlambdainitial = false;

	for (int i = 0; i < transition.size(); i++)
	{
		tuple1 = transition[i];
		if (first_state_0 == initial_state && stack_state_0 == initialStackState)
		{
			existcharinitial = true;
			if (alphabet_0 == m_lambda)
			{
				existlambdainitial = true;
			}
			if (existcharinitial && existlambdainitial)
			{
				return false;
			}
		}

		for (int j = i + 1; j < transition.size(); j++)
		{
			tuple2 = transition[j];
			if (first_state_0 == first_state_1 && alphabet_0 == alphabet_1 && stack_state_0 != stack_state_1)
			{
				return false;
			}
		}
	}
	return true;
}

std::ostream& operator<<(std::ostream& out, Push_Down_Automaton& automat)
{
	out << "\nFinite Automat :\n" << "M = ({";
	for (int i = 0; i < automat.m_States.size(); i++)
	{
		out << automat.m_States[i];
		if (i < automat.m_States.size() - 1)
		{
			out << ", ";
		}
	}

	out << "} ,{";
	for (int i = 0; i < automat.entry_alphabet.size(); i++)
	{
		out << automat.entry_alphabet[i];
		if (i < automat.entry_alphabet.size() - 1)
		{
			out << ", ";
		}
	}

	out << "} ,{";
	for (int i = 0; i < automat.m_stackStates.size(); i++)
	{
		out << automat.m_stackStates[i];
		if (i < automat.entry_alphabet.size() - 1)
		{
			out << ", ";
		}
	}

	out << "}, D, " << automat.initial_state << ", " << automat.initialStackState;

	out << "}) cu D :";
	for (int i = 0; i < automat.transition.size(); i++)
	{
		auto& [St_state, alph_ch, stack_state] = automat.transition[i];
		out << "\n D(" << St_state << " ," << alph_ch << " ," << stack_state << ')';

		auto& [state, stack_Fstate] = automat.transitionResults[i];
		out << " -> (" << state << " ," << stack_Fstate << ')';
	}
	return out;
}
