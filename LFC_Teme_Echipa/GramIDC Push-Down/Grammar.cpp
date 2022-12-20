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
		if (P_left_member.size() != 1 )
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

Grammar::~Grammar()
{
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
