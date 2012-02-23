#include <exception>
#include "ilexer.h"
#include "exception.h"
#include "cork.h"

using namespace std;

ILexer::ILexer() : line(-1), column(-1)
{
}

ILexer::~ILexer()
{
}

void ILexer::setInput(char* in)
{
    line = 1;
    column = 0;
    input = _new istringstream( string( in ) );
    consume();
}

void ILexer::setInput(string& in)
{
    line = 1;
    column = 0;
    input = _new istringstream( in );
    consume();
}

void ILexer::setInput(istream* in)
{
    line = 1;
    column = 0;
    input = in;
    consume();
}

bool ILexer::eof(void)
{
    return ((input == NULL) || (input->eof()));
}

void ILexer::consume(void)
{
    if(input->eof())
    {
        current = EOF;
    }
    else
    {
        current = input->get();
        if(current == '\n')
        {
            line++;
            column = 0;
        }
        else
        {
            column++;
        }
    }
}

void ILexer::match(char x) {
    if ( current == x)
    {
        consume();
    }
    else
    {
        ostringstream oss;
        oss << "Unexpected character. Expected " << x << ", received " << current << ".";
        Exception ex(line,column);
        ex.setMessage(oss.str());
        throw ex;
    }
}


