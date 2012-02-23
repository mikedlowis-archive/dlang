#ifndef SEXP_H
#define SEXP_H

#include "ivisitor.h"
#include <iostream>
#include <sstream>

class SEXP : public IVisitor {
    protected:
        ostringstream stream;
    public:
        SEXP(AST* root) : IVisitor(root) {};
        string str();
    private:
        void beforeVisit(AST* cur, int depth);
        void afterVisit(AST* cur, int depth);
        void beforeChildren(AST* cur, int depth);
        void afterChildren(AST* cur, int depth);
        void beforeChild(AST* cur, int depth);
        void afterChild(AST* cur, int depth);
};

#endif
