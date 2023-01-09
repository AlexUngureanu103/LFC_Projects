#include "Grammar.h"
#include <iostream>
#include <unordered_set>
#include <string>
#include <fstream>
#include "IDC_To_PushDown.h"

// Tema cu PUSH DOWN 
void Menu()
{
	Grammar g;
	std::ifstream inp{ "IDC_Input.txt" };
	if (inp.is_open())
	{
		inp >> g;
	}
	else {
		throw std::invalid_argument("File not found");
	}
	inp.close();
	std::cout << "Check grammar : " << g.checkGrammar() << "\nRegular grammar : " << g.isIDC();
	if (/*g.isRegular() == */true)
	{
		std::cout << g;
		std::unordered_set<std::string> words;
		Push_Down_Automaton PD_Automaton;
		bool exit = false;
		bool PD_created = false;
		int cnt = 0;
		while (!exit)
		{
			std::cout << "\nMenu options :\nx) Exit menu\na) Display grammar\nb) Generate N words from grammar\nc) Simplify grammar\nd) Get FNG grammar";
			std::cout << "\ne)Generate and show PD automaton\nf)Check is the input word is accepted by the PD automaton\ng)Get the lsit of the generated words\nh) generarea unui cuvant in G si verificarea daca e acceptat de automat\n";
			char choice;
			std::cout << "\nChoice : ";
			if (std::cin >> choice)
			{
				switch (choice)
				{
				case 'x':
				{
					exit = true;
					break;
				}
				case 'a':
				{
					std::cout << g;
					break;
				}
				case 'b':
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
				case 'c':
				{
					g.simplifyGrammar();
					std::cout << g;
					break;
				}
				case 'd':
				{
					g.transformGrammarToFNG();
					std::cout << g;
					break;
				}
				case 'e':
				{
					if (PD_created == false)
					{
						PD_Automaton = TransformIDCGrammarToPushDownAutomaton(g);
						PD_created = true;
					}
					bool isDeterministic = PD_Automaton.isDeterministic();
					std::cout << "The PD automaton is deterministic: " << isDeterministic << "\n";
					std::cout << PD_Automaton;
					break;
				}
				case 'f':
				{
					if (PD_created == false)
					{
						std::cout << "The automaton is not created yet\n";
					}
					std::string word;
					std::cout << "\nInput word : ";
					std::cin >> word;
					if (PD_Automaton.checkWord(word) == true)
					{
						std::cout << "\nThe word " << word << " is accepted by the PD automaton\n";
					}
					else
					{
						std::cout << "\nThe word " << word << "  is not accepted by the PD automaton\n";
					}
					break;
				}
				case 'g':
				{
					std::cout << "\nGenerated words : " << words.size() << '\n';
					for (const auto& w : words)
					{
						std::cout << w << ' ';
					}
					break;
				}
				case 'h':
				{
					cnt = words.size();
					int generations = 0;
					for (int i = cnt; i < cnt + 1; generations++)
					{
						auto word = g.generateWord();
						std::cout << "\n\n";
						if (words.find(word) == words.end())
						{
							if (PD_Automaton.checkWord(word) == true)
							{
								std::cout << "\nThe word " << word << " is accepted by the PD automaton\n";
							}
							else
							{
								std::cout << "\nThe word " << word << "  is not accepted by the PD automaton\n";
							}
							words.insert(word);
							i++;
						}
						if (generations > 501)
						{
							std::cout << "\nNo new word was  generated in 500+ tries\n";
							break;
						}
					}
					break;
				}

				default:
					break;
				}
			}
			else {
				std::cout << "\nInvalid input\n";
				std::cin.clear();
				std::cin.seekg(std::ios::end);
			}
		}
	}
}

int main()
{
	try
	{
		Menu();

	}
	catch (const std::exception& ex)
	{
		std::cout << std::endl << ex.what() << std::endl;
	}
}
