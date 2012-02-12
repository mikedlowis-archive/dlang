#include <fstream>
#include "scheme.h"

using namespace std;

Scheme::Scheme(AST* root) : Visitor(root) {
	ifstream input("res/environment.scm");
	if (input.is_open())
	{
		while ( input.good() )
		{
			string line;
			getline(input,line);
			stream << line << endl;
		}
    }
    input.close();
}

string Scheme::str()
{
	return stream.str();
}

string Scheme::typeToString(ASTNodeType type)
{
	ostringstream ret;

	switch (type)
	{
		case ID:
			ret << "ID "; break;
		case NUM:
			ret << "NUM "; break;
		case CHAR:
			ret << "CHAR "; break;
		case ADD:
			ret << "ADD "; break;
		case SUB:
			ret << "SUB "; break;
		case MUL:
			ret << "MUL "; break;
		case DIV:
			ret << "DIV "; break;
		case AND:
			ret << "and "; break;
		case OR:
			ret << "or "; break;
		case NOT:
			ret << "NOT "; break;
		case EQ:
			ret << "EQ "; break;
		case NE:
			ret << "NE "; break;
		case LT:
			ret << "LT "; break;
		case GT:
			ret << "GT "; break;
		case LTE:
			ret << "LTE "; break;
		case GTE:
			ret << "GTE "; break;
		case MACRO:
			ret << "MACRO "; break;
		case ASSIGN:
			ret << "define "; break;
		case PROGRAM:
			ret << "begin "; break;
		case VECTOR:
			ret << "VECTOR "; break;
		case LIST:
			ret << "LIST "; break;
		case BLOCK:
			ret << "begin "; break;
		case FUNC:
			ret << "lambda "; break;
		case FN_CALL:
			ret << "FN_CALL "; break;
		case ARRY_IDX:
			ret << "ARRY_IDX "; break;
		case PARAMS:
			break;
		default:
			ret << type; break;
	}

	return ret.str();
}

void Scheme::beforeVisit(AST* cur, int depth)
{
}

void Scheme::afterVisit(AST* cur, int depth)
{
	stream << endl;
}

void Scheme::beforeChildren(AST* cur, int depth)
{
	if( isDatatype( cur->type() ) )
	{
		printDatatype( cur );
	}
	else
	{
		stream << "(" << typeToString( cur->type() ) << cur->text();
	}
}

void Scheme::afterChildren(AST* cur, int depth)
{
	if( !isDatatype( cur->type() ) )
	{
		stream << ")";
	}
}

void Scheme::beforeChild(AST* cur, int depth)
{
	stream << endl;
	for(int i = 0; i< depth; i++)
	{
		stream << "  ";
	}
}

void Scheme::afterChild(AST* cur, int depth)
{
}

bool Scheme::isDatatype(ASTNodeType type)
{
	bool ret = false;
	switch(type)
	{
		case ID:
		case NUM:
		case CHAR:
		case STRING:
		case SYMBOL:
			ret = true;
			break;
		default:
			break;
	}
	return ret;
}

void Scheme::printDatatype(AST* cur)
{
	switch(cur->type())
	{
		case ID:
			stream << "dl/" << cur->text();
			break;
		case NUM:
			stream << cur->text();
			break;
		case CHAR:
			charToString( cur->text() );
			break;
		case STRING:
			stream << '"' << cur->text() << '"';
			break;
		case SYMBOL:
			stream << '\'' << cur->text();
			break;
		default:
			break;
	}
}

void Scheme::charToString(string ch)
{
	switch(ch.at(0))
	{
		case ' ':
			stream << "#\\space";
			break;
		case '\n':
			stream << "#\\newline";
			break;
		case '\r':
			stream << "#\\return";
			break;
		default:
			stream << "#\\" << ch;
	}
}
