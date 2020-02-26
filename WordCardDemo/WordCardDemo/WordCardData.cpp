#include "stdafx.h"
#include <iostream>
#include <locale.h>
#include <fstream>
#include <sstream>

#include "WordCardData.h"


CWordCardData::CWordCardData()
{
	setlocale(LC_ALL, "chs");
}


CWordCardData::~CWordCardData()
{
}

bool CWordCardData::ReadFile()
{
	std::ifstream fin("text1.csv");
	std::string line;
	if (fin.is_open())
		std::cout << "Open Success!";
	else
		std::cout << "Open Fail!";
	while (getline(fin, line))
	{
		//printf("原始字符:%s\n",line.c_str());
		std::istringstream sin(line);
		std::vector<std::string> fields;
		std::string field;
		while (getline(sin, field, ','))
		{
			fields.push_back(field);
		}
		WordInfo wordcard;
		wordcard.word = Trim(fields[0]);
		wordcard.wordmean = Trim(fields[1]);
		m_vecWordInfo.push_back(wordcard);
		//printf("处理后：%s\t%s\n", word.c_str(), wordmean.c_str());
		//std::cout << "处理后" << word << "\t" << wordmean << std::endl;
	}
	fin.close();
	return true;

}

std::string CWordCardData::Trim(std::string & str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
	return str;
}

const std::vector<WordInfo> CWordCardData::GetData()
{
	return m_vecWordInfo;
}
