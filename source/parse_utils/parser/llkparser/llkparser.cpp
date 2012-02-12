#include "llkparser.h"
#include "exception.h"

LLKParser::LLKParser(int k_val, Lexer* lxer) : k(k_val), next(0), lexer(lxer)
{
	if ( lexer != NULL )
	{
		lookahead = new Token*[k];
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
		Parser::setInput((istream*)NULL);
	}
	if (lookahead != NULL)
	{
		delete[] lookahead;
	}
}

void LLKParser::setInput(char* in)
{
	Parser::setInput(in);
	lexer->setInput(in);
	for (int i = 0; i < k; i++)
	{
		consume();
	}
}

void LLKParser::setInput(string& in)
{
	Parser::setInput(in);
	lexer->setInput(in);
	for (int i = 0; i < k; i++)
	{
		consume();
	}
}

void LLKParser::setInput(istream* in)
{
	Parser::setInput(in);
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

Token* LLKParser::lookaheadToken(int i)
{
	Token* ret = NULL;
	if( lookahead != NULL )
	{
		ret = lookahead[(next + i - 1) % k];
	}
	return ret; 
}

TokenType_T LLKParser::lookaheadType(int i)
{
	TokenType_T ret = EOF;
	if( lookahead != NULL )
	{
		Token* tok = lookaheadToken(i);
		ret = (tok != NULL) ? tok->type() : EOF;
	}
	return ret;
}

