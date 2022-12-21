#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <random>

class Grammar
{
private:
	std::vector<char> VNonterminal;
	std::vector<char> VTerminal;
	char Start;
	char m_lambda;
	std::vector<std::pair<std::string, std::string>> Productions;
public:
	Grammar();
	bool checkGrammar();
	std::string generateWord();
	friend std::istream& operator >> (std::istream& in, Grammar& gram);
	friend std::ostream& operator <<(std::ostream& out, const  Grammar& gram);
	bool isRegular();
	bool isIDC();
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

Grammar CreateFNG(Grammar& gram);