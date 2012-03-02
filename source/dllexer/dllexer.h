#ifndef DLLEXER_H
#define DLLEXER_H

#include "ilexer.h"
#include <sstream>

typedef enum TokenTypes
{
    // Datatypes
    ID       = 0,
    NUM      = 1,
    CHAR     = 2,
    STRING   = 3,
    SYMBOL   = 4,
    LIST     = 5,
    VECTOR   = 6,
    FUNC     = 7,

    // Symbols
    LBRACK   = 10,
    RBRACK   = 11,
    LPAR     = 12,
    RPAR     = 13,
    LBRACE   = 14,
    RBRACE   = 15,
    COMMA    = 16,
    PIPE     = 17,

    // Operators
    AND      = 20,
    OR       = 21,
    NOT      = 22,
    EQ       = 23,
    NE       = 24,
    LT       = 25,
    GT       = 26,
    LTE      = 27,
    GTE      = 28,
    ASSIGN   = 29,
    ADD      = 30,
    SUB      = 31,
    MUL      = 32,
    DIV      = 33,

    // AST "Virtual" Node Types
    MACRO    = 40,
    SEP      = 41,
    PROGRAM  = 42,
    BLOCK    = 43,
    FN_CALL  = 44,
    PARAMS   = 45,
    ARRY_IDX = 46,
    PATT     = 47,
} eTokenTypes;

typedef struct {
    char match;
    eTokenTypes type;
} SingleCharMatch_T;

class DLLexer : public ILexer {
    public:
        bool isWhiteSpace(void);
        bool isLetter(void);
        bool isDigit(void);
        bool isOperator(void);
        bool isStringChar(void);
        void WS(void);
        void COMMENT(void);

        Token next(void);
        void Id(Token& tok);
        void Number(Token& tok);
        void Decimal(Token& tok, std::ostringstream& oss);
        void Char(Token& tok);
        void String(Token& tok);
        void Symbol(Token& tok);
        void SingleCharOp(Token& tok);
        void MultiCharOp(Token& tok);
};



#endif
