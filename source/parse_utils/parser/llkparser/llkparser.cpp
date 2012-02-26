#include "llkparser.h"
#include "exception.h"
#include "cork.h"

LLKParser::LLKParser(int k_val, ILexer* lxer) : k(k_val), next(0), lexer(lxer)
{
    if ( lexer != NULL )
    {
        lookahead = new Token[k];
    }
    else
    {
        throw std::exception();
    }
}

LLKParser::~LLKParser()
{
    if(lexer != NULL)
    {
        delete lexer;
        // Input stream was deleted with th elexer so null it out
        IParser::setInput((istream*)NULL);
    }
    if (lookahead != NULL)
    {
        delete[] lookahead;
    }
}

void LLKParser::setInput(char* in)
{
    IParser::setInput(in);
    lexer->setInput(in);
    for (int i = 0; i < k; i++)
    {
        consume();
    }
}

void LLKParser::setInput(string& in)
{
    IParser::setInput(in);
    lexer->setInput(in);
    for (int i = 0; i < k; i++)
    {
        consume();
    }
}

void LLKParser::setInput(istream* in)
{
    IParser::setInput(in);
    lexer->setInput(in);
    for (int i = 0; i < k; i++)
    {
        consume();
    }
}

void LLKParser::consume(void)
{
    if ( lookahead != NULL )
    {
        lookahead[next] = lexer->next();
        next = (next + 1) % k;
    }
}

void LLKParser::match(TokenType_T type)
{
    if( lookaheadType(1) == type )
    {
        consume();
    }
    else
    {
        throw std::exception();
    }
}

Token& LLKParser::lookaheadToken(int i)
{
    Token& ret = lookahead[(next + i - 1) % k];
    return ret;
}

TokenType_T LLKParser::lookaheadType(int i)
{
    TokenType_T ret = EOF;
    if( lookahead != NULL )
    {
        ret = lookaheadToken(i).type();
    }
    return ret;
}

