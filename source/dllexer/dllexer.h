#ifndef DLLEXER_H
#define DLLEXER_H

#include "llnlexer.h"
#include <sstream>

typedef enum TokenTypes
{
    // Symbols
    PROGRAM    = 0,
    DEFINE     = 1,
    ASSIGN     = 2,
    LAMBDA     = 3,
    BEGIN      = 4,
    IF         = 5,
    APPLY      = 6,
    ID_LIST    = 7,
    EXP_LIST   = 8,
    MACRO      = 9,
    TRANSFORM  = 10,
    MACRO_APP  = 11,
    LPAR       = 12,
    RPAR       = 13,
    COMMA      = 14,
    TERM       = 15,

    // Datatypes
    ID         = 16,
    NUM        = 17,
    CHAR       = 18,
    STRING     = 19,
    SYMBOL     = 20,
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
        bool isLetter(void);
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
