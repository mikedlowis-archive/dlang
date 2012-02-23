#ifndef AST_H
#define AST_H

#include <stdarg.h>
#include <list>
#include <string>

using namespace std;

typedef unsigned int ASTNodeType;

class AST
{
    protected:
        ASTNodeType node_type;
        string node_text;
        list<AST*>* node_children;
    public:
        AST(ASTNodeType type);
        AST(ASTNodeType type, const char* text);
        AST(ASTNodeType type, string text);
        AST(ASTNodeType type, int child_count, ...);
        virtual ~AST();

        AST& operator = (AST& rhs);

        ASTNodeType type(void);
        string      text(void);
        list<AST*>* children(void);
        void        addChild(AST* node);
        AST*        clone(void);
};

#endif
