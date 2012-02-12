#include <exception>
#include "lexer.h"
#include "exception.h"

using namespace std;

Lexer::Lexer() : line(-1), column(-1)
{
}

Lexer::~Lexer()
{
}

void Lexer::setInput(char* in)
{
	line = 1;
	column = 0;
	input = new istringstream( string( in ) );
	consume();
}

void Lexer::setInput(string& in)
{
	line = 1;
	column = 0;
	input = new istringstream( in );
	consume();
}

void Lexer::setInput(istream* in)
{
	line = 1;
	column = 0;
	input = in;
	consume();
}

bool Lexer::eof(void)
{
	return ((input == NULL) || (input->eof()));
}

void Lexer::consume(void)
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

void Lexer::match(char x) {
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


