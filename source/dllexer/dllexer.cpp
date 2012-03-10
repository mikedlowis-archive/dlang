#include "dllexer.h"
#include "exception.h"
#include "common.h"

using namespace std;

#define NUM_SINGLE_CHAR_MATCHES 14
SingleCharMatch_T Single_Character_Matches[ NUM_SINGLE_CHAR_MATCHES ] = {
    { '[', LBRACK },
    { ']', RBRACK },
    { '(', LPAR },
    { ')', RPAR },
    { '{', LBRACE },
    { '}', RBRACE },
    { ',', COMMA },
    { '+', ADD },
    { '-', SUB },
    { '*', MUL },
    { '/', DIV },
    { '%', MACRO },
    { ':', SEP },
    { '.', MEMB },
};

DLLexer::DLLexer(std::istream& in) : ILexer(in)
{
}

bool DLLexer::isWhiteSpace(void)
{
    return (current == ' ') ||
           (current == '\t') ||
           (current == '\r') ||
           (current == '\n');
}

bool DLLexer::isLetter(void)
{
    return  ((current >= 'a') && (current <= 'z')) ||
            ((current >= 'A') && (current <= 'Z'));
}

bool DLLexer::isDigit(void)
{
    return ((current >= '0') && (current <= '9'));
}

bool DLLexer::isOperator(void)
{
    return (   (current == '=')
            || (current == '!')
            || (current == '<')
            || (current == '>')
            || (current == '|')
            || (current == '&'));
}

bool DLLexer::isStringChar(void)
{
    return (    (current != '"')
             && (current != '\r')
             && (current != '\n'));
}

Token DLLexer::next(void)
{
    Token ret;
    while ( !eof() && (ret.type() == EOF) )
    {
        if (isWhiteSpace())
        {
            WS();
        }
        else if(current == '#')
        {
            COMMENT();
        }
        else if (isLetter())
        {
            Id(ret);
        }
        else if( isOperator() )
        {
            MultiCharOp(ret);
        }
        else if (isDigit())
        {
            Number(ret);
        }
        else if(current == '\'')
        {
            Char(ret);
        }
        else if(current == '"')
        {
            String(ret);
        }
        else if(current == '$')
        {
            Symbol(ret);
        }
        else
        {
            SingleCharOp(ret);
        }
    }
    return ret;
}

void DLLexer::WS(void)
{
    do
    {
        consume();
    }
    while(isWhiteSpace());
}

void DLLexer::COMMENT(void)
{
    match('#');
    do
    {
        consume();
    }
    while(   (current != '\n')
          && (current != EOF));

}

void DLLexer::Id(Token& tok)
{
    ostringstream oss;
    do
    {
        oss << current;
        consume();
    }
    while(isLetter() || isDigit() || current == '_');
    tok = Token(ID, oss.str(), line, column);
}

void DLLexer::Number(Token& tok)
{
    ostringstream oss;
    do
    {
        oss << current;
        consume();
    }
    while(isDigit());

    if(current == '.')
    {
        Decimal(tok, oss);
    }

    tok = Token(NUM, oss.str(), line, column);
}

void DLLexer::Decimal(Token& tok, std::ostringstream& oss)
{
    oss << current;
    consume();

    if(!isDigit())
    {
        Exception ex(line,column);
        ex.setMessage("Missing fractional portion of floating point number.");
        throw ex;
    }

    do
    {
        oss << current;
        consume();
    }
    while ( isDigit() );

    tok = Token(NUM, oss.str(), line, column);
}

void DLLexer::Char(Token& tok)
{
    ostringstream oss;

    match('\'');
    if(current != '\'')
    {
        oss << current;
        consume();
    }
    else
    {
        Exception ex(line,column);
        ex.setMessage("Invalid character literal.");
        throw ex;
    }
    match('\'');

    tok = Token( CHAR, oss.str(), line, column );
}

void DLLexer::String(Token& tok)
{
    ostringstream oss;
    match('"');
    while( isStringChar() )
    {
        oss << current;
        consume();
    }
    match('"');
    tok = Token( STRING, oss.str(), line, column );
}

void DLLexer::Symbol(Token& tok)
{
    ostringstream oss;
    match('$');
    do
    {
        oss << current;
        consume();
    }
    while(isLetter() || isDigit() || current == '_');
    tok = Token( SYMBOL, oss.str(), line, column );
}

void DLLexer::SingleCharOp(Token& tok)
{
    for(int i = 0; i < NUM_SINGLE_CHAR_MATCHES; i++)
    {
        if(current == Single_Character_Matches[i].match)
        {
            consume();
            tok = Token( Single_Character_Matches[i].type, line, column );
            break;
        }
    }

    if( tok.type() == EOF)
    {
        throw Exception(line,column);
    }
}

void DLLexer::MultiCharOp(Token& tok)
{
    // save the current token so we can refer back to it
    char last = current;
    // remove the current token from the buffer so we cna see the next
    consume();

    if(last == '=')
    {
        if(current == '=')
        {
            consume();
            tok = Token(EQ, line, column);
        }
        else
        {
            tok = Token(ASSIGN, line, column);
        }
    }
    else if(last == '!')
    {
        if(current == '=')
        {
            consume();
            tok = Token(NE, line, column);
        }
        else
        {
            tok = Token(NOT, line, column);
        }
    }
    else if(last == '<')
    {
        if(current == '=')
        {
            consume();
            tok = Token(LTE, line, column);
        }
        else
        {
            tok = Token(LT, line, column);
        }
    }
    else if(last == '>')
    {
        if(current == '=')
        {
            consume();
            tok = Token(GTE, line, column);
        }
        else
        {
            tok = Token(GT, line, column);
        }
    }
    else if(last == '|')
    {
        if(current == '|')
        {
            consume();
            tok = Token(OR, line, column);
        }
        else
        {
            tok = Token(PIPE, line, column);
        }
    }
    else if((last == '&') && (current == '&'))
    {
        consume();
        tok = Token(AND, line, column);
    }
    else
    {
        throw Exception(line,column);
    }
}
