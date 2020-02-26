#ifndef PARSER_H_
#define PARSER_H_


namespace Dio
{
	class Scanner;
	class Node;
	class Parser
	{
	public:
		Parser(Scanner &scaner);
		~Parser(){}
		void Parse();
		double Calculate() const;
	private:
		Node* Expr();
		Node* Term();
		Node* Factor();

		Scanner& _scanner;
		Node* _tree;
	};
}


#endif