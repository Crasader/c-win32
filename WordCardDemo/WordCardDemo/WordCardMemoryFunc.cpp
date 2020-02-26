#include "stdafx.h"
#include "WordCardMemoryFunc.h"
#include <fstream>
#include <sstream>
#include <iostream>

CWordCardMemoryFunc::CWordCardMemoryFunc()
{
	InitData();
}


CWordCardMemoryFunc::~CWordCardMemoryFunc()
{
	SaveData();
}

void CWordCardMemoryFunc::InitData()
{
	std::ifstream fin("memoryData.csv");
	std::string line;
	if (fin.is_open())
	{
		while (getline(fin, line))
		{
			std::istringstream sin(line);
			std::vector<std::string> fields;
			std::string wordcard;
			while (getline(sin,wordcard,','))
			{
				fields.push_back(wordcard);
			}
			if (fields.size()>1)
			{
				WordCardShow wordcardshow;
				wordcardshow.index = Trim(fields[0]);
				wordcardshow.memorytime = Trim(fields[1]);
				wordcardshow.memorycount = Trim(fields[2]);
				m_vecWordCardMemory.push_back(wordcardshow);
			}
			else
			{
				worktime = Trim(wordcard);
			}
		}
	}
	else
	{
		time(&worktime);
	}
}

void CWordCardMemoryFunc::SaveData()
{
	std::ofstream os("memoryData.csv", ios::out);
	if (os.is_open())
	{
		os << worktime << "\n";
		for (auto wordcard:m_vecWordCardMemory)
		{
			os << wordcard.index << ","<<wordcard.memorytime<<","<<wordcard.memorycount<<"\n";
		}
	}
	os.close();
}

void CWordCardMemoryFunc::GetTime()
{
	time(&nowtime);
}

void CWordCardMemoryFunc::ClassifyWordCard()
{
	for (auto &wordcard:m_vecWordCardMemory)
	{
		CheckMemoryCount(wordcard);
		GetTime();
		int passtime = nowtime - wordcard.memorytime;
		passtime = passtime - passtime % 60;
		switch (passtime)
		{
		case 300:		//根据遗忘曲线 5分钟一次
		{
			if (wordcard.memorycount==0)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		case 1800://30分钟一次
		{
			if (wordcard.memorycount == 1)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		case 43200:	//12h 一次
		{
			if (wordcard.memorycount == 2)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
			
		}
			
			break;
		case 86400:	//1d 一次
		{
			if (wordcard.memorycount==3)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}	
		}
			
			break;
		case 172800:	//2d 一次
		{
			if  (wordcard.memorycount == 4)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
			
		}
			break;
		case 345600:	//4d 一次
		{
			if (wordcard.memorycount == 5)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		case 604800://7d 一次
		{
			if (wordcard.memorycount == 6)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		case 1296000://15d 一次
		{
			if (wordcard.memorycount == 7)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		case 2592000://1m  一次
		{
			if (wordcard.memorycount == 8)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorytime++;
			}
		}
			break;
		case 7776000://3m 一次
		{
			if (wordcard.memorycount == 9)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		case 15552000:	//6m 一次
		{
			if (wordcard.memorycount == 10)
			{
				m_vecReshowGroup.push_back(wordcard);
				wordcard.memorycount++;
			}
		}
			break;
		default:
			break;
		}
	}
}

void CWordCardMemoryFunc::InsertWordCard(WordCardShow & wordcard)
{
	m_vecWordCardMemory.push_back(wordcard);
}

BOOL CWordCardMemoryFunc::IsShowtime()
{
	BOOL ret = FALSE;
	GetTime();
	int passtime = nowtime-worktime;
	if (passtime<5)
	{
		ret = TRUE;
	}
	else
	{
		if ((passtime-(passtime%60))/60>=5)
		{
			if (((passtime - (passtime % 60)) / 60)%5==0)
			{
				ret = TRUE;
			}
		}
	}	//看是不是5分钟
	return ret;
}

BOOL CWordCardMemoryFunc::CheckMemoryCount(WordCardShow& checkcard)
{
	BOOL ret = FALSE;
	GetTime();
	int passtime = nowtime - checkcard.memorytime;
	passtime = passtime - passtime % 60;
	//300,1800,43200,86400,172800,345600,604800,1296000,2592000,7776000,15552000

	switch (checkcard.memorycount)
	{
	case 0:
	{
		if (passtime>300)
			checkcard.memorytime = checkcard.memorytime + (passtime - 300);
	}
	break;
	case 1:
	{
		if (passtime>1800)
			checkcard.memorytime = checkcard.memorytime + (passtime - 1800);
	}
	break;
	case 2:
	{
		if (passtime>43200)
			checkcard.memorytime = checkcard.memorytime + (passtime - 43200);
	}
		break;
	case 3:
	{
		if (passtime > 86400)
			checkcard.memorytime = checkcard.memorytime + (passtime - 43200);
	}
		break;
	case 4:
	{
		if (passtime > 172800)
			checkcard.memorytime = checkcard.memorytime + (passtime - 172800);
	}
		break;
	case 5:
	{
		if (passtime>345600)
			checkcard.memorytime = checkcard.memorytime + (passtime - 345600);
	}
		break;
	case 6:
	{
		if (passtime > 604800)
			checkcard.memorytime = checkcard.memorytime + (passtime - 604800);
	}
		break;
	case 7:
	{
		if (passtime > 1296000)
			checkcard.memorytime = checkcard.memorytime + (passtime - 1296000);
	}
		break;
	case 8:
	{
		if (passtime > 2592000)
			checkcard.memorytime = checkcard.memorytime + (passtime - 2592000);
	}
		break;
	case 9:
	{
		if (passtime > 7776000)
			checkcard.memorytime = checkcard.memorytime + (passtime - 7776000);
	}
	break;
	case 10:
	{
		if (passtime > 15552000)
			checkcard.memorytime = checkcard.memorytime + (passtime - 15552000);
	}
	break;
	default:
		break;
	}
	return 0;
}


BOOL CWordCardMemoryFunc::ShowNextWordCard(WordCardShow& wordcard)
{
	BOOL ret = FALSE;
	if (!m_vecReshowGroup.empty())
	{
		auto end = m_vecReshowGroup.end() - 1;
		wordcard = *end;
		m_vecReshowGroup.erase(end);
		ret = TRUE;
	}
	return ret;
}

int CWordCardMemoryFunc::WordCardDataSize()
{
	return m_vecWordCardMemory.size();
}

int CWordCardMemoryFunc::Trim(std::string & strbuf)
{
	strbuf.erase(0, strbuf.find_first_not_of(" \t\r\n"));
	strbuf.erase(strbuf.find_last_not_of(" \t\r\n") + 1);
	return atoi(strbuf.c_str());
}
