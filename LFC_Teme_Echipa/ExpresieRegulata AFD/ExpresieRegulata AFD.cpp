#include "Grammar.h"
#include <iostream>
#include <unordered_set>
#include <fstream>
#include "Finite_Automaton.h"
#include <stack>
#include <cstdlib>
#include "AutomatLambdaTranzitii.h"
// Expresie Regulata ->AFD


uint16_t prec(const char& character)
{
	if (character == '|')
		return 1;
	if (character == '.')
		return 2;
	if (character == '*')
		return 3;
	if (character == '(')
		return 0;
}
bool verifAlphaNum(const char& character)
{
	if ((character >= 'a' && character <= 'z') || (character >= '0' && character <= '9'))
		return true;
	return false;
}
std::vector<char > CitireFormaPoloneza()
{
	std::ifstream inp{ "FormaPoloneza.txt" };
	std::string expr;
	std::vector<char> formaPoloneza;

	if (inp.is_open())
	{
		std::stack<char> ordineOperatori; // de redenumit
		std::string expresieRegulata;
		std::getline(inp, expresieRegulata);
		expr = expresieRegulata; // salvare de stare initiala
		expresieRegulata.push_back(' ');
		uint16_t cntParantezeDeschise = 0;
		uint16_t cntAlternantaNumarSemne = 0;
		while (!expresieRegulata.empty())
		{
			if (expresieRegulata[0] == ' ' || expresieRegulata[0] == '	')
			{
				expresieRegulata.erase(0, 1);
				continue;
			}
			else if (verifAlphaNum(expresieRegulata[0]))
			{
				cntAlternantaNumarSemne++;
				formaPoloneza.push_back(expresieRegulata[0]);
				expresieRegulata.erase(0, 1);
				continue;
			}
			else if (expresieRegulata[0] == '(')
			{
				ordineOperatori.push(expresieRegulata[0]);
				cntParantezeDeschise++;
				cntAlternantaNumarSemne++;
				expresieRegulata.erase(0, 1);
				continue;
			}
			else if (expresieRegulata[0] == ')')
			{
				cntParantezeDeschise--;
				if (cntParantezeDeschise < 0)
				{
					formaPoloneza.clear();
					throw std::exception("Expresia Regulata este invalida!  , prea multe paranteze )\n");
					return formaPoloneza;
				}
				while (!ordineOperatori.empty() && ordineOperatori.top() != '(')
				{
					formaPoloneza.push_back(ordineOperatori.top());
					ordineOperatori.pop();
				}
				ordineOperatori.pop();
				expresieRegulata.erase(0, 1);
				continue;
			}
			else if (expresieRegulata[0] == '.' || expresieRegulata[0] == '|' || expresieRegulata[0] == '*')
			{
				cntAlternantaNumarSemne--;
				while (!ordineOperatori.empty() && prec(expresieRegulata[0]) <= prec(ordineOperatori.top()))
				{
					formaPoloneza.push_back(ordineOperatori.top());
					ordineOperatori.pop();
				}
				ordineOperatori.push(expresieRegulata[0]);
				expresieRegulata.erase(0, 1);
				continue;
			}
			else
			{
				formaPoloneza.clear();
				throw std::exception("Expresia Regulata este invalida!  , caracter invalid\n");
				return formaPoloneza;
			}
		}
		if (cntParantezeDeschise != 0)
		{
			formaPoloneza.clear();
			throw std::exception("Expresia Regulata este invalida!  , prea multe paranteze (\n");
			return formaPoloneza;
		}
		if (cntAlternantaNumarSemne != 1)
		{
			formaPoloneza.clear();
			throw std::exception("Expresia Regulata este invalida!  , numar invalid de semne\n");
			return formaPoloneza;
		}
		while (!ordineOperatori.empty())
		{
			formaPoloneza.push_back(ordineOperatori.top());
			ordineOperatori.pop();
		}
	}
	inp.close();
	return formaPoloneza;
}

AutomatLambdaTranzitii transformareFormaPolonezaInAutomatLambdaTranzitii(std::vector<char>& formaPoloneza)
{
	uint16_t cntStari = 0;
	const std::string q_statePattern = "q";
	std::stack<AutomatLambdaTranzitii> SA;
	for (uint16_t index = 0; index < formaPoloneza.size(); index++)
	{
		if (verifAlphaNum(formaPoloneza[index]) == true)
		{
			std::string q_state = q_statePattern + std::to_string(cntStari);
			cntStari++;
			std::string q_state2 = q_statePattern + std::to_string(cntStari);
			cntStari++;
			SA.push(AutomatLambdaTranzitii(q_state, formaPoloneza[index], q_state2));
			continue;
		}
		else if (formaPoloneza[index] == '|')
		{
			AutomatLambdaTranzitii BTop = SA.top();
			SA.pop();
			AutomatLambdaTranzitii ATop = SA.top();
			SA.pop();
			AutomatLambdaTranzitii C = OperatorOr(ATop, BTop, cntStari, q_statePattern);
			SA.push(C);
			continue;
		}
		else if (formaPoloneza[index] == '.')
		{
			AutomatLambdaTranzitii BTop = SA.top();
			SA.pop();
			AutomatLambdaTranzitii ATop = SA.top();
			SA.pop();
			AutomatLambdaTranzitii C = ConcatenateAutomates(ATop, BTop);
			SA.push(C);
			continue;
		}
		else if (formaPoloneza[index] == '*')
		{
			AutomatLambdaTranzitii ATop = SA.top();
			SA.pop();
			AutomatLambdaTranzitii C = InchidereaKleene(ATop, cntStari, q_statePattern);
			SA.push(C);
			continue;
		}
	}
	return SA.top();
}

