#include "token.h"

Token::Token(TokenType_T ttype, std::string ttext, int line, int col) : tok_type(ttype), tok_text(ttext), tok_line(line), tok_col(col)
{
}

Token::Token(TokenType_T ttype, int line, int col) : tok_type(ttype), tok_line(line), tok_col(col)
{
}

TokenType_T Token::type()
{
	return tok_type;
}

const std::string& Token::text()
{
	return tok_text;
}

int Token::line()
{
	return tok_line;
}

int Token::column()
{
	return tok_col;
}

