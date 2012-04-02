#include "dllexer.h"
#include "exception.h"
#include "common.h"

using namespace std;

#define NUM_SINGLE_CHAR_MATCHES 12
SingleCharMatch_T Single_Character_Matches[ NUM_SINGLE_CHAR_MATCHES ] = {
    { '[', LBRACK },
    { ']', RBRACK },
    { '(', LPAR },
    { ')', RPAR },
    { '{', LBRACE },
    { '}', RBRACE },
    { ',', COMMA },
    { '+', ADD },
    { '*', MUL },
    { '/', DIV },
    { '.', MEMB },
    { '%', MACRO },
};

DLLexer::DLLexer(std::istream& in) : LLNLexer(in)
{
}

bool DLLexer::isWhiteSpace(void)
{
    return (lookahead(1) == ' ') ||
           (lookahead(1) == '\t') ||
           (lookahead(1) == '\r') ||
           (lookahead(1) == '\n');
}

bool DLLexer::isLetter(void)
{
    return  ((lookahead(1) >= 'a') && (lookahead(1) <= 'z')) ||
            ((lookahead(1) >= 'A') && (lookahead(1) <= 'Z'));
}

bool DLLexer::isDigit(void)
{
    return ((lookahead(1) >= '0') && (lookahead(1) <= '9'));
}

bool DLLexer::isOperator(void)
{
    return (   (lookahead(1) == '=')
            || (lookahead(1) == '!')
            || (lookahead(1) == '<')
            || (lookahead(1) == '>')
            || (lookahead(1) == '|')
            || (lookahead(1) == '&')
            || (lookahead(1) == ':')
            || (lookahead(1) == '@'));
}

bool DLLexer::isStringChar(void)
{
    return (    (lookahead(1) != '"')
             && (lookahead(1) != '\r')
             && (lookahead(1) != '\n'));
}

Token DLLexer::next(void)
{
    Token ret;
    while ( !eof() && (ret.type() == EOF) )
    {
        if (isWhiteSpace())
        {
            WS();
        }
        else if(lookahead(1) == '#')
        {
            COMMENT();
        }
        else if (isLetter())
        {
            Id(ret);
        }
        else if( isOperator() )
        {
            MultiCharOp(ret);
        }
        else if (isDigit())
        {
            Number(ret,false);
        }
        else if(lookahead(1) == '\'')
        {
            Char(ret);
        }
        else if(lookahead(1) == '"')
        {
            String(ret);
        }
        else if(lookahead(1) == '$')
        {
            Symbol(ret);
        }
        else if(lookahead(1) == '-')
        {
            consume();
            if(isDigit())
            {
                Number(ret,true);
            }
            else
            {
                ret = Token(SUB, line, column - 1);
            }
        }
        else
        {
            SingleCharOp(ret);
        }
    }
    return ret;
}

void DLLexer::WS(void)
{
    do
    {
        consume();
    }
    while(isWhiteSpace());
}

void DLLexer::COMMENT(void)
{
    match('#');
    do
    {
        consume();
    }
    while(   (lookahead(1) != '\n')
          && (lookahead(1) != EOF));

}

void DLLexer::Id(Token& tok)
{
    ostringstream oss;
    do
    {
        oss << lookahead(1);
        consume();
    }
    while(isLetter() || isDigit() || lookahead(1) == '_');
    tok = Token(ID, oss.str(), line, column);
}

void DLLexer::Number(Token& tok, bool isNegative)
{
    ostringstream oss;

    // Get the first part of the number
    oss << FloatingPoint(isNegative);

    // Get teh exponent if we have one
    if ( lookahead(1) == 'e')
    {
        // consume the 'e'
        oss << lookahead(1);
        consume();

        if(lookahead(1) == '-')
        {
            // Capture the sign
            oss << lookahead(1);
            consume();
        }

        if( isDigit() )
        {
            // Capture the integer part
            do
            {
                oss << lookahead(1);
                consume();
            }
            while(isDigit());
        }
        else
        {
            Exception ex(line,column);
            ex << "Integer for floating point exponent";
            throw ex;
        }
    }

    tok = Token(NUM, oss.str(), line, column);
}

std::string DLLexer::FloatingPoint(bool isNegative)
{
    ostringstream oss;

    // Make sure we capture the correct sign
    oss << ((isNegative) ? '-' : '+');

    // Capture the integer part
    do
    {
        oss << lookahead(1);
        consume();
    }
    while(isDigit());

    // Capture the decimal point if we have one
    if(lookahead(1) == '.')
    {
        Decimal(oss);
    }

    return oss.str();
}

