#include "macroprocessor.h"

MacroProcessor::MacroProcessor(std::map<std::string,Macro*> &macros) : macro_registry(macros)
{
}

void MacroProcessor::beforeVisit(AST* cur, int depth)
{
}

void MacroProcessor::afterVisit(AST* cur, int depth)
{
}

void MacroProcessor::beforeChildren(AST* cur, int depth)
{
    // If we reached a syntax use then expand it
    if (cur->type() == EXPAND)
    {
        //expandMacro(cur);
    }
}

void MacroProcessor::afterChildren(AST* cur, int depth)
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

void MacroProcessor::beforeChild(AST* cur, int depth)
{
}

void MacroProcessor::afterChild(AST* cur, int depth)
{
}

