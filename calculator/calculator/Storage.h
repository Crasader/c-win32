#ifndef STORAGE_H_
#define STORAGE_H_
#include <vector>
namespace Dio
{
	class SymbolTable;
	class Storage
	{
	public:
		Storage(SymbolTable & symbol_table);
		bool IsInit(const unsigned int id)const;
		double GetValue(const unsigned int id)const;
		void SetValue(const unsigned int id, const double value);
		void AddValue(const unsigned int id, const double value);
		void Clear();
		~Storage();
	private:
		void AddCostantants(SymbolTable& symbol_table);
		std::vector<double> _cells;
		std::vector<bool> _inits;
	};
}


#endif