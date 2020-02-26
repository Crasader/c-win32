#define _CRT_SECURE_NO_WARNINGS

#include "Parser.h"
#include <iostream>
#define MAXINPUTSIZE 20



void Dio::Parser::Parse(const char* input)
{
	
	do {
		if (strstr(input, "No"))
		{
			_type = _NoInfo;
			_contain._stu_no = atoi(input);
			break;
		}
		if (strstr(input, "Class"))
		{
			_type = _ClassInfo;
			char* tmp = 0;
			_contain._class_num = atoi(input);
			break;
		}
		if (strstr(input, "Name"))
		{
			_type = _NameInfo;
			if (strstr(input,"Find"))
				_contain._stu_name.append(input, 0, strlen(input)-strlen("Find ") - strlen(" Name"));
			if (strstr(input, "Erase"))
				_contain._stu_name.append(input, 0, strlen(input) - strlen("Erase ") - strlen(" Name"));
			break;
		}
		if (strstr(input, "Sex"))
		{
			_type = _NoInfo;
			//memcpy((char*)_contain, input, strlen(input) - strlen(" Sex"));
			break;
		}
		_type = _UnknownInfo;
	} while (_type != _UnknownInfo);
	
}

Dio::Infotype Dio::Parser::GetType()
{
	return _type;
}

Dio::StuInfo& Dio::Parser::GetContain()
{
	return _contain;
}

void Dio::Parser::clear()
{
	memset(&_contain, 0, sizeof(StuInfo));
}

void Dio::Parser::findmode(Dio::FuncTable &func,Parser& parser,Infotype type)
{
	switch	(type)
	{ 
	case Dio::_NameInfo:
		func.Datafind(&parser.GetContain()._stu_name, parser.GetType());
		break;
	case Dio::_NoInfo:
		func.Datafind(&parser.GetContain()._stu_no, parser.GetType());
		break;
	case Dio::_ClassInfo:
		func.Datafind(&parser.GetContain()._class_num, parser.GetType());
		break;
	case Dio::_SexInfo:
		func.Datafind(&parser.GetContain()._sex, parser.GetType());
		break;
	case Dio::_UnknownInfo:
		std::cout << "´íÎóÊäÈë" << std::endl;
		break;
	default:
		break;
	}
}

void Dio::Parser::erasemode(Dio::FuncTable & func, Parser & parser, Infotype type)
{
	switch (type)
	{
	case Dio::_NameInfo:
		func.DataErase(&parser.GetContain()._stu_name, parser.GetType());
		break;
	case Dio::_NoInfo:
		func.DataErase(&parser.GetContain()._stu_no, parser.GetType());
		break;
	case Dio::_ClassInfo:
		func.DataErase(&parser.GetContain()._class_num, parser.GetType());
		break;
	case Dio::_SexInfo:
		//func.DataErase(&parser.GetContain()._sex, parser.GetType());
		break;
	case Dio::_UnknownInfo:
		std::cout << "´íÎóÊäÈë" << std::endl;
		break;
	default:
		break;
	}
}

void Dio::Parser::modifymode(Dio::FuncTable & func)
{
	char input[MAXINPUTSIZE];

	std::cout << ">" << "FindNo:";
	std::cin.getline(input, 20);

	_contain._stu_no = atoi(input);
	memset(input, 0, strlen(input));
	std::cout << ">" << "ModifyNo:";
	std::cin.getline(input, 20);
	No tmp = atoi(input);
	func.DataModify(_contain._stu_no, tmp);
}

void Dio::Parser::addmode(Dio::FuncTable & func)
{
	char input[MAXINPUTSIZE];
	
	std::cout << ">" << "Name:";
	std::cin.getline(input, 20);
	_contain._stu_name.append(input, 0, strlen(input));
	
	std::cout << ">" << "No:";
	memset(input, 0, strlen(input));
	std::cin.getline(input, 20);
	_contain._stu_no=atoi(input);

	std::cout << ">" << "Class:";
	memset(input, 0, strlen(input));
	std::cin.getline(input, 20);
	_contain._class_num = atoi(input);

	func.DataAdd(_contain);
}



