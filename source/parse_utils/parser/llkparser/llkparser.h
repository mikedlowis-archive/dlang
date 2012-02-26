#ifndef LLK_PARSER_H
#define LLK_PARSER_H

#include <exception>
#include "iparser.h"
#include "ilexer.h"
#include "ast.h"

class LLKParser : public IParser
{
    private:
        int k;
        int next;
        ILexer*  lexer;
        Token* lookahead;
    public:
        LLKParser(int k_val, ILexer* lxer);
        ~LLKParser();

        void setInput(char* in);
        void setInput(string& in);
        void setInput(istream* in);

        void         consume(void);
        void         match(TokenType_T type);
        Token&       lookaheadToken(int i);
        TokenType_T  lookaheadType(int i);
        virtual AST* parse(void) = 0;
};

#endif
