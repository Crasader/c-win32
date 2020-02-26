#pragma once
#include <vector>

struct WordInfo
{
	std::string word;
	//wchar_t WORDIPA[50];
	std::string wordmean;

};

struct WordCardBtn
{
	std::string wordmean;
	BOOL IsRight;
};

struct WordCardBtnGroup
{
	std::string word;
	WordCardBtn btn1;
	WordCardBtn btn2;
	WordCardBtn btn3;
	WordCardBtn btn4;
};

class CWordCardData
{
public:
	CWordCardData();
	~CWordCardData();
	bool ReadFile();
	
	std::string Trim(std::string& str);
	const std::vector<WordInfo> GetData();
protected:
	std::vector<WordInfo> m_vecWordInfo;
};

