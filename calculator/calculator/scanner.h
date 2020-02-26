#ifndef SCANNER_H_
#define SCANNER_H_
#include <string>
namespace Dio {

	enum EToken
	{
		TOKEN_END,
		TOKEN_ERROR,
		TOKEN_NUMBER,
		TOKEN_PLUS,
		TOKEN_MINUS,
		TOKEN_MULTIPLY,
		TOKEN_DIVIDE,
		TOKEN_LPARENTEESES,
		TOKEN_RPARENTEESES
	};
	class Scanner
	{
	public:
		Scanner(const std::string& buf);
		~Scanner();
		double NumBer() const;
		EToken Token() const;
		void Accept();
		void Test_tmp(const size_t len);
	private:
		std::size_t _curPos;
		void skipWhite();
		const std::string _buf;
		
		EToken _token;
		double _number;
	};
}


#endif