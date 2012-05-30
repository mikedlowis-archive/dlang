#ifndef DLLEXER_H
#define DLLEXER_H

#include "llnlexer.h"
#include <sstream>

typedef enum TokenTypes
{
    // Core Forms
    PROGRAM    = 0,
    DEFINE     = 1,
    ASSIGN     = 2,
    IF         = 3,
    BEGIN      = 4,
    QUOTE      = 5,
    LAMBDA     = 6,
    MACRO      = 7,
    SYNTAX     = 8,

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
    protected:
        std::string terminator_string;
    public:
        DLLexer(std::istream& in);
        bool isWhiteSpace(void);
        bool isIDChar(void);
        bool isDigit(char lach);
        bool isStringChar(void);
        void WS(void);
        void COMMENT(void);
        void terminator(std::string term);
        std::string terminator(void);

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
