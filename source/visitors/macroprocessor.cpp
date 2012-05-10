#include "macroprocessor.h"

MacroProcessor::MacroProcessor(std::set<std::string> &macros) : macro_registry(macros)
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
        macro_registry.insert( (*(cur->children()->begin()))->text() );
    }
}

void MacroProcessor::beforeChild(AST* cur, int depth)
{
}

void MacroProcessor::afterChild(AST* cur, int depth)
{
}

