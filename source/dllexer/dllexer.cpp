#include "dllexer.h"
#include "exception.h"
#include "common.h"

using namespace std;

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

bool DLLexer::isDigit(char lach)
{
    return ((lach >= '0') && (lach <= '9'));
}

bool DLLexer::isStringChar(void)
{
    return (    (lookahead(1) != '"')
             && (lookahead(1) != '\r')
             && (lookahead(1) != '\n'));
}

bool DLLexer::isIDChar(void)
{
    return ( !isWhiteSpace() &&
            ('(' != lookahead(1)) &&
            (')' != lookahead(1)) &&
            ('#' != lookahead(1)) &&
            (EOF != lookahead(1)) );
}

Token DLLexer::next(void)
{
    Token ret;
    bool escaped = false;

    // Prime the buffer. For empty input strings this results in us finding
    // the EOF and skipping the loop
    (void)lookahead(1);

    // If we have non-EOF chars then process them
    while ( !eof() && (ret.type() == EOF) )
    {
        // Consume whitespace
        if (isWhiteSpace())
        {
            WS();
        }

        // Consume and ignore comments
        else if(lookahead(1) == '#')
        {
            COMMENT();
        }

        // Consume positive or negative numbers
        else if ( isDigit( lookahead(1) ) )
        {
            Number(ret,false);
        }
        else if( (lookahead(1) == '-') && isDigit( lookahead(2) ) )
        {
            consume();
            Number(ret,true);
        }

        // Consume character literals
        else if(lookahead(1) == '\'')
        {
            Char(ret);
        }

        // Consume string literals
        else if(lookahead(1) == '"')
        {
            String(ret);
        }

        // Consume symbol literals
        else if(lookahead(1) == '$')
        {
            Symbol(ret);
        }

        // Consume parentheses
        else if (lookahead(1) == '(')
        {
            consume();
            ret = Token( LPAR, "(", line, column );
        }
        else if (lookahead(1) == ')')
        {
            consume();
            ret = Token( RPAR, ")", line, column );
        }

        // Everything else (except the unescaped terminator) is considered an ID
        else
        {
            if ( lookahead(1) == '\\' )
            {
                consume();
                escaped = true;
            }

            Id(ret);
        }
    }

    if( !escaped && (ret.text().compare( "end" ) == 0) )
    {
        ret.type( TERM );
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
    while(   (lookahead(1) != '\n')
          && (lookahead(1) != EOF))
    {
        consume();
    }
}

void DLLexer::Id(Token& tok)
{
    ostringstream oss;
    do
    {
        oss << lookahead(1);
        consume();
    }
    while( isIDChar() );
    tok = Token(ID, oss.str(), line, column);
}

void DLLexer::Number(Token& tok, bool isNegative)
{
    ostringstream oss;

    // Get the first part of the number
    oss << FloatingPoint(isNegative);

    // Get the exponent if we have one
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

        if( isDigit( lookahead(1) ) )
        {
            // Capture the integer part
            do
            {
                oss << lookahead(1);
                consume();
            }
            while(isDigit( lookahead(1) ));
        }
        else
        {
            Exception ex(line,column);
            ex << "Missing integer for floating point exponent";
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
    while(isDigit(lookahead(1)));

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

    if(!isDigit(lookahead(1)))
    {
        Exception ex(line,column);
        ex << "Missing fractional portion of floating point number.";
        throw ex;
    }
    else
    {
        do
        {
            oss << lookahead(1);
            consume();
        }
        while ( isDigit(lookahead(1)) );
    }
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
    while( isIDChar() );
    tok = Token( SYMBOL, oss.str(), line, column );
}

std::string DLLexer::EscapeSequence()
{
    ostringstream oss;
    // consume backslash
    oss << lookahead(1);
    consume();
    // consume escaped char
    oss << lookahead(1);
    consume();
    return oss.str();
}

