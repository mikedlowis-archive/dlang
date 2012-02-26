#include "sexp.h"

using namespace std;

string SEXP::str()
{
    return stream.str();
}

void SEXP::beforeVisit(AST* cur, int depth)
{
}

void SEXP::afterVisit(AST* cur, int depth)
{
    stream << endl;
}

void SEXP::beforeChildren(AST* cur, int depth)
{
    stream << "(" << cur->type() << " " << cur->text();
}

void SEXP::afterChildren(AST* cur, int depth)
{
    stream << ")";
}

void SEXP::beforeChild(AST* cur, int depth)
{
    stream << endl;
    for(int i = 0; i< depth; i++)
    {
        stream << "  ";
    }
}

void SEXP::afterChild(AST* cur, int depth)
{
}