void DLLexer::Decimal(std::ostringstream& oss)
{
    oss << lookahead(1);
    consume();

    if(!isDigit())
    {
        Exception ex(line,column);
        ex << "Missing fractional portion of floating point number.";
        throw ex;
    }

    do
    {
        oss << lookahead(1);
        consume();
    }
    while ( isDigit() );
}

void DLLexer::Char(Token& tok)
{
    ostringstream oss;

    match('\'');
    if(lookahead(1) == '\\')
    {
        oss << EscapeSequence();
    }
    else
    {
        oss << lookahead(1);
        consume();
    }
    match('\'');

    tok = Token( CHAR, oss.str(), line, column );
}

void DLLexer::String(Token& tok)
{
    ostringstream oss;
    match('"');
    while( isStringChar() )
    {
        if(lookahead(1) == '\\')
        {
            oss << EscapeSequence();
        }
        else
        {
            oss << lookahead(1);
            consume();
        }
    }
    match('"');
    tok = Token( STRING, oss.str(), line, column );
}

void DLLexer::Symbol(Token& tok)
{
    ostringstream oss;
    match('$');
    do
    {
        oss << lookahead(1);
        consume();
    }
    while(isLetter() || isDigit() || lookahead(1) == '_');
    tok = Token( SYMBOL, oss.str(), line, column );
}

void DLLexer::SingleCharOp(Token& tok)
{
    for(int i = 0; i < NUM_SINGLE_CHAR_MATCHES; i++)
    {
        if(lookahead(1) == Single_Character_Matches[i].match)
        {
            consume();
            tok = Token( Single_Character_Matches[i].type, line, column );
            break;
        }
    }

    if( tok.type() == EOF)
    {
        Exception ex(line,column);
        ex << "Unrecognized token";
        throw ex;
    }
}

void DLLexer::MultiCharOp(Token& tok)
{
    // save the current token so we can refer back to it
    char last = lookahead(1);
    // remove the current token from the buffer so we cna see the next
    consume();

    if(last == '=')
    {
        if(lookahead(1) == '=')
        {
            consume();
            tok = Token(EQ, line, column);
        }
        else
        {
            tok = Token(ASSIGN, line, column);
        }
    }
    else if(last == '!')
    {
        if(lookahead(1) == '=')
        {
            consume();
            tok = Token(NE, line, column);
        }
        else
        {
            tok = Token(NOT, line, column);
        }
    }
    else if(last == '<')
    {
        if(lookahead(1) == '=')
        {
            consume();
            tok = Token(LTE, line, column);
        }
        else
        {
            tok = Token(LT, line, column);
        }
    }
    else if(last == '>')
    {
        if(lookahead(1) == '=')
        {
            consume();
            tok = Token(GTE, line, column);
        }
        else
        {
            tok = Token(GT, line, column);
        }
    }
    else if(last == '|')
    {
        if(lookahead(1) == '|')
        {
            consume();
            tok = Token(OR, line, column);
        }
        else
        {
            tok = Token(PIPE, line, column);
        }
    }
    else if((last == '&') && (lookahead(1) == '&'))
    {
        consume();
        tok = Token(AND, line, column);
    }
    else if(last == ':')
    {
        if(lookahead(1) == '=')
        {
            consume();
            tok = Token(DEFN, line, column);
        }
        else
        {
            tok = Token(SEP, line, column);
        }
    }
    else if(last == '@')
    {
        if(lookahead(1) == '=')
        {
            consume();
            tok = Token(IMPORT, line, column);
        }
        else
        {
            tok = Token(MAP, line, column);
        }
    }
    else
    {
        Exception ex(line,column);
        ex << "Unexpected token";
        throw ex;
    }
}

std::string DLLexer::EscapeSequence()
{
    ostringstream oss;

    oss << lookahead(1);
    consume();

    if ( lookahead(1) == 'x' )
    {
        oss << lookahead(1);
        consume();
        for(int i = 0; i < 2; i++)
        {
            if ( ((lookahead(1) >= '0') || (lookahead(1) <= '9')) ||
                 ((lookahead(1) >= 'a') || (lookahead(1) <= 'f')) ||
                 ((lookahead(1) >= 'A') || (lookahead(1) <= 'F')))
            {
                oss << lookahead(1);
                consume();
            }
            else
            {
                Exception ex(line,column);
                ex << "Invalid hexadecimal escape sequence.";
                throw ex;
            }
        }
    }
    else
    {
        oss << lookahead(1);
        consume();
    }
    return oss.str();
}

