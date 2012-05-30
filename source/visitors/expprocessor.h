#ifndef EXP_PROCESSOR_H
#define EXP_PROCESSOR_H

#include <map>
#include "ivisitor.h"
#include "dllexer.h"
#include "syntax.h"

class ExpProcessor : public IVisitor {
    protected:
        std::map<std::string,Syntax*>& syntax_registry;
    public:
        ExpProcessor(std::map<std::string,Syntax*>& syntaxes);
    private:
        void beforeVisit(AST* cur, int depth);
        void afterVisit(AST* cur, int depth);
        void beforeChildren(AST* cur, int depth);
        void afterChildren(AST* cur, int depth);
        void beforeChild(AST* cur, int depth);
        void afterChild(AST* cur, int depth);
};

#endif
