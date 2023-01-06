#include "Grammar.h"
#include <unordered_set>

Grammar::Grammar()
{
	this->Start = 'S';
}

bool Grammar::checkGrammar()
{
	bool is_S = false;
	for (int i = 0; i < VNonterminal.size(); i++)
	{
		for (int j = 0; j < VTerminal.size(); j++)
		{
			if (VNonterminal[i] == VTerminal[j])//validate if VNonterminal  intersects VTerminal = NULL
				return false;
		}
		if (VNonterminal[i] == Start)
		{
			is_S = true;
		}
	}
	if (!is_S)// check if S is part of VNonterminal
	{
		return false;
	}

	bool p_right_is_Start = false;
	for (const auto& [P_left_member, P_right_member] : Productions)
	{
		bool p_right_containst_neterminal = false;

		if (P_left_member.size() == 1 && P_left_member[0] == Start)// there must be a START production
		{
			p_right_is_Start = true;
		}
		int i = 0;
		while (i < VNonterminal.size())
		{
			if (P_left_member.find(VNonterminal[i]) != std::string::npos)// p_right must contain a VNonterminal
			{
				p_right_containst_neterminal = true;
			}
			i++;
		}
		if (p_right_containst_neterminal == false)
		{
			return false;
		}
		for (const auto& ch_right : P_left_member)
		{
			bool check_ch = false;
			int index = 0;
			while (index < VTerminal.size() && index < VNonterminal.size())
			{
				if (VTerminal[index] == ch_right || VNonterminal[index] == ch_right)
				{
					check_ch = true;
					break;
				}
				index++;
			}
			while (check_ch == false && index < VNonterminal.size())
			{
				if (VNonterminal[index] == ch_right)
				{
					check_ch = true;
					break;
				}
				index++;
			}
			while (check_ch == false && index < VTerminal.size())
			{
				if (VTerminal[index] == ch_right)
				{
					check_ch = true;
					break;
				}
				index++;
			}
			if (check_ch == false) // Prod contains unknown elements
			{
				return false;
			}
		}
		for (const auto& ch_left : P_right_member)
		{
			bool check_ch = false;
			int index = 0;
			while (index < VTerminal.size() && index < VNonterminal.size())
			{
				if (VTerminal[index] == ch_left || VNonterminal[index] == ch_left)
				{
					check_ch = true;
					break;
				}
				index++;
			}
			while (check_ch == false && index < VNonterminal.size())
			{
				if (VNonterminal[index] == ch_left)
				{
					check_ch = true;
					break;
				}
				index++;
			}
			while (check_ch == false && index < VTerminal.size())
			{
				if (VTerminal[index] == ch_left)
				{
					check_ch = true;
					break;
				}
				index++;
			}
			if (check_ch == false) // Prod contains unknown elements
			{
				return false;
			}
		}
	}
	if (p_right_is_Start == false)
	{
		return false;
	}
	return true;
}


std::string Grammar::generateWord()
{
	if (checkGrammar() == true)
	{
		std::random_device rd;
		std::mt19937 eng(rd());
		std::uniform_int_distribution<> distr_Prod(0, Productions.size() - 1);
		std::pair < std::string, std::string> Prod;
		auto& [P_left_member, P_right_member] = Prod;
		std::string word;
		word += Start;
		std::cout << "\nStart :" << word;
		bool check_valid = true;
		while (check_valid)
		{
			check_valid = false;
			for (auto& a : Productions)
			{
				if (word.find(a.first) != std::string::npos)
				{
					check_valid = true;
					break;
				}
			}
			if (check_valid == true)
			{
				std::unordered_set<int> indices;

				Prod = Productions[distr_Prod(eng)];

				int index = 0;
				while ((index = word.find(P_left_member, index)) != std::string::npos) {
					indices.insert(index);
					index += P_left_member.length();
				}

				if (indices.size())
				{
					std::uniform_int_distribution<> distr_Indices(0, indices.size() - 1);
					auto it = indices.begin();
					std::advance(it, distr_Indices(eng));

					if (P_right_member.find(m_lambda) != std::string::npos)
					{
						std::string aux = P_right_member;
						const auto& pos = aux.find(m_lambda);
						aux.erase(pos, pos + 1);
						word.replace(word.begin() + *it, word.begin() + *it + P_left_member.size(), aux);
					}
					else {
						word.replace(word.begin() + *it, word.begin() + *it + P_left_member.size(), P_right_member);
					}
					std::cout << " --> " << word;
				}
			}
		}
		return word;
	}
	return "";
}

