#ifndef TOKEN_H
#define TOKEN_H

#include <string>

typedef int TokenType_T;

class Token
{
    private:
        TokenType_T tok_type;
        std::string tok_text;
        int tok_line;
        int tok_col;
    public:
        Token(TokenType_T ttype, int line, int col);
        Token(TokenType_T ttype, std::string ttext, int line, int col);
        TokenType_T type();
        std::string text();
        int line();
        int column();
};

#endif
