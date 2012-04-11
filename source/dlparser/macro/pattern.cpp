#include "pattern.h"
#include "dllexer.h"
#include "exception.h"

using namespace std;

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
                AST* temp = *it;
                *it = expand( *it, params );
                delete temp;
            }
            else
            {
                apply( *it, params );
            }
        }
    }
}

AST* Pattern::expand(const AST* cur,std::vector<AST*>& params)
{
    AST* ret = NULL;
    unsigned int arg;
    istringstream(cur->text()) >> arg;

    if (arg <= params.size())
    {
        ret = params[ arg - 1 ];
    }
    else
    {
        Exception ex;
        ex << "Invalid parameter number";
        throw ex;
    }

    return ret;
}

AST* Pattern::accept(std::vector<AST*>& params)
{
    AST* ret = NULL;

    if( expr_ast->type() == SYMBOL )
    {
        ret = expand( expr_ast, params );
    }
    else
    {
        ret = expr_ast->clone();
        apply( ret, params );
    }

    return ret;
}

