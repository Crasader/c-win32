#include <iostream>
#include "Node.h"
#include "scanner.h"
#include "Parser.h"

int main()
{
	using namespace Dio;
	Node* tmp=new NumberNode(10);
	Node* tmp2 = new NumberNode(20);
	Node* tmp3 = new DivideNode(tmp2, tmp);
	double tmp4 = tmp3->Calc();
	
	std::string str(" 1.235+ (1.25/1.25 * -123.5)");
	//Scanner sca(str);
	//sca.Test_tmp(str.length());

	do 
	{
		std::cout << ">";
		std::string buf;
		std::getline(std::cin, buf);
		Scanner scanner(buf);
		Parser parser(scanner);
		parser.Parse();
		std::cout << parser.Calculate() << std::endl;
	} while (true);


	double test = 2+4/4;
	//printf("%.3f", test);
	return 0;
}