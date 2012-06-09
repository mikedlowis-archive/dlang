#ifndef DLLEXER_H
#define DLLEXER_H

#include "llnlexer.h"
#include <sstream>

typedef enum TokenTypes
{
    UNKNOWN    = 0,

    // Core Forms
    PROGRAM    = 1,
    DEFINE     = 2,
    ASSIGN     = 3,
    IF         = 4,
    BEGIN      = 5,
    QUOTE      = 6,
    LAMBDA     = 7,
    MACRO      = 8,
    SYNTAX     = 9,

    // Punctuation and Symbols
    LPAR       = 10,
    RPAR       = 11,
    TERM       = 12,

    // Actions and Virtual nodes
    APPLY      = 20,
    EXPAND     = 21,
    TRANSFORM  = 22,
    LIST       = 23,

    // Datatypes
    ID         = 30,
    NUM        = 31,
    CHAR       = 32,
    STRING     = 33,
    SYMBOL     = 34,
} eTokenTypes;

typedef struct {
    char match;
    eTokenTypes type;
} SingleCharMatch_T;

class DLLexer : public LLNLexer {
    public:
        DLLexer(std::istream& in);
        bool isWhiteSpace(void);
        bool isIDChar(void);
        bool isDigit(char lach);
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
