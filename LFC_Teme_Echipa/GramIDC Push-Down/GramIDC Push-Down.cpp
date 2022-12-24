#include "Grammar.h"
#include <iostream>
#include <unordered_set>
#include <string>
#include <fstream>
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
		g.simplifyGrammar();
		std::unordered_set<std::string> words;
		//Finite_Automaton autom;
		bool exit = false;
		bool AFN_created = false;
		int cnt = 0;
		while (!exit)
		{
			std::cout << "\nMenu options :\nx) Exit menu\na) Display grammar\nb) Generate N words from grammar";

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
