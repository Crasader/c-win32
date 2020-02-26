#ifndef _FUNCTABLE_H_
#define _FUNCTABLE_H_
#include "StuDataStorage.h"
#include <string>
namespace Dio
{
	class FuncTable
	{
	public:
		FuncTable():_storage(),_Info(){}
		~FuncTable() { _Info.clear(); _storage.~StuDataStorage(); }

		void Datafind(void* info,Infotype _type);
		void DataErase(void* info, Infotype _type);
		void DataAdd(StuInfo info);
		void DataModify(No _tag, No _src);

		void Dump();
		void clear();
		
	private:
		void erasename(std::string& info);
		void eraseno(No info);
		void eraseclass(size_t info);

		void findname(std::string& info);
		void findno(No info);
		void findclass(size_t info);
		void findsex(Sex info);
		
		void modifyname(std::string& info);
		void modifyno(No info);
		void modifyclass(size_t info);
	private:
		std::vector<StuInfo> _Info;
		StuDataStorage _storage;
	};
}


#endif//!_FUNCTABLE_H_