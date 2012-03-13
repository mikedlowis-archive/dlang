#ifndef Scheme_H
#define Scheme_H

#include <iostream>
#include <sstream>
#include "ivisitor.h"
#include "dllexer.h"
#include "scopestack.h"

class Scheme : public IVisitor {
    protected:
        std::ostream& stream;
        ScopeStack scope_stack;
    public:
        Scheme(std::ostream& in);
        std::string typeToString(ASTNodeType type);
        bool isDatatype(ASTNodeType type);
        void printDatatype(AST* cur);
        void charToString(std::string ch);
    private:
        void beforeVisit(AST* cur, int depth);
        void afterVisit(AST* cur, int depth);
        void beforeChildren(AST* cur, int depth);
        void afterChildren(AST* cur, int depth);
        void beforeChild(AST* cur, int depth);
        void afterChild(AST* cur, int depth);
        void nodeTypeBeginAction(AST* cur);
        void nodeTypeEndAction(AST* cur);
        void defineSymbol(AST* cur);
        void assignSymbol(AST* cur);
        void referenceSymbol(AST* cur);
};

#endif