bool Grammar::isRegular()
{
	if (checkGrammar() == false)
	{
		return false;
	}

	bool is_regular = true;

	for (const auto& [P_left_member, P_right_member] : Productions)
	{
		bool check_left_member = false;
		if (P_left_member.size() != 1 || P_right_member.size() > 2)
		{
			return false;
		}

		if (P_left_member[0] != this->Start && P_right_member[0] == this->m_lambda)
		{
			return false;
		}

		for (const auto& vn : VNonterminal)
		{
			if (vn == P_left_member[0])
			{
				check_left_member = true;
				break;
			}
		}
		if (check_left_member == false)
			return false;


		bool check_ch = false;
		for (const auto& vt : VTerminal)
		{
			if (vt == P_right_member[0])
			{
				check_ch = true;
				break;
			}
		}
		if (check_ch == false)// Form is not aA / a
		{
			return false;
		}

		if (P_right_member.size() == 2) // Form must be aA
		{
			if (P_right_member[0] == this->m_lambda)
			{
				return false;
			}
			check_ch = false;
			for (const auto& vn : VNonterminal)
			{
				if (vn == P_right_member[1])
				{
					check_ch = true;
					break;
				}
			}
			if (check_ch == false)
			{
				return false;
			}
		}
	}
	return true;
}

bool Grammar::isIDC()
{
	if (checkGrammar() == false)
	{
		return false;
	}

	for (const auto& [P_left_member, P_right_member] : Productions)
	{
		bool check_left_member = false;
		if (P_left_member.size() != 1)
		{
			return false;
		}
		if (P_left_member[0] == this->Start && P_right_member[0] == this->m_lambda && P_right_member.size() == 1)
		{
			continue;
		}
		// tratare caz S -> Lmbda
		if (P_left_member[0] != this->Start && P_right_member[0] == this->m_lambda)
		{
			return false;
		}

		for (const auto& vn : VNonterminal)
		{
			if (vn == P_left_member[0])
			{
				check_left_member = true;
				break;
			}
		}
		if (check_left_member == false)
			return false;

		bool check_ch = false;
		for (const auto& ch : P_right_member)
		{
			// LAMBDA NU E TERMINAL/NETERMINAL 
			if (ch == m_lambda)
			{
				return false;
			}
			for (const auto& vn : VNonterminal)
			{
				if (vn == ch)
				{
					check_ch = true;
					break;
				}
			}
			if (check_ch == false)
			{
				for (const auto& vt : VTerminal)
				{
					if (vt == ch)
					{
						check_ch = true;
						break;
					}
				}
			}
			if (check_ch == false)
			{
				return false;
			}
		}
	}
	return true;
}

void Grammar::findAndDestroyProductions(const std::vector<char> &NewVNonterminal)
{
	bool ok;
	std::vector<char> excludedNonterminals;
	for (const auto& nonT : VNonterminal)
	{
		ok = true;
		for (const auto& newNonT : NewVNonterminal)
		{
			if (nonT == newNonT)
			{
				ok = false;
				break;
			}

		}

		if (ok)
		{
			excludedNonterminals.push_back(nonT);
		}
	}


	std::vector<std::pair<std::string, std::string>> NewVProductions;
	std::ranges::for_each(Productions, [&excludedNonterminals, &NewVProductions, &ok](const auto& prod) {
		const auto& [left, right] = prod;
		ok = true;
		for (const auto& vNonTBf : excludedNonterminals)
		{
			if (right.find(vNonTBf) != right.npos || left.find(vNonTBf) != left.npos)
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			NewVProductions.push_back(prod);
		}
		});

	Productions = NewVProductions;
	VNonterminal = NewVNonterminal;
}

