#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#include <string>
#include <time.h>
#include <iostream>
#include "StuDataStorage.h"
#include "FuncTable.h"
#include "Parser.h"
#define testsize 50


std::string surname[] = { "��","��","��","��","��","��","֣","�Ϲ�","����","�Ϲ�","��","��","ۣ","��","�θ�","�ذ�","��","����","��","��",
"����","����","����","��ͻ","����","��ľ","����" ,"ľ","��","��","ۨ","��","ö","��","��","�","��",
"��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��" };

std::string sec_name[] = { "����"
,  "ۭӱ"
, "�ľ�"
, "��Ƚ"
, "����"
,  "����"
, "����"
, "���"
, "����"
, "����"
, "��ϼ"
, "��ѩ"
, "����"
, "����"
, "����"
, "ع��"
,  "����"
, "ʮ��""��","��","��","��","˫","��","ݷ","��","��","̷","��","��","��","��","��","��","��","Ƚ","��","۪","Ӻ","ȴ",
"�","ɣ","��","�","ţ","��","ͨ","��","��","��","��","��","��","ũ","��","��","ׯ","��","��","��","��","��","Ľ","��","��","ϰ",
"��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","��","»",
"��","��","ŷ","�","��","��","ε","Խ" };



std::string getrandomname()
{
	std::string _tmp;
	int nPos_1 = rand() % 53;
	int nPos_2 = rand() %100 ;
	_tmp.append(surname[nPos_1]);
	_tmp.append(sec_name[nPos_2]);
	return _tmp;
}


using namespace Dio;

//StuInfo test[500000];
int main()
{

	//�����ĵ��Ĵ���
	//FILE* fp;
	//
	//srand(time(NULL));
	//fp = fopen("STUINFO.txt", "wb+");
	//for (int i = 0; i < 500000; ++i)
	//{

	//	test[i]._class_num = rand() % (200 - 1 + 1);
	//	test[i]._stu_no = rand() % 1000001;
	//	test[i]._sex = rand() % (true - false + 1);
	//	test[i]._stu_name = getrandomname();
	//	fwrite(&test[i], sizeof(test[i]), 1, fp);
	//}
	//fclose(fp);
	//std::cout << "�����ʽ������ѯ�����ݡ�+���ո�+����ѯ�����͡�����ѯ��������Name Class No" << std::endl << std::endl;

	Dio::Parser parser;
	Dio::FuncTable func;
	char input[20];
	do
	{
		std::cout << ">" ;
		std::cin.getline(input, 20);
		parser.Parse(input);
	

		
		if (strstr(input,"Find"))
		{
			Parser::findmode(func, parser, parser.GetType());
		}
		if (strstr(input,"Erase"))
		{
			Parser::erasemode(func, parser, parser.GetType());
		}
		if (strstr(input,"Modify"))
		{
			parser.modifymode(func);
		}
		if (strstr(input, "Add"))
		{
			parser.addmode(func);
		}
		func.Dump();
		parser.clear();
	} while (true);

	
	
	return 0;
}