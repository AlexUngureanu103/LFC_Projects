#include "Grammar.h"
#include <iostream>
#include <unordered_set>
#include <fstream>
#include "Finite_Automaton.h"
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
		int cnt = 0;
		while (!exit)
		{
			std::cout << "\nMenu options :\n0. Exit menu\n1.Display grammar\n2.Generate N words from grammar\n3.Generate the finite automate and display it\n4.Check if the given word is accepted by the automate\n5.Generate a word and see if it is accepted";

			int choice;
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
				int n;
				std::cout << "\nNumber of new words to be generated : ";
				std::cin >> n;
				cnt = words.size();
				int generations = 0;
				for (int i = cnt; i < cnt + n; generations++)
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

int main()
{
	Menu();
}

