#include <stdio.h>
#include "token.h"

Token::Token() : tok_type(EOF), tok_text(""), tok_line(-1), tok_col(-1)
{
}

Token::Token(TokenType_T ttype, std::string ttext, int line, int col) : tok_type(ttype), tok_text(ttext), tok_line(line), tok_col(col)
{
}

Token::Token(TokenType_T ttype, int line, int col) : tok_type(ttype), tok_line(line), tok_col(col)
{
}

void Token::type(TokenType_T typ)
{
    tok_type = typ;
}

TokenType_T Token::type()
{
    return tok_type;
}

void Token::text(std::string txt)
{
    tok_text = txt;
}

std::string Token::text()
{
    return tok_text;
}

void Token::line(int ln)
{
    tok_line = ln;
}

int Token::line()
{
    return tok_line;
}

void Token::column(int col)
{
    tok_col = col;
}

int Token::column()
{
    return tok_col;
}

