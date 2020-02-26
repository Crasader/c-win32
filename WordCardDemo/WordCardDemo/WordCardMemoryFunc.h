#pragma once
#include "ThreadManager.h"
#include <vector>
#include "time.h"
struct WordCardShow
{
	int index = 0;
	time_t memorytime = 0;
	int memorycount;
};

class CWordCardMemoryFunc
{
public:
	CWordCardMemoryFunc();
	~CWordCardMemoryFunc();

	void InitData();
	void SaveData();
	void GetTime();
	void ClassifyWordCard();
	void InsertWordCard(WordCardShow& wordcard);
	BOOL IsShowtime();
	BOOL CheckMemoryCount(WordCardShow& checkcard);
	BOOL ShowNextWordCard(WordCardShow& wordcard);
	int WordCardDataSize();
private:
	int Trim(std::string& strbuf);
private:

	std::vector<WordCardShow> m_vecWordCardMemory;

	//std::vector<WordCardShow> m_vecFirstMemory;
	//std::vector<WordCardShow> m_vecSecondMemory;
	//std::vector<WordCardShow> m_vecThirdMemory;
	//std::vector<WordCardShow> m_vecFourthMemory;
	//std::vector<WordCardShow> m_vecFifthMemory;
	//std::vector<WordCardShow> m_vecSixthMemory;
	//std::vector<WordCardShow> m_vecSeventhMemory;
	//std::vector<WordCardShow> m_vecEighthMemory;
	//std::vector<WordCardShow> m_vecNinthMemory;
	//std::vector<WordCardShow> m_vecTenthMemory;
	//std::vector<WordCardShow> m_vecEleventhMemory;
	//std::vector<WordCardShow> m_vecTwelfthMemory;

	std::vector<WordCardShow> m_vecReshowGroup;
private:
	time_t worktime;
	time_t nowtime;
};

