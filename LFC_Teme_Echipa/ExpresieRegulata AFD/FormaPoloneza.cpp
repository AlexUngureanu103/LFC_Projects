#include "FormaPoloneza.h"

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

std::vector<char > CitireFormaPoloneza(std::string expresieRegulata)
{
	std::string expr;
	std::vector<char> formaPoloneza;
	std::stack<char> ordineOperatori; // de redenumit

	expr = expresieRegulata; // salvare de stare initiala
	expresieRegulata.push_back(' ');
	uint16_t cntParantezeDeschise = 0;
	uint16_t cntAlternantaNumarSemne = 0;
	uint16_t cntStar = 0;
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
			if (expresieRegulata[0] == '*')
				cntStar++;
			else
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