void findNextStates(std::string& initial, const std::vector<std::tuple<std::string, char, std::string>>& transitionsLambda, const char alphabetCharacter)
{
	std::stack<std::string> states;
	states.push(initial);
	std::string firstState;
	if (alphabetCharacter != '#')
	{
		initial.clear();

	}

	while (!states.empty())
	{
		firstState = states.top();
		states.pop();
		std::ranges::for_each(transitionsLambda, 
			[&states, &alphabetCharacter, &initial, &firstState]
		(const std::tuple<std::string, char, std::string>& transition) {
				auto& [currState, character, nextState] = transition;
				std::string aux;
				for (uint16_t i = 0; i < firstState.size(); i++)
				{
					while (i < firstState.size() && firstState[i] != ',')
					{
						aux.push_back(firstState[i]);
						i++;
					}

					if (currState == aux && character == alphabetCharacter)
					{
						if (initial.find(nextState) == initial.npos)
						{
							if(alphabetCharacter == '#')
							{
								states.push(nextState);
							}
							if (!initial.empty())
							{
								initial += ',';
							}
							initial += nextState;
						}

					}
					aux.clear();
				}
			});
	}
}

Finite_Automaton fromAFNLambdaToAFD(const AutomatLambdaTranzitii& automatLambda)
{
	using Transition = std::tuple<char, char, char>;
	std::vector<Transition> transitionsAFD;
	std::vector< std::string> statesBeforeLambda;
	std::vector< char> AFDStates, finalStates;
	std::string initial, current, aux, aux2;

	const auto& transitionsLambdaAFN = automatLambda.getTransition();
	const auto& states = automatLambda.getStates();
	const auto& alphabetCharacters = automatLambda.getEntryAlphabet();
	const auto& finalStatesAFN = automatLambda.getFinState();

	bool ok;
	uint16_t stateIndex = 0;
	char stateSymbol = 'A';

	initial = automatLambda.getInitialState();
	statesBeforeLambda.push_back(initial);
	std::stack<std::string> unexploredStates;
	findNextStates(initial, transitionsLambdaAFN, '#');
	AFDStates.push_back(stateSymbol);
	stateSymbol++;
	unexploredStates.push(initial);

	while (!unexploredStates.empty())
	{
		initial = unexploredStates.top();
		unexploredStates.pop();

		for (const auto& alphabetChar : alphabetCharacters)
		{
			current = initial;
			findNextStates(current, transitionsLambdaAFN, alphabetChar);
			if (!current.empty())
			{
				ok = true;
				for (uint16_t i = 0; i < statesBeforeLambda.size(); i++)
				{
					if (i != stateIndex && current == statesBeforeLambda[i])
					{
						transitionsAFD.push_back(Transition(AFDStates[stateIndex], alphabetChar, AFDStates[i]));
						ok = false;
						break;
					}
				}
				if (ok)
				{
					statesBeforeLambda.push_back(current);
					findNextStates(current, transitionsLambdaAFN, '#');
					unexploredStates.push(current);

					aux.clear();
					aux2.clear();
					for (uint16_t i = 0; i < current.size(); i++)
					{
						while (i < current.size() && current[i] != ',')
						{
							aux.push_back(current[i]);
							i++;
						}

						for (uint16_t i = 0; i < finalStatesAFN.size(); i++)
						{
							while (!isdigit(finalStatesAFN[i]))
							{
								aux2.push_back(finalStatesAFN[i]);
								i++;
							}
							while (isdigit(finalStatesAFN[i]))
							{
								aux2.push_back(finalStatesAFN[i]);
								i++;
							}
							if (aux == aux2)
							{
								break;
							}
							aux2.clear();
							if (i < finalStatesAFN.size())
							{
								aux2.push_back(finalStatesAFN[i]);
							}
						}
						if (aux == aux2)
						{
							finalStates.push_back(stateSymbol);
							break;
						}
						aux.clear();
					}

					AFDStates.push_back(stateSymbol);
					stateSymbol++;
					transitionsAFD.push_back(Transition(AFDStates[stateIndex], alphabetChar, AFDStates[AFDStates.size() - 1]));
				}
			}
		}
		stateIndex++;
	}

	Finite_Automaton AFD;
	std::vector<char> AFDalphabet;
	std::ranges::for_each(alphabetCharacters, [&AFDalphabet](const char character) {
		AFDalphabet.push_back(character);
		});
	AFD.setEntryAlphabet(AFDalphabet);
	AFD.setFinState(finalStates);
	AFD.setInitialState(AFDStates[0]);
	AFD.setQ(AFDStates);
	AFD.setTransition(transitionsAFD);

	return AFD;
}

uint16_t main()
{
	try
	{
		std::vector<char> FP = CitireFormaPoloneza();
		for (auto& c : FP)
		{
			std::cout << c;
		}
		AutomatLambdaTranzitii AFNlambda = transformareFormaPolonezaInAutomatLambdaTranzitii(FP);
		Finite_Automaton AFD = fromAFNLambdaToAFD(AFNlambda);
		AFD.checkWord("bb");
		AFD.checkWord("aabb");
		AFD.checkWord("bbb");
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}

