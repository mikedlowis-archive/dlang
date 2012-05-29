#include "expprocessor.h"

ExpProcessor::ExpProcessor(std::map<std::string,Macro*> &macros) : macro_registry(macros)
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
        Macro* macro = new Macro();
        macro->name( name );
        macro_registry[ macro->name() ] = macro;
    }
}

void ExpProcessor::beforeChild(AST* cur, int depth)
{
}

void ExpProcessor::afterChild(AST* cur, int depth)
{
}

