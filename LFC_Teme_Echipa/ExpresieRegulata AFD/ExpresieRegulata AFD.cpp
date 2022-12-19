#include <iostream>
#include "Finite_Automaton.h"
#include "FormaPoloneza.h"
#include "AFN_lambdaToAFD.h"
// Expresie Regulata ->AFD

void afisareMeniu()
{
	std::cout << "Meniu:\n";
	std::cout << "(a) afisarea automatului M\n";
	std::cout << "(b) afisarea inteligibila a expresiei regulate r din fisier\n";
	std::cout << "(c) verificarea daca un cuvant este sau nu acceptat de automatul\n";
	std::cout << "(x) Exit\n";
	std::cout << "\nIntroduceti optiunea dorita: ";
}

void Meniu()
{
	std::string expresieRegulata;
	std::ifstream inp{ "FormaPoloneza.txt" };
	if (inp.is_open()) 
	{
		std::getline(inp, expresieRegulata);
		inp.close();
	}
	else
	{
		throw std::exception {"Fisierul nu a putut fi deschis"};
	}

	std::vector<char> FP = CitireFormaPoloneza(expresieRegulata);
	AutomatLambdaTranzitii AFNlambda = transformareFormaPolonezaInAutomatLambdaTranzitii(FP);
	Finite_Automaton M = fromAFNLambdaToAFD(AFNlambda);

	char opt;
	int n;
	std::string cuv;
	do {
		afisareMeniu();
		std::cin >> opt;
		switch (opt)
		{
		case 'a':
		{
			std::cout << std::endl << M << std::endl << std::endl;
			break;
		}
		case 'b':
		{
			std::cout << std::endl;

			std::ranges::for_each(expresieRegulata, [](const char ch) {
				if (ch != '.')
				{
					std::cout << ch; 
				}
				});
			
			std::cout << std::endl << std::endl;
			break;
		}
		case 'c':
		{
			std::cout << "\nIntroduceti cuvantul: ";
			std::cin >> cuv;
			M.checkWord(cuv);
			std::cout << std::endl << std::endl;
			break;
		}
		case 'x':
		{
			std::cout << "La revedere!";
			break;
		}
		default:
		{
			std::cout << "Optiunea introdusa NU este valida" << std::endl << std::endl;
		}
		}
	} while (opt != 'x');
}

uint16_t main()
{
	try
	{
		Meniu();
	}
	catch (std::exception& e)
	{
		std::cout << e.what();
	}

	return 0;
}

