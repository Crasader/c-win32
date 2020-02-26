#include <iostream>
#include "Parser.h"
#include "Node.h"
#include "scanner.h"

Dio::Parser::Parser(Scanner & scaner):_scanner(scaner)
{
	
}

void Dio::Parser::Parse()
{
	_tree = Expr();
}

double Dio::Parser::Calculate() const
{
	return _tree->Calc();
}

Dio::Node * Dio::Parser::Expr()
{
	Node* node = Term();
	EToken token = _scanner.Token();
	if(token==TOKEN_PLUS||token==TOKEN_MINUS)
	{
		MultiNode *multi = new SumNode(node);
		do 
		{
			_scanner.Accept();
			Node* nextNode = Term();
			multi->AppendChild(nextNode, token == TOKEN_PLUS);
			token = _scanner.Token();

		} while (token==TOKEN_PLUS||token==TOKEN_MINUS);
		node = multi;
	}
	return node;
}
//Expr= term+Expr
//		term-Expr
//		term

//Term= factor*term
//		factor/term
//		factor

//Factor= number
//		  - factor
//		  (Expr)

Dio::Node * Dio::Parser::Term()
{
	Node* node = Factor();
	EToken token = _scanner.Token();

	if (token == TOKEN_MULTIPLY || token == TOKEN_DIVIDE)
	{
		MultiNode *multi = new CalcNode(node);
		do
		{
			_scanner.Accept();
			Node* nextNode = Factor();
			multi->AppendChild(nextNode, token == TOKEN_MULTIPLY);
			token = _scanner.Token();

		} while (token == TOKEN_MULTIPLY || token == TOKEN_DIVIDE);
		node = multi;
	}

	return node;
}

Dio::Node * Dio::Parser::Factor()
{
	Node* node = nullptr;
	EToken token = _scanner.Token();
	if (token==TOKEN_LPARENTEESES)
	{
		_scanner.Accept();
		node = Expr();
		if (_scanner.Token()==TOKEN_RPARENTEESES)
			_scanner.Accept();
		else
			std::cout << "\nÈ±ÉÙÓÒÀ¨»¡" << std::endl; 
	}
	else if (token==TOKEN_NUMBER)
	{
		node = new NumberNode(_scanner.NumBer());
		_scanner.Accept();
	}
	else if(token==TOKEN_MINUS)
	{
		_scanner.Accept();
		node = new MinusNode(Factor());
	}
	return node;
}


