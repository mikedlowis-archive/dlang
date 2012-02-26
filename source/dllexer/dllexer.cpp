#include "dllexer.h"
#include "exception.h"
#include "cork.h"

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
    { '`', LIST },
    { '%', MACRO },
    { ':', SEP },
};

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
    Token* temp = NULL;
    while ( (!input->eof()) && (temp == NULL) )
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
            temp = Id();
        }
        else if( isOperator() )
        {
            temp = MultiCharOp();
        }
        else if (isDigit())
        {
            temp = Number();
        }
        else if(current == '\'')
        {
            temp = Char();
        }
        else if(current == '"')
        {
            temp = String();
        }
        else if(current == '$')
        {
            temp = Symbol();
        }
        else
        {
            temp = SingleCharOp();
        }
    }

    if(temp !=  NULL)
    {
        ret = *(temp);
        delete temp;
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

Token* DLLexer::Id(void)
{
    ostringstream oss;
    do
    {
        oss << current;
        consume();
    }
    while(isLetter() || isDigit() || current == '_');
    return _new Token(ID, oss.str(), line, column);
}

Token* DLLexer::Number(void)
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
        return Decimal(oss);
    }

    return _new Token(NUM, oss.str(), line, column);
}

Token* DLLexer::Decimal(ostringstream& oss)
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

    return _new Token(NUM, oss.str(), line, column);
}

Token* DLLexer::Char(void)
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

    return _new Token( CHAR, oss.str(), line, column );
}

Token* DLLexer::String(void)
{
    ostringstream oss;
    match('"');
    while( isStringChar() )
    {
        oss << current;
        consume();
    }
    match('"');
    return _new Token( STRING, oss.str(), line, column );
}

Token* DLLexer::Symbol(void)
{
    ostringstream oss;
    match('$');
    do
    {
        oss << current;
        consume();
    }
    while(isLetter() || isDigit() || current == '_');
    return _new Token( SYMBOL, oss.str(), line, column );
}

Token* DLLexer::SingleCharOp(void)
{
    for(int i = 0; i < NUM_SINGLE_CHAR_MATCHES; i++)
    {
        if(current == Single_Character_Matches[i].match)
        {
            consume();
            return _new Token( Single_Character_Matches[i].type, line, column );
        }
    }
    throw Exception(line,column);
}

Token* DLLexer::MultiCharOp(void)
{
    Token* tok = NULL;
    // save the current token so we can refer back to it
    char last = current;
    // remove the current token from the buffer so we cna see the next
    consume();

    if(last == '=')
    {
        if(current == '=')
        {
            consume();
            tok = _new Token(EQ, line, column);
        }
        else
        {
            tok = _new Token(ASSIGN, line, column);
        }
    }
    else if(last == '!')
    {
        if(current == '=')
        {
            consume();
            tok = _new Token(NE, line, column);
        }
        else
        {
            tok = _new Token(NOT, line, column);
        }
    }
    else if(last == '<')
    {
        if(current == '=')
        {
            consume();
            tok = _new Token(LTE, line, column);
        }
        else
        {
            tok = _new Token(LT, line, column);
        }
    }
    else if(last == '>')
    {
        if(current == '=')
        {
            consume();
            tok = _new Token(GTE, line, column);
        }
        else
        {
            tok = _new Token(GT, line, column);
        }
    }
    else if(last == '|')
    {
        if(current == '|')
        {
            consume();
            tok = _new Token(OR, line, column);
        }
        else
        {
            tok = _new Token(PIPE, line, column);
        }
    }
    else if((last == '&') && (current == '&'))
    {
        consume();
        tok = _new Token(AND, line, column);
    }
    else
    {
        throw Exception(line,column);
    }
    return tok;
}
