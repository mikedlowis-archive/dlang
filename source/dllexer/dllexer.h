#ifndef DLLEXER_H
#define DLLEXER_H

#include "llnlexer.h"
#include <sstream>

typedef enum TokenTypes
{
    // Symbols
    PROGRAM  = 0,
    DEFINE   = 1,
    ASSIGN   = 2,
    LAMBDA   = 3,
    BEGIN    = 4,
    IF       = 5,
    APPLY    = 6,
    ID_LIST  = 7,
    EXP_LIST = 8,
    LPAR     = 9,
    RPAR     = 10,

    // Datatypes
    ID       = 11,
    NUM      = 12,
    CHAR     = 13,
    STRING   = 14,
    SYMBOL   = 15,
} eTokenTypes;

typedef struct {
    char match;
    eTokenTypes type;
} SingleCharMatch_T;

class DLLexer : public LLNLexer {
    public:
        DLLexer(std::istream& in);
        bool isWhiteSpace(void);
        bool isLetter(void);
        bool isDigit(void);
        bool isStringChar(void);
        void WS(void);
        void COMMENT(void);

        Token next(void);
        void Id(Token& tok);
        void Number(Token& tok, bool isNegative);
        std::string FloatingPoint(bool isNegative);
        void Decimal(std::ostringstream& oss);
        void Char(Token& tok);
        void String(Token& tok);
        void Symbol(Token& tok);
        std::string EscapeSequence();
};

#endif
