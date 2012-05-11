#ifndef DLPARSER_H
#define DLPARSER_H

#include <map>
#include "btparser.h"
#include "dllexer.h"
#include "macro.h"

class DLParser : public BTParser
{
    private:
        std::map<std::string,eTokenTypes> core_forms;
        std::map<std::string,Macro*> macros;
    public:
        DLParser();
        ~DLParser();
        bool isMacroName(void);
        bool isCoreFormName(void);
        eTokenTypes getCoreFormId(void);
        void parse(void);

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
