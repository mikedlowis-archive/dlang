#include "pattern.h"
#include "dllexer.h"

Pattern::Pattern(const std::list<PatternType_T>& patt, const AST* ast) : pattern(patt), expr_ast(ast)
{
}

Pattern::Pattern(const Pattern& patt)
{
    pattern = patt.pattern;
    expr_ast = patt.expr_ast->clone();
}

Pattern::~Pattern()
{
    delete expr_ast;
}

std::list<PatternType_T>::iterator Pattern::begin()
{
    return pattern.begin();
}

std::list<PatternType_T>::iterator Pattern::end()
{
    return pattern.end();
}

void Pattern::apply(AST* cur,std::vector<AST*>& params)
{
    if  (cur != NULL)
    {
        list<AST*>* children = cur->children();
        list<AST*>::iterator it = children->begin();

        // Visit the tree
        for(; it != children->end(); it++)
        {
            if ((*it)->type() == SYMBOL)
            {
                unsigned int arg;
                istringstream((*it)->text()) >> arg;

                if (arg <= params.size())
                {
                    AST* temp = *it;
                    *it = params[ arg - 1 ];
                    delete temp;
                }
                else
                {
                    throw "Invalid parameter number";
                }
            }
            else
            {
                apply( *it, params );
            }
        }
    }
}

AST* Pattern::accept(std::vector<AST*>& params)
{
    AST* ret = expr_ast->clone();
    apply( ret, params );
    return ret;
}

