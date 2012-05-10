#ifndef MACRO_PROCESSOR_H
#define MACRO_PROCESSOR_H

#include <set>
#include "ivisitor.h"
#include "dllexer.h"

class MacroProcessor : public IVisitor {
    protected:
        std::set<std::string>& macro_registry;
    public:
        MacroProcessor(std::set<std::string>& macros);
    private:
        void beforeVisit(AST* cur, int depth);
        void afterVisit(AST* cur, int depth);
        void beforeChildren(AST* cur, int depth);
        void afterChildren(AST* cur, int depth);
        void beforeChild(AST* cur, int depth);
        void afterChild(AST* cur, int depth);
};

#endif