void Grammar::simplifyGrammar()
{
	if (!isIDC())
	{
		throw std::exception{ "Grammar is not IDC" };
	}

	//Eliminarea simbolurilor care nu genereaza cuvinte
	std::vector<char> NewVNonterminal, VNonterminalBefore;
	std::unordered_set<int> indiciProd;
	std::vector<std::pair<std::string, std::string>> NewProductions;

	bool ok;
	int index;
	do
	{
		VNonterminalBefore = NewVNonterminal;
		index = 0;
		for (const auto& [left, right] : Productions)
		{
			if (indiciProd.size() && indiciProd.find(index) != indiciProd.end())
			{
				index++;
				continue;
			}

			for (const auto& nonTerminal : NewVNonterminal)
			{
				if (left[0] == nonTerminal)
				{
					goto endLoop;
				}
			}

			for (const auto& ch : right)
			{
				ok = false;
				for (const auto& terminal : VTerminal)
				{
					if (ch == terminal)
					{
						ok = true;
						break;
					}
				}

				if (ok)
				{
					continue;
				}

				for (const auto& nonterminal : VNonterminalBefore)
				{
					if (ch == nonterminal)
					{
						ok = true;
						break;
					}
				}

				if (!ok)
				{
					break;
				}
			}
			if (ok)
			{
				NewVNonterminal.push_back(left[0]);
			endLoop:
				NewProductions.push_back({ left, right });
				indiciProd.insert(index);
			}
			index++;
		}

	} while (NewVNonterminal != VNonterminalBefore);

	Productions = NewProductions;
	findAndDestroyProductions(NewVNonterminal);

	//eliminarea simbolurilor inaccesibile
	std::unordered_set<int> NonTerminals;
	NonTerminals.insert(Start);

	for (const auto& NewVNonT : NonTerminals)
	{
		for (const auto& [left, right] : Productions)
		{
			if (left[0] == NewVNonT)
			{
				for (const auto& VNonT : VNonterminal)
				{
					if (right.find(VNonT) != right.npos)
					{
						NonTerminals.insert(VNonT);
					}
				}
			}
		}
	}

	NewVNonterminal.clear();
	std::ranges::copy(NonTerminals ,std::back_inserter(NewVNonterminal));
	findAndDestroyProductions(NewVNonterminal);

	//eliminarea redenumirilor
	NewProductions.clear();
	NewVNonterminal.clear();
	std::string newRight;
	bool rightIsNonTerminal, leftIsNonTerminal;

	for (const auto& [left, right] : Productions)
	{
		if (left.size() == 1 && right.size() == 1)
		{
			rightIsNonTerminal = 0;
			leftIsNonTerminal = 0;
			for (const auto& VNonT : VNonterminal)
			{
				if (!rightIsNonTerminal && right.find(VNonT) != right.npos)
				{
					rightIsNonTerminal = 1;
				}
				else if (!leftIsNonTerminal && left.find(VNonT) != left.npos)
				{
					leftIsNonTerminal = 1;
				}

				if (leftIsNonTerminal && rightIsNonTerminal)
				{
					break;
				}
			}

			if (leftIsNonTerminal && rightIsNonTerminal)
			{
				NewVNonterminal.push_back(right[0]);
				NewProductions.clear();
				for (const auto& prod : Productions)
				{
					newRight.clear();
					if (prod.first == right)
					{
						if (prod.second.find(right) != prod.second.npos)
						{
							if (prod.first.size() == 1 && prod.first.size() == 1)
							{
								continue;
							}
							for (int i = 0; i < prod.second.size(); i++)
							{
								if(prod.second[i] != right[0])
								{
									newRight += prod.second[i];
								}
								else
								{
									newRight += left[0];
								}
							}
							NewProductions.push_back({ left, newRight });
						}
						else
						{
							NewProductions.push_back({ left, prod.second });
						}
					}
					else if (prod.second.find(right) == prod.second.npos)
					{
						for (int i = 0; i < prod.second.size(); i++)
						{
							if (prod.second[i] != right[0])
							{
								newRight += prod.second[i];
							}
							else
							{
								newRight += left[0];
							}
						}
						NewProductions.push_back({ prod.first, newRight });
					}
				}

				Productions = NewProductions;
			}	
		}
	}

	VNonterminalBefore.clear();
	std::ranges::for_each(VNonterminal, [&NewVNonterminal, &VNonterminalBefore, &ok](const auto& vNonT) {
		ok = true;
		for (const auto& excludedNonT : NewVNonterminal)
		{
			if (excludedNonT == vNonT)
			{
				ok = false;
				break;
			}
		}

		if (ok)
		{
			VNonterminalBefore.push_back(vNonT);
		}

		});

	VNonterminal = VNonterminalBefore;
}

