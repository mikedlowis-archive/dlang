#ifndef DLPARSER_H
#define DLPARSER_H

#include <map>
#include "btparser.h"
#include "dllexer.h"

class Macro{};

class DLParser : public BTParser
{
    private:
        std::map<std::string,Macro*> macros;
    public:
        DLParser();
        ~DLParser();
        AST* parse(void);
        bool isMacro(Token& token);
        bool speculate_GroupExpr(void);

        /**********************************************************************
         * EBNF Syntax Grammar
         *********************************************************************/
        // Program    = Expression*
        //
        // Expression = ID '=' LogicalExpr
        //            | MacroDefinition
        //            | $MacroExpansion$
        //            | LogicalExpr
        //
        // LogicalExpr = CompExpr (('&&' | '||') CompExpr)*
        //
        // CompExpr = AddSubExpr (('==' | '!=' | '<' | '>' | '<=' | '>=') AddSubExpr)*
        //
        // AddSubExpr = MulDivExpr (('+' | '-') MulDivExpr)*
        //
        // MulDivExpr = UnaryExpr (('*' | '/') UnaryExpr)*
        //
        // UnaryExpr = '!' GroupExpr
        //           | GroupExpr
        //
        // GroupExpr = '(' LogicalExpr ')'
        //           | '(' LogicalExpr ')' '(' ExpList ')'
        //           | '(' LogicalExpr ')' '[' LogicalExpr ']'
        //           | Literal
        //           | Literal '(' ExpList ')'
        //           | Literal '[' LogicalExpr ']'
        //
        // Literal = VectorLiteral
        //         | ListLiteral
        //         | FuncLiteral
        //         | ID
        //         | NUM
        //         | CHAR
        //         | STRING
        //         | SYMBOL
        //
        // VectorLiteral = '[' ExpList ']'
        //
        // ListLiteral = '(' ExpList ')'
        //
        // FuncLiteral = '{' ExpBlock '}'
        //             |  '{' '|' ExpList '|' ExpBlock '}'
        //
        // MacroDefinition = '%' ID '[' MacroParamList ']'
        //
        // MacroPatternList = MacroPattern (',' MacroPattern)*
        //
        // MacroPattern = '(' ID+ ')' ':' LogicalExpr
        //
        // ExpList = (GroupExpr (',' GroupExpr)*)?
        //
        // ExpBlock = Expression*
    private:
        // Entry Rules
        AST* Program(void);

        // Order of Precedence rules for expressions
        AST* Expression(void);
        AST* LogicalExpr(void);
        AST* CompExpr(void);
        AST* AddSubExpr(void);
        AST* MulDivExpr(void);
        AST* UnaryExpr(void);
        AST* GroupExpr(void);

        // Literal Type Rules
        AST* Literal(void);
        AST* VectorLiteral(void);
        AST* ListLiteral(void);
        AST* FuncLiteral(void);

        // Macro Rules
        AST* MacroDefinition(void);
        AST* MacroPatternList(void);
        AST* MacroPattern(void);

        // Helper rules for lists and blocks of expressions
        AST* ExpList(TokenType_T node_type, TokenType_T terminator);
        AST* ExpBlock(TokenType_T node_type, TokenType_T terminator);
};

#endif
