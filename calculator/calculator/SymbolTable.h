#ifndef SYMBOL_TABLE_H_
#define SYMBOL_TABLE_H_
#include <map>

namespace Dio
{
	class SymbolTable
	{
	public:
		SymbolTable();
		unsigned int AddSymbol(const std::string &str);
		unsigned int FindSymbol(const std::string &str) const;
		std::string GetSymbolName(unsigned int id) const;
		void Clear();
		~SymbolTable();
	private:
		unsigned int _curID;
		std::map<const std::string, unsigned int> _dict;
	};
}


#endif