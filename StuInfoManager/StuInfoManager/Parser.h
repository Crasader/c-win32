
#ifndef _PARSER_H_ 
#define _PARSER_H_
#include "FuncTable.h"
#include "StuDataStorage.h"
#include <string>
namespace Dio
{

	class Parser
	{
	public:
		Parser():_contain(),_type(_UnknownInfo){}
		~Parser() { memset(&_contain, 0, sizeof(StuInfo)); }
		void Parse(const char* input);
		Infotype GetType();
		StuInfo& GetContain();
		void clear();

		static void findmode(Dio::FuncTable &func, Parser& parser, Infotype type);
		static void erasemode(Dio::FuncTable &func, Parser& parser, Infotype type);
		void modifymode(Dio::FuncTable &func);
		void addmode(Dio::FuncTable & func);
	private:
		StuInfo _contain;
		Infotype _type;

	};
}


#endif//!_PARSER_H_ 