#include "pattern.h"

Pattern::Pattern(const std::list<PatternType_T>& patt, const AST* ast) : pattern(patt), expr_ast(ast)
{
}

Pattern::~Pattern()
{
}

