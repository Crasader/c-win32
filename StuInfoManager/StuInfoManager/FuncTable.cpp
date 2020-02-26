#include "FuncTable.h"
#include <iostream>

void Dio::FuncTable::Datafind(void * info, Infotype _type)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	switch (_type)
	{
	case Dio::_NameInfo:
		findname(*(std::string*)info);
		break;
	case Dio::_NoInfo:
		findno(*(No*)info);
		break;
	case Dio::_ClassInfo:
		findclass(*(size_t*)info);
		break;
	case Dio::_SexInfo:
		findsex(*(Sex*)info);
		break;
	case Dio::_UnknownInfo:
		throw;
		break;
	default:
		break;
	}
}

void Dio::FuncTable::DataErase(void * info, Infotype _type)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	switch (_type)
	{
	case Dio::_NameInfo:
		erasename(*(std::string*)info);
		break;
	case Dio::_NoInfo:
		eraseno(*(No*)info);
		break;
	case Dio::_ClassInfo:
		eraseclass(*(size_t*)info);
		break;
	case Dio::_SexInfo:
		//findsex(*(Sex*)info);
		break;
	case Dio::_UnknownInfo:
		throw;
		break;
	default:
		break;
	}
}

void Dio::FuncTable::DataAdd(StuInfo info)
{
	_storage._data.push_back(info);
}

void Dio::FuncTable::DataModify(No _tag, No _src)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	_it = std::find_if(_it, _storage._data.end() - 1, [&_tag](StuInfo _tmp)->bool {return _tmp._stu_no == _tag; });
	_it->_stu_no = _src;
}


void Dio::FuncTable::findname(std::string & info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	do
	{
		_it = std::find_if(_it, _storage._data.end()-1, [&info](StuInfo _tmp)->bool {return _tmp._stu_name == info; });
		_Info.push_back(*_it);
		if (_it!= _storage._data.end())
			++_it;
	} while (_it != _storage._data.end());
	StuInfo _tmp = *(_Info.end() - 1);
	if (_tmp._stu_name!=info)
	{
		_Info.pop_back();
	}
}

void Dio::FuncTable::findno(No info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	do
	{
		_it = std::find_if(_it, _storage._data.end()-1, [&info](StuInfo _tmp)->bool {return _tmp._stu_no == info; });
		_Info.push_back(*_it);
		if (_it != _storage._data.end())++_it;
	} while (_it != _storage._data.end());
	StuInfo _tmp = *(_Info.end() - 1);
	if (_tmp._stu_no != info)
	{
		_Info.pop_back();
	}
}

void Dio::FuncTable::findclass(size_t info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	do
	{
		_it = std::find_if(_it, _storage._data.end()-1, [&info](StuInfo _tmp)->bool {return _tmp._class_num == info; });
		_Info.push_back(*_it);
		if (_it != _storage._data.end())++_it;
	} while (_it != _storage._data.end());
	StuInfo _tmp = *(_Info.end() - 1);
	if (_tmp._class_num != info)
	{
		_Info.pop_back();
	}
}

void Dio::FuncTable::findsex(Sex info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	do
	{
		_it = std::find_if(_it, _storage._data.end()-1, [&info](StuInfo _tmp)->bool {return _tmp._sex == info; });
		_Info.push_back(*_it);
		if (_it != _storage._data.end())++_it;
	} while (_it != _storage._data.end());
	StuInfo _tmp = *(_Info.end() - 1);
	if (_tmp._sex != info)
	{
		_Info.pop_back();
	}
}

void Dio::FuncTable::modifyname(std::string & info)
{

}

void Dio::FuncTable::modifyno(No info)
{
}

void Dio::FuncTable::modifyclass(size_t info)
{
}

void Dio::FuncTable::Dump()
{
	std::cout << "Ãû×Ö  |" << " " << " °à¼¶  |" << " " << " Ñ§ºÅ  |" << std::endl;
	std::for_each(_Info.begin(), _Info.end(), [](StuInfo _it) {std::cout << _it._stu_name <<"    "<<_it._class_num<<"      "<<_it._stu_no<< std::endl; });
}

void Dio::FuncTable::clear()
{
	_Info.clear();
}

void Dio::FuncTable::erasename(std::string & info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
		_it = std::find_if(_it, _storage._data.end() - 1, [&info](StuInfo _tmp)->bool {return _tmp._stu_name == info; });
		_storage._data.erase(_it);
	
}

void Dio::FuncTable::eraseno(No info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	_it = std::find_if(_it, _storage._data.end() - 1, [&info](StuInfo _tmp)->bool {return _tmp._stu_no == info; });
	_storage._data.erase(_it);
	
}

void Dio::FuncTable::eraseclass(size_t info)
{
	std::vector<StuInfo>::iterator _it = _storage._data.begin();
	
	_it = std::find_if(_it, _storage._data.end() - 1, [&info](StuInfo _tmp)->bool {return _tmp._class_num == info; });
		_storage._data.erase(_it);
	
}



