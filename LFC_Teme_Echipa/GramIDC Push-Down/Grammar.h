#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <random>
#include <unordered_map>

class Grammar
{
private:
	std::vector<char> VNonterminal;
	std::vector<char> VTerminal;
	char Start;
	char m_lambda;
	std::vector<std::pair<std::string, std::string>> Productions;

	void findAndDestroyProductions(const std::vector<char>& NewVNonterminal);
	void lema1(int indexProd, int indexNonT);
	void lema2(std::vector<int>indexProd, int indexProdToTerminal);
	bool isTerminal(char symbol);

public:
	Grammar();
	bool checkGrammar();
	std::string generateWord();
	friend std::istream& operator >> (std::istream& in, Grammar& gram);
	friend std::ostream& operator <<(std::ostream& out, const  Grammar& gram);
	bool isRegular();
	bool isIDC();
	void simplifyGrammar();

	void transformGrammarToFNG();
	~Grammar();

	void setStart(char start);
	char getStart() const;
	void setLambda(char lambda);
	char getLambda() const;
	void setVNonterminal(std::vector<char> vNonterminal);
	std::vector<char> getVNonterminal() const;
	void setVTerminal(std::vector<char> vTerminal);
	std::vector<char> getVTerminal() const;
	void setProductions(std::vector<std::pair<std::string, std::string>> productions);
	std::vector<std::pair<std::string, std::string>> getProductions() const;
};
