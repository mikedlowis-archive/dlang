#include <fstream>
#include "scheme.h"

using namespace std;

Scheme::Scheme() : IVisitor() {
    ifstream input("res/environment.scm");
    if (input.is_open())
    {
        while ( input.good() )
        {
            string line;
            getline(input,line);
            cout << line << endl;
        }
    }
    input.close();
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
    cout << endl;
}

void Scheme::beforeChildren(AST* cur, int depth)
{
    if( isDatatype( cur->type() ) )
    {
        printDatatype( cur );
    }
    else
    {
        cout << "(" << typeToString( cur->type() ) << cur->text();
    }
}

void Scheme::afterChildren(AST* cur, int depth)
{
    if( !isDatatype( cur->type() ) )
    {
        cout << ")";
    }
}

void Scheme::beforeChild(AST* cur, int depth)
{
    cout << endl;
    for(int i = 0; i< depth; i++)
    {
        cout << "  ";
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
            cout << "dl/" << cur->text();
            break;
        case NUM:
            cout << cur->text();
            break;
        case CHAR:
            charToString( cur->text() );
            break;
        case STRING:
            cout << '"' << cur->text() << '"';
            break;
        case SYMBOL:
            cout << '\'' << cur->text();
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
            cout << "#\\space";
            break;
        case '\n':
            cout << "#\\newline";
            break;
        case '\r':
            cout << "#\\return";
            break;
        default:
            cout << "#\\" << ch;
    }
}
