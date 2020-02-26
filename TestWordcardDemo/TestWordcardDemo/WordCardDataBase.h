#pragma once
#include <vector>

struct WordInfo
{
	std::string word;
	//wchar_t WORDIPA[50];
	std::string wordmean;

};

class CWordCardData
{
public:
	CWordCardData();
	~CWordCardData();
	bool ReadFile();
	
	std::string Trim(std::string& str);
private:
	std::vector<WordInfo> m_vecWordInfo;
};

