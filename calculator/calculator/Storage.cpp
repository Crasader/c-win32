#include "Storage.h"
#include <assert.h>



Dio::Storage::Storage(SymbolTable & symbol_table)
{
}

bool Dio::Storage::IsInit(const unsigned int id) const
{
	return id<_cells.size()&&_inits.at(id);
}

double Dio::Storage::GetValue(const unsigned int id) const
{
	assert(id <= _cells.size());

	return _cells.at(id);
}

void Dio::Storage::SetValue(const unsigned int id, const double value)
{
	assert(id <= _cells.size());
	if (id<_cells.size())
	{
		_cells.at(id) = value;
	}
	else
	{
		AddValue(id, value);
	}
}

void Dio::Storage::AddValue(const unsigned int id, const double value)
{
	_cells.resize(id + 1);
	_inits.resize(id + 1);
	_cells.at()
}

void Dio::Storage::Clear()
{
	_cells.clear();
	_inits.clear();
}

void Dio::Storage::AddCostantants(SymbolTable & symbol_table)
{
}

