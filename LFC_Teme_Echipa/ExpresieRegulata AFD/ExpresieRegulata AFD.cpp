#include "Grammar.h"
#include <iostream>
#include <unordered_set>
#include <fstream>
#include "Finite_Automaton.h"
#include <stack>
#include <cstdlib>
#include "AutomatLambdaTranzitii.h"
// Expresie Regulata ->AFD

void Menu()
{
	Grammar g;
	std::ifstream inp{ "input.txt" };
	if (inp.is_open())
	{
		inp >> g;
	}
	else {
		std::cout << "\nFine not found !\n";
	}
	inp.close();
	std::cout << "Check grammar : " << g.checkGrammar() << "\nRegular grammar : " << g.isRegular();
	if (g.isRegular() == true)
	{
		std::unordered_set<std::string> words;
		Finite_Automaton autom;
		bool exit = false;
		bool AFN_created = false;
		uint16_t cnt = 0;
		while (!exit)
		{
			std::cout << "\nMenu options :\n0. Exit menu\n1.Display grammar\n2.Generate N words from grammar\n3.Generate the finite automate and display it\n4.Check if the given word is accepted by the automate\n5.Generate a word and see if it is accepted";

			uint16_t choice;
			std::cout << "\nChoice : ";
			std::cin >> choice;
			switch (choice)
			{
			case 0:
			{
				exit = true;
				break;
			}
			case 1:
			{
				std::cout << g;
				break;
			}
			case 2:
			{
				uint16_t n;
				std::cout << "\nNumber of new words to be generated : ";
				std::cin >> n;
				cnt = words.size();
				uint16_t generations = 0;
				for (uint16_t i = cnt; i < cnt + n; generations++)
				{
					auto word = g.generateWord();
					if (words.find(word) == words.end())
					{
						words.insert(word);
						i++;
					}
					if (generations > n + 500)
					{
						std::cout << "\nNo new word was  generated in 500+ tries\n";
						break;
					}
				}
				break;
			}
			case 3:
			{
				if (AFN_created == false)
				{
					autom = g.createAFN();
					std::cout << "\nVerify Automate : " << autom.verifyAutomaton();
					std::cout << "\nIs Deterministic : " << autom.isDeterministic();
					AFN_created = true;
				}
				std::cout << autom;
				break;
			}
			case 4:
			{
				if (AFN_created == true)
				{
					std::cout << "\nWord to be checked : ";
					std::string word;
					std::cin >> word;
					if (autom.checkWord(word))
					{
						std::cout << "\nThe word is valid";
					}
					else std::cout << "\nInvalid word";
				}
				else std::cout << "\nThe automate is not defined yet";
				break;
			}
			case 5:
			{
				if (AFN_created == true)
				{
					if (autom.checkWord(g.generateWord()))
					{
						std::cout << "\nThe word is valid";
					}
					else std::cout << "\nInvalid word";
				}
				else std::cout << "\nThe automate is not defined yet";

				break;
			}
			case 6:
			{
				std::cout << "\nGenerated words : " << words.size() << '\n';
				for (const auto& w : words)
				{
					std::cout << w << ' ';
				}
				break;
			}
			default:
				break;
			}
		}
	}
}

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
	std::ifstream inp{ "Forma poloneza.in" };
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
void transformareFormaPolonezaInAutomatLambdaTranzitii(std::vector<char>& formaPoloneza)
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
			AutomatLambdaTranzitii ATop = SA.top();
			SA.pop();
			AutomatLambdaTranzitii BTop = SA.top();
			SA.pop();
			// TO DO

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
	return;
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
		transformareFormaPolonezaInAutomatLambdaTranzitii(FP);
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}
	//Menu();

	return 0;
}