void Grammar::lema1(int indexProd, int indexNonT)
{
	std::string left = Productions[indexProd].first;
	std::string right = Productions[indexProd].second;
	std::vector<std::pair<std::string, std::string>> newProductions;
	bool modified = false;

	for (const auto& [prodLeft, prodRight] : Productions)
	{
		if (prodLeft[0] == right[indexNonT])
		{
			if (prodRight[0] == m_lambda)
			{
				continue;
			}
			if (modified)
			{
				std::string aux = right;
				const auto beginIt = aux.begin() + indexNonT;
				aux.replace(beginIt, beginIt + 1, prodRight);
				newProductions.push_back({ left, aux});
			}
			else
			{
				const auto beginIt = Productions[indexProd].second.begin() + indexNonT;
				Productions[indexProd].second.replace(beginIt, beginIt + 1, prodRight);
				modified = true;
			}
		}
	}

	for (auto& prod : newProductions)
	{
		Productions.emplace_back(prod);
	}

}

void Grammar::lema2(std::vector<int> indexProd, int indexProdToTerminal)
{
	std::string newNonT;
	if (VNonterminal[VNonterminal.size() - 1] < 'K')
	{
		newNonT = "Z";
	}
	else
	{
		newNonT = VNonterminal[VNonterminal.size() - 1] - 1;
	}
	VNonterminal.push_back(newNonT[0]);

	for (const int index : indexProd)
	{
		auto& [left, right] = Productions[index];
		left = newNonT;
		right.replace(right.begin(), right.begin() + 1, "");

		Productions.push_back({ newNonT, right + newNonT});
	}
	Productions.push_back({ Productions[indexProdToTerminal].first, Productions[indexProdToTerminal].second + newNonT });
}

bool Grammar::isTerminal(char symbol)
{
	if (symbol == m_lambda)
	{
		return true;
	}

	for (const auto& terminal : VTerminal)
	{
		if (symbol == terminal)
		{
			return true;
		}
	}

	return false;
}

