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
    if (cur->type() == MACRO_APP)
    {
        //expandMacro(cur);
    }
}

void MacroProcessor::afterChildren(AST* cur, int depth)
{
    if (cur->type() == MACRO)
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

