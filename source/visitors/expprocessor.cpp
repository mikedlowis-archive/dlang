#include "expprocessor.h"

ExpProcessor::ExpProcessor(std::map<std::string,Syntax*> &syntaxes) : syntax_registry(syntaxes)
{
}

void ExpProcessor::beforeVisit(AST* cur, int depth)
{
}

void ExpProcessor::afterVisit(AST* cur, int depth)
{
}

void ExpProcessor::beforeChildren(AST* cur, int depth)
{
    // If we reached a syntax use then expand it
    if (cur->type() == EXPAND)
    {
        //expandMacro(cur);
    }
}

void ExpProcessor::afterChildren(AST* cur, int depth)
{
    // If we have a new syntax definition then register it
    if (cur->type() == SYNTAX)
    {
        std::string name = (*(cur->children()->begin()))->text();
        Syntax* syntax = new Syntax();
        syntax->name( name );
        syntax_registry[ syntax->name() ] = syntax;
    }
}

void ExpProcessor::beforeChild(AST* cur, int depth)
{
}

void ExpProcessor::afterChild(AST* cur, int depth)
{
}

