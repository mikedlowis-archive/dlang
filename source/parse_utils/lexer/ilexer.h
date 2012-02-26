#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <sstream>
#include <cstdio>
#include "token.h"

class ILexer
{
    protected:
        int line;
        int column;
        char current;
        std::istream* input;

    public:
        ILexer();
        virtual ~ILexer();

        void setInput(char* in);
        void setInput(std::string& in);
        void setInput(std::istream* in);

        void consume(void);
        void match(char x);
        bool eof(void);

        virtual Token next(void) = 0;
};

#endif
