#include "pattern.h"

Pattern::Pattern(const std::list<PatternType_T>& patt, const AST* ast) : pattern(patt), expr_ast(ast)
{
}

Pattern::~Pattern()
{
}

std::list<PatternType_T>::iterator Pattern::begin()
{
    return pattern.begin();
}

std::list<PatternType_T>::iterator Pattern::end()
{
    return pattern.end();
}

