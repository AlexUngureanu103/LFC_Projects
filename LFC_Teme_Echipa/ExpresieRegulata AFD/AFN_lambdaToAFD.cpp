#include "AFN_lambdaToAFD.h"

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
							if (alphabetCharacter == '#')
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
	std::deque<std::string> unexploredStates;
	findNextStates(initial, transitionsLambdaAFN, '#');
	AFDStates.push_back(stateSymbol);
	stateSymbol++;
	unexploredStates.push_back(initial);

	while (!unexploredStates.empty())
	{
		initial = unexploredStates.front();
		unexploredStates.pop_front();

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
					unexploredStates.push_back(current);

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