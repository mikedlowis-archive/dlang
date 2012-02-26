#include "btparser.h"
#include "exception.h"

BTParser::BTParser(ILexer* lxer) : lexer(lxer), current(0)
{
}

BTParser::~BTParser()
{
    if(lexer != NULL)
    {
        delete lexer;
        // Input stream was deleted with the lexer so null it out
        IParser::setInput((istream*)NULL);
    }
}

void BTParser::setInput(char* in)
{
    IParser::setInput(in);
    lexer->setInput(in);
}

void BTParser::setInput(string& in)
{
    IParser::setInput(in);
    lexer->setInput(in);
}

void BTParser::setInput(istream* in)
{
    IParser::setInput(in);
    lexer->setInput(in);
}

void BTParser::consume(void)
{
    current++;
    if((current == lookahead.size()) && !isSpeculating())
    {
        current = 0;
        lookahead.clear();
    }
    sync(1);
}

void BTParser::sync(unsigned int i)
{
    unsigned int next_index = current + i - 1;
    unsigned int max_index = (lookahead.size() == 0) ? 0 : (lookahead.size() - 1);
    if( next_index >= max_index )
    {
        fill( next_index - max_index);
    }
}

void BTParser::fill(unsigned int n)
{
    unsigned int i = 0;
    for (i = 0; i <= n; i++)
    {
        lookahead.push_back( lexer->next() );
    }
}

void BTParser::match(TokenType_T type)
{
    if( lookaheadType(1) == type )
    {
        consume();
    }
    else
    {
        Token& tok = lookaheadToken(1);
        ostringstream oss;
        oss << "Expected token type. Expected " << type << ", received " << tok.type() << ".";
        Exception ex( tok.line(), tok.column() );
        ex.setMessage(oss.str());
    }
}

Token& BTParser::lookaheadToken(unsigned int i)
{
    sync(i);
    return lookahead.at( current + i - 1 );
}

TokenType_T BTParser::lookaheadType(unsigned int i)
{
    return lookaheadToken(i).type();
}

unsigned int BTParser::mark(void)
{
    markers.push_back(current);
    return current;
}

void BTParser::release(void)
{
    unsigned int marker = markers.back();
    markers.pop_back();
    seek(marker);
}

void BTParser::seek(unsigned int index)
{
    current = index;
}

bool BTParser::isSpeculating(void)
{
    return (markers.size() > 0);
}

