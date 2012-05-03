#ifndef DLPARSER_H
#define DLPARSER_H

#include <map>
#include "btparser.h"
#include "dllexer.h"
#include "macro.h"

class DLParser : public BTParser
{
    private:
        std::map<std::string,Macro> macros;
        std::map<std::string,PatternType_T> pattern_types;
    public:
        DLParser();
        ~DLParser();
        void parse(void);
        bool isMacro(Token& token);
        bool speculate_GroupExpr(void);
        bool speculate_MacroPatternMatch(Pattern patt);

    private:
        // Entry Rules
        AST* Program(void);
        AST* Expression(void);
        AST* Application(void);
        AST* Literal(void);
        AST* ExpList(void);
        AST* IdList(void);
};

#endif
