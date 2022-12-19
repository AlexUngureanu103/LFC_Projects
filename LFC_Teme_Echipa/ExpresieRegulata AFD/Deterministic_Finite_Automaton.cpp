#include "Deterministic_Finite_Automaton.h"
#include <queue>
#include <stack>

bool Deterministic_Finite_Automaton::verifyAutomaton()
{
	bool is_deterministic = true;
	if (m_State.empty() || entry_alphabet.empty())// check if those are not NULL
	{
		return false;
	}
	bool initial_found = false;
	for (const auto& st : m_State)
	{
		if (initial_state == st)
		{
			initial_found = true;
			break;
		}
	}
	if (initial_found == false)// initial state is not a valid state
	{
		return false;
	}
	for (const auto& fin : Fin_state)
	{
		bool check_fin_st = false;
		for (const auto& st : m_State)
		{
			if (fin == st)
			{
				check_fin_st = true;
				break;
			}
		}
		if (check_fin_st == false)// a final state is not a valid state 
		{
			return false;
		}
	}
	for (const auto& transit : transition)
	{
		const auto& [q_init, alph, state_fin] = transit;
		bool check_q_init = false;
		bool check_q_fin = false;
		bool check_trainsition_element = false;
		for (const auto& st : m_State)
		{
			if (check_q_init == false && q_init == st)
			{
				check_q_init = true;
			}
			if (check_q_fin == false && state_fin == st)
			{
				check_q_fin = true;
			}
			if (check_q_init == true && check_q_fin == true)
			{
				break;
			}
		}
		if (check_q_fin == false || check_q_init == false)// a transition state is not valid 
		{
			return false;
		}
		for (const auto& el : entry_alphabet)
		{
			if (alph == el)
			{
				check_trainsition_element = true;
				break;
			}
		}
		if (check_trainsition_element == false) // transition word is not in entry_alphabet
		{
			return false;
		}
	}
	return true;
}

void Deterministic_Finite_Automaton::setQ(const std::vector<char>& State)
{
	this->m_State = State;
}

void Deterministic_Finite_Automaton::setEntryAlphabet(const std::vector<char>& alphabet)
{
	this->entry_alphabet = alphabet;
}

void Deterministic_Finite_Automaton::setInitialState(const char& state)
{
	this->initial_state = state;
}

void Deterministic_Finite_Automaton::setFinState(const std::vector<char>& state)
{
	this->Fin_state = state;
}

void Deterministic_Finite_Automaton::setTransition(const std::vector<std::tuple<char, char, char>>& transition)
{
	this->transition = transition;
}

bool Deterministic_Finite_Automaton::checkWord(std::string word)
{
	if (!isDeterministic())
	{
		throw std::exception {"The Automaton is not feterministic"};
	}

	char state = initial_state;
	if (word.empty())
	{
		for (const auto& fin_states : Fin_state)
		{
			if (initial_state == fin_states)
			{
				std::cout << "Valid , Initial state is a final state\n";
				return true;
			}
		}
		std::cout << "Invalid\n";
		return false;
	}
	std::stack<std::pair<char, std::string>> que;
	que.push({ initial_state,word });
	std::cout << '\n';
	while (!que.empty() && !word.empty())
	{
		std::pair<char, std::string> current = que.top();
		auto& [curent_state, curent_word] = current;
		std::cout << '(' << curent_state << ',' << curent_word << ')' << " -> ";
		if (curent_word == "")
		{
			for (auto& fin : Fin_state)
			{
				if (curent_state == fin)
				{
					std::cout << "Valid";
					return true;

				}
			}
		}
		que.pop();
		for (auto& st : transition)
		{
			if (std::get<0>(st) == curent_state)
			{
				if (std::get<1>(st) == curent_word[0])
				{
					que.push({ std::get<2>(st) ,curent_word.substr(1) });

				}
			}
		}
	}
	std::cout << "Invalid";
	return false;
}

bool Deterministic_Finite_Automaton::isDeterministic()
{
	if (this->verifyAutomaton())// it can be deterministic only if it's a verified automate
	{
		std::tuple<char, char, char> tuple1;
		std::tuple<char, char, char> tuple2;
		const auto& [first_state_0, alphabet_0, second_state_0] = tuple1;
		const auto& [first_state_1, alphabet_1, second_state_1] = tuple2;

		for (int i = 0; i < transition.size(); i++)
		{
			for (int j = i + 1; j < transition.size(); j++)
			{
				tuple1 = transition[i];
				tuple2 = transition[j];
				if (first_state_0 == first_state_1 && alphabet_0 == alphabet_1 && second_state_0 != second_state_1)
				{
					return false;
				}
			}
		}
		return true;
	}
	return false;
}

std::ostream& operator<<(std::ostream& out, Deterministic_Finite_Automaton& automat)
{
	if (!automat.isDeterministic())
	{
		throw std::exception{ "The Automaton is not deterministic" };
	}

	out << "\nFinite Automat :\n" << "M = ({";
	for (int i = 0; i < automat.m_State.size(); i++)
	{
		out << automat.m_State[i];
		if (i < automat.m_State.size() - 1)
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
	out << "}, D, " << automat.initial_state << ", {";
	for (int i = 0; i < automat.Fin_state.size(); i++)
	{
		out << automat.Fin_state[i];
		if (i < automat.Fin_state.size() - 1)
		{
			out << ", ";
		}
	}
	out << "}) cu D :";
	for (int i = 0; i < automat.transition.size(); i++)
	{
		auto& [St_state, alph_ch, fin_state] = automat.transition[i];
		out << "\n D(" << St_state << " ," << alph_ch << " ," << fin_state << ')';
	}
	return out;
}
