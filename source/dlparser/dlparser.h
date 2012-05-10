#ifndef DLPARSER_H
#define DLPARSER_H

#include <map>
#include <set>
#include "btparser.h"
#include "dllexer.h"
#include "macro.h"

class DLParser : public BTParser
{
    private:
        std::map<std::string,eTokenTypes> core_forms;
        std::set<std::string> macros;
    public:
        DLParser();
        ~DLParser();
        bool isMacroName(void);
        bool isCoreFormName(void);
        eTokenTypes getCoreFormId(void);
        void parse(void);
        bool isMacro(Token& token);
        bool speculate_GroupExpr(void);
        bool speculate_MacroPatternMatch(Pattern patt);

    private:
        // Entry Rules
        AST* Program(void);
        AST* Expression(void);
        AST* CoreForm(void);
        AST* Application(void);
        AST* Literal(void);
        AST* ExpList(eTokenTypes term);
        AST* IdList(void);
};

#endif
