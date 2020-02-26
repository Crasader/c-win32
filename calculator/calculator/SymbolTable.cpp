#include "SymbolTable.h"
#include <algorithm>




Dio::SymbolTable::SymbolTable()
{
}

unsigned int Dio::SymbolTable::AddSymbol(const std::string & str)
{
	_dict[str] = _curID;
	return _curID;
}

unsigned int Dio::SymbolTable::FindSymbol(const std::string & str) const
{
	unsigned int ret = -1;
	auto it = _dict.find(str);
	if (it != _dict.end())
	{
		ret = it->second;
	}
	return ret;
}

class IsEqualID
{
public:
	bool operator()(const std::pair<const std::string, unsigned int> &it) const
	{
		return it.second == _id;
	}
	IsEqualID(unsigned int id):_id(id){}
private:
	unsigned int _id;
};

std::string Dio::SymbolTable::GetSymbolName(unsigned int id) const
{
	std::string ret;

	auto it=std::find_if(_dict.begin(),_dict.end(),IsEqualID(id));
	if (it!=_dict.end())
	{
		ret = it->first;
	}
}

void Dio::SymbolTable::Clear()
{
	_dict.clear();
}

Dio::SymbolTable::~SymbolTable()
{
}