void Grammar::transformGrammarToFNG()
{
	//aplicare lema2

	std::unordered_map<std::string, int> prodToTerminal;
	std::unordered_map<char, char> prodTerminalToNonT;

	for (int i = 0; i < Productions.size(); i++)
	{
		if (Productions[i].second.size() == 1 && isTerminal(Productions[i].second[0]))
		{
			prodToTerminal.insert({ Productions[i].first, i });
			prodTerminalToNonT.insert({ Productions[i].second[0] , Productions[i].first[0] });
		}

		if (Productions[i].second[0] == Productions[i].first[0])
		{
			int indexProdToTerminal;
			bool foundTerminal = false;
			if (prodToTerminal.find(Productions[i].first) != prodToTerminal.end())
			{
				foundTerminal = true;
			}

			std::vector<int> indexProd;
			indexProd.push_back(i);
			for (int j = i + 1; j < Productions.size(); j++)
			{
				if (!foundTerminal && Productions[j].second.size() == 1 && isTerminal(Productions[j].second[0]))
				{
					prodToTerminal.insert({ Productions[j].first, j });
					prodTerminalToNonT.insert({ Productions[j].second[0] , Productions[j].first[0] });
					foundTerminal = true;
				}

				if (Productions[j].first == Productions[i].first &&
					Productions[j].second[0] == Productions[j].first[0])
				{
					indexProd.push_back(j);
				}
			}

			indexProdToTerminal = prodToTerminal[Productions[i].first];
			lema2(indexProd, indexProdToTerminal);
		}
	}

	//aplicare lema1
	for (int i = 0; i < Productions.size(); i++)
	{
		auto& [left, right] = Productions[i];

		for (int j = 1; j < right.size(); j++)
		{
			if (isTerminal(right[j]))
			{
				right[j] = prodTerminalToNonT[right[j]];
			}
		}
		
		if (!isTerminal(right[0]))
		{
			lema1(i, 0);
			i--;
		}
	}

}

Grammar::~Grammar()
{
}

void Grammar::setStart(char start)
{
	this->Start = start;
}

char Grammar::getStart() const
{
	return Start;
}

void Grammar::setLambda(char lambda)
{
	this->m_lambda = lambda;
}

char Grammar::getLambda() const
{
	return m_lambda;
}

void Grammar::setVNonterminal(std::vector<char> vNonterminal)
{
	this->VNonterminal = vNonterminal;
}

std::vector<char> Grammar::getVNonterminal() const
{
	return this->VNonterminal;
}

void Grammar::setVTerminal(std::vector<char> vTerminal)
{
	this->VTerminal = vTerminal;
}

std::vector<char> Grammar::getVTerminal() const
{
	return VTerminal;
}

void Grammar::setProductions(std::vector<std::pair<std::string, std::string>> productions)
{
	this->Productions = productions;
}

std::vector<std::pair<std::string, std::string>> Grammar::getProductions() const
{
	return Productions;
}

std::istream& operator>>(std::istream& in, Grammar& gram)
{
	std::string line;
	std::getline(in, line);
	gram.m_lambda = line[0];
	std::getline(in, line);
	//read every VNonterminal
	//line += ' ';
	while (!line.empty())
	{
		if (std::isalnum(line[0]))
		{
			gram.VNonterminal.emplace_back(line[0]);
		}
		line.erase(0, 1);
	}

	std::getline(in, line);
	//read every VTerminal
	//line += ' ';
	while (!line.empty())
	{
		if (std::isalnum(line[0]))
		{
			gram.VTerminal.emplace_back(line[0]);
		}
		line.erase(0, 1);
	}
	gram.VTerminal.emplace_back(gram.m_lambda);

	while (!in.eof())
	{
		std::pair < std::string, std::string> Prod;
		auto& [P_left_member, P_right_member] = Prod;
		in >> P_left_member >> P_right_member;
		gram.Productions.emplace_back(Prod);
	}
	return in;
}

std::ostream& operator<<(std::ostream& out, const Grammar& gram)
{
	out << "\nGramatica :\nLambda = " << gram.m_lambda << "\nG = ({";
	for (int i = 0; i < gram.VNonterminal.size(); i++)
	{
		out << gram.VNonterminal[i];
		if (i < gram.VNonterminal.size() - 1)
		{
			out << ", ";
		}
	}
	out << "}, {";
	for (int i = 0; i < gram.VTerminal.size() - 1; i++)
	{
		out << gram.VTerminal[i];
		if (i < gram.VTerminal.size() - 2)
		{
			out << ", ";
		}
	}
	out << "}, " << gram.Start << ", P), P continand urmatoarele productiile: \n";
	for (int i = 0; i < gram.Productions.size(); i++)
	{
		const auto& [P_left_member, P_right_member] = gram.Productions[i];
		out << '(' << i << ") " << P_left_member << " -> " << P_right_member << std::endl;
	}
	return out;
}
