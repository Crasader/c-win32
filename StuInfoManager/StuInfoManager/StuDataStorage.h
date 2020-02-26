#ifndef _STUDATASTORAGE_H_
#define _STUDATASTORAGE_H_
#include <string>
#include <vector>
#include <algorithm>
#define testsize 10000
namespace Dio
{
	typedef size_t No;
	typedef bool Sex;
	struct StuInfo
	{
		std::string _stu_name;
		No _stu_no;
		size_t _class_num;
		Sex _sex;
		StuInfo():_stu_name(),_stu_no(0),_class_num(0),_sex(true) { }
	};

	enum Infotype
	{
		_NameInfo,
		_NoInfo,
		_ClassInfo,
		_SexInfo,
		_UnknownInfo
	};

	class StuDataStorage
	{
	public:
		StuDataStorage()
		{
			initdata();
		}
		~StuDataStorage()
		{
			memset(&_data_base, 0, sizeof(StuInfo)*testsize);
			_data.clear();
		}
		void initdata();

	public:
		std::vector<StuInfo> _data;
	private:
		
		StuInfo _data_base[testsize];
	};
}


#endif//!_STUDATASTORAGE_H_