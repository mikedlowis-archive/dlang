#ifndef Scheme_H
#define Scheme_H

#include <iostream>
#include <sstream>
#include "ivisitor.h"
#include "dllexer.h"

class Scheme : public IVisitor {
    protected:
        std::ostream& stream;
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
};

#endif
