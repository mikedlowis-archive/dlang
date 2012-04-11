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
        // AssignExpr = LogicalExpr '=' LogicalExpr
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
        // MemberExpr = Literal '.' LogicalExpr
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
        // MapLiteral = '{' (Literal ':' AssignExpr)* '}'
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
        AST* AssignExpr(void);
        AST* LogicalExpr(void);
        AST* CompExpr(void);
        AST* AddSubExpr(void);
        AST* MulDivExpr(void);
        AST* UnaryExpr(void);
        AST* GroupExpr(void);
        AST* MemberExpr(void);

        // Literal Type Rules
        AST* Literal(void);
        AST* MapLiteral(void);
        AST* VectorLiteral(void);
        AST* ListLiteral(void);
        AST* FuncLiteral(void);

        // Macro Rules
        AST* MacroDefinition(void);
        std::list<Pattern> MacroPatternList(void);
        Pattern MacroPattern(void);
        AST* MacroExpansion();
        AST* MacroPatternMatch(Pattern patt);

        // Helper rules for lists and blocks of expressions
        AST* ExpList(TokenType_T node_type, TokenType_T terminator);
        AST* ExpBlock(TokenType_T node_type, TokenType_T terminator);
};

#endif
