#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "ast.h"
#include <string>
#include <iostream>

class IVisitor {
    protected:
        AST* ast;
    public:
        IVisitor(AST* tree) : ast(tree) {};
        ~IVisitor() { delete ast; }
        void visit(AST* cur = NULL, int depth = 0);
    private:
        virtual void beforeVisit(AST* cur, int depth) = 0;
        virtual void afterVisit(AST* cur, int depth) = 0;
        virtual void beforeChildren(AST* cur, int depth) = 0;
        virtual void afterChildren(AST* cur, int depth) = 0;
        virtual void beforeChild(AST* cur, int depth) = 0;
        virtual void afterChild(AST* cur, int depth) = 0;
};

class IVisitorFactory {
    public:
        virtual IVisitor* createIVisitor(AST* root) = 0;
};

#endif
