#include "scanner.h"



Dio::Scanner::~Scanner()
{
	
}

Dio::Scanner::Scanner(const std::string & buf):_buf(buf),_curPos(0),_number(0.0),_token(TOKEN_ERROR)
{
	Accept();
}



double Dio::Scanner::NumBer() const
{
	return _number;
}

Dio::EToken Dio::Scanner::Token() const
{
	return _token;
}


void Dio::Scanner::Accept()
{
	
	skipWhite();
	switch (_buf[_curPos])
	{
	case '+':
		++_curPos;
		_token = TOKEN_PLUS;
		 break;
	case '-':
		++_curPos;
		_token = TOKEN_MINUS;
		break;
	case '*':
		++_curPos;
		_token = TOKEN_MULTIPLY;
		break;
	case '/':
		++_curPos;
		_token = TOKEN_DIVIDE;
		break;
	case '(':
		++_curPos;
		_token = TOKEN_LPARENTEESES;
		break;
	case ')':
		++_curPos;
		_token = TOKEN_RPARENTEESES;
		break;
	case '0':case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':case '.':
	{
		_token = TOKEN_NUMBER;
		char *pTmp = nullptr;
		_number = strtod(&_buf[_curPos], &pTmp);
		_curPos = pTmp - &_buf[0]; 
	}
		break;
	case '\0':case '\n':case '\r':case EOF:
		_token = TOKEN_END;
		break;

	default:
		_token = TOKEN_ERROR;
		break;
	}
}

void Dio::Scanner::Test_tmp(const size_t len)
{
	size_t tmp = 0;
	while (tmp < len)
	{

		Accept();
		switch (Token())
		{
		case TOKEN_DIVIDE:
			printf("/\n");
			break;
		case TOKEN_MULTIPLY:
			printf("*\n");
			break;
		case TOKEN_MINUS:
			printf("-\n");
			break;
		case TOKEN_PLUS:
			printf("+\n");
			break;
		case TOKEN_NUMBER:
			printf("%f\n", NumBer());
			break;
		default:
			break;
		}
		tmp = _curPos;
	}
}

void Dio::Scanner::skipWhite()
{
	while (isspace(_buf[_curPos]))
		_curPos++;
}
