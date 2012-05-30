// Unit Test Framework Includes
#include "UnitTest++.h"

// Supporting Includes
#include <sstream>
#include "exception.h"

// File To Test
#include "dllexer.h"

using namespace UnitTest;

std::istringstream* input_stream = 0;

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

#define CHECK_TOKEN(typ,txt,ln,col) \
    CHECK(lexer->next() == Token(typ,txt,ln,col))

DLLexer* SetupLexer(const std::string& input)
{
    if( input_stream != 0 ) delete input_stream;
    input_stream = new std::istringstream( input );
    DLLexer* lexer = new DLLexer( *input_stream );
    return lexer;
}

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
namespace {

    //-------------------------------------------------------------------------
    // Recognize and Ignore Whitespace
    //-------------------------------------------------------------------------
    TEST(Recognize_And_Ignore_Whitespace)
    {
        DLLexer* lexer = SetupLexer( "foo \t\r\n bar" );
        CHECK_TOKEN( ID, "foo", 1, 3 );
        CHECK_TOKEN( ID, "bar", 2, 4 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Recognize and Ignore Comments
    //-------------------------------------------------------------------------
    TEST(Recognize_And_Ignore_Comments)
    {
        DLLexer* lexer = SetupLexer(
            "foo # Comment after valid token\r\n"
            "# Comment on a line by itself\r\n"
            "# Comment terminated by only a newline\n"
            "bar\n"
            "#\n" // An Empty comment
            "foo#\n" // No whitepace between comment and id
            "#" // A comment at the end of the file
        );
        CHECK_TOKEN( ID,  "foo",  1,  3 );
        CHECK_TOKEN( ID,  "bar",  4,  3 );
        CHECK_TOKEN( ID,  "foo",  6,  3 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test Number Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Positive_Integers)
    {
        DLLexer* lexer = SetupLexer( "0 1 2 3 4 5 6 7 8 9" );
        CHECK_TOKEN( NUM, "+0", 1, 1 );
        CHECK_TOKEN( NUM, "+1", 1, 3 );
        CHECK_TOKEN( NUM, "+2", 1, 5 );
        CHECK_TOKEN( NUM, "+3", 1, 7 );
        CHECK_TOKEN( NUM, "+4", 1, 9 );
        CHECK_TOKEN( NUM, "+5", 1, 11 );
        CHECK_TOKEN( NUM, "+6", 1, 13 );
        CHECK_TOKEN( NUM, "+7", 1, 15 );
        CHECK_TOKEN( NUM, "+8", 1, 17 );
        CHECK_TOKEN( NUM, "+9", 1, 19 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Negative_Integers)
    {
        DLLexer* lexer = SetupLexer( "-0 -1 -2 -3 -4 -5 -6 -7 -8 -9" );
        CHECK_TOKEN( NUM, "-0", 1, 2 );
        CHECK_TOKEN( NUM, "-1", 1, 5 );
        CHECK_TOKEN( NUM, "-2", 1, 8 );
        CHECK_TOKEN( NUM, "-3", 1, 11 );
        CHECK_TOKEN( NUM, "-4", 1, 14 );
        CHECK_TOKEN( NUM, "-5", 1, 17 );
        CHECK_TOKEN( NUM, "-6", 1, 20 );
        CHECK_TOKEN( NUM, "-7", 1, 23 );
        CHECK_TOKEN( NUM, "-8", 1, 26 );
        CHECK_TOKEN( NUM, "-9", 1, 29 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Positive_Floats)
    {
        DLLexer* lexer = SetupLexer( "1.0 1.1 1.2 1.3 1.4 1.5 1.6 1.7 1.8 1.9" );
        CHECK_TOKEN( NUM, "+1.0", 1, 3 );
        CHECK_TOKEN( NUM, "+1.1", 1, 7 );
        CHECK_TOKEN( NUM, "+1.2", 1, 11 );
        CHECK_TOKEN( NUM, "+1.3", 1, 15 );
        CHECK_TOKEN( NUM, "+1.4", 1, 19 );
        CHECK_TOKEN( NUM, "+1.5", 1, 23 );
        CHECK_TOKEN( NUM, "+1.6", 1, 27 );
        CHECK_TOKEN( NUM, "+1.7", 1, 31 );
        CHECK_TOKEN( NUM, "+1.8", 1, 35 );
        CHECK_TOKEN( NUM, "+1.9", 1, 39 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Negative_Floats)
    {
        DLLexer* lexer = SetupLexer( "-1.0 -1.1 -1.2 -1.3 -1.4 -1.5 -1.6 -1.7 -1.8 -1.9" );
        CHECK_TOKEN( NUM, "-1.0", 1, 4 );
        CHECK_TOKEN( NUM, "-1.1", 1, 9 );
        CHECK_TOKEN( NUM, "-1.2", 1, 14 );
        CHECK_TOKEN( NUM, "-1.3", 1, 19 );
        CHECK_TOKEN( NUM, "-1.4", 1, 24 );
        CHECK_TOKEN( NUM, "-1.5", 1, 29 );
        CHECK_TOKEN( NUM, "-1.6", 1, 34 );
        CHECK_TOKEN( NUM, "-1.7", 1, 39 );
        CHECK_TOKEN( NUM, "-1.8", 1, 44 );
        CHECK_TOKEN( NUM, "-1.9", 1, 49 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Floats_With_Positive_Exponents)
    {
        DLLexer* lexer = SetupLexer(
            "1.0e1 1.0e2 1.0e3 1.0e4 1.0e5 "
            "1e6 1e7 1e8 1e9 1e10"
        );
        CHECK_TOKEN( NUM, "+1.0e1", 1, 5 );
        CHECK_TOKEN( NUM, "+1.0e2", 1, 11 );
        CHECK_TOKEN( NUM, "+1.0e3", 1, 17 );
        CHECK_TOKEN( NUM, "+1.0e4", 1, 23 );
        CHECK_TOKEN( NUM, "+1.0e5", 1, 29 );
        CHECK_TOKEN( NUM, "+1e6",   1, 33 );
        CHECK_TOKEN( NUM, "+1e7",   1, 37 );
        CHECK_TOKEN( NUM, "+1e8",   1, 41 );
        CHECK_TOKEN( NUM, "+1e9",   1, 45 );
        CHECK_TOKEN( NUM, "+1e10",  1, 50 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Floats_With_Negative_Exponents)
    {
        DLLexer* lexer = SetupLexer(
            "1.0e-1 1.0e-2 1.0e-3 1.0e-4 1.0e-5 "
            "1e-6 1e-7 1e-8 1e-9 1e-10"
        );
        CHECK_TOKEN( NUM, "+1.0e-1", 1, 6 );
        CHECK_TOKEN( NUM, "+1.0e-2", 1, 13 );
        CHECK_TOKEN( NUM, "+1.0e-3", 1, 20 );
        CHECK_TOKEN( NUM, "+1.0e-4", 1, 27 );
        CHECK_TOKEN( NUM, "+1.0e-5", 1, 34 );
        CHECK_TOKEN( NUM, "+1e-6",   1, 39 );
        CHECK_TOKEN( NUM, "+1e-7",   1, 44 );
        CHECK_TOKEN( NUM, "+1e-8",   1, 49 );
        CHECK_TOKEN( NUM, "+1e-9",   1, 54 );
        CHECK_TOKEN( NUM, "+1e-10",  1, 60 );
        CHECK_TOKEN( EOF, "",    -1, -1 );
        delete lexer;
    }

    TEST(Throw_Exception_For_Missing_Exponent)
    {
        DLLexer* lexer = SetupLexer("1.0e-");
        CHECK_THROW( lexer->next(), Exception );
        delete lexer;
    }

    TEST(Throw_Exception_For_Invalid_Exponent)
    {
        DLLexer* lexer = SetupLexer("1.0e-a");
        CHECK_THROW( lexer->next(), Exception );
        delete lexer;
    }

    TEST(Throw_Exception_For_Missing_Decimal)
    {
        DLLexer* lexer = SetupLexer("1.");
        CHECK_THROW( lexer->next(), Exception );
        delete lexer;
    }

    TEST(Throw_Exception_For_Invalid_Decimal)
    {
        DLLexer* lexer = SetupLexer("1.a");
        CHECK_THROW( lexer->next(), Exception );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test Character Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Characters)
    {
        DLLexer* lexer = SetupLexer("'a' '\\a'");
        CHECK_TOKEN( CHAR, "a",    1,  3 );
        CHECK_TOKEN( CHAR, "\\a",  1,  8 );
        CHECK_TOKEN( EOF,  "",    -1, -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test String Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Strings)
    {
        DLLexer* lexer = SetupLexer(
            // Make Sure we recognize all valid characters for a symbol
            "\"\" \n"
            "\"a\" \n"
            "\"\\a\" \n"
        );
        CHECK_TOKEN( STRING, "",     1,   2 );
        CHECK_TOKEN( STRING, "a",    2,   3 );
        CHECK_TOKEN( STRING, "\\a",  3,   4 );
        CHECK_TOKEN( EOF,    "",    -1,  -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test Symbol Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Symbols)
    {
        DLLexer* lexer = SetupLexer(
            // Make Sure we recognize all valid characters for a symbol
            "$abc\n"
            "$ABC\n"
            "$a_123\n"
            "$a123\n"
            "$a_\n"
            "$a_a\n"
        );
        CHECK_TOKEN( SYMBOL, "abc",    1,   4 );
        CHECK_TOKEN( SYMBOL, "ABC",    2,   4 );
        CHECK_TOKEN( SYMBOL, "a_123",  3,   6 );
        CHECK_TOKEN( SYMBOL, "a123",   4,   5 );
        CHECK_TOKEN( SYMBOL, "a_",     5,   3 );
        CHECK_TOKEN( SYMBOL, "a_a",    6,   4 );
        CHECK_TOKEN( EOF,    "",       -1,  -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test Expression Terminator Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Default_Expression_Terminator)
    {
        DLLexer* lexer = SetupLexer("end");
        CHECK_TOKEN( TERM, "end",  1,  3 );
        CHECK_TOKEN( EOF,  "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Overridden_Expression_Terminator)
    {
        DLLexer* lexer = SetupLexer(";");
        lexer->terminator( ";" );
        CHECK_TOKEN( TERM, ";",    1,  1 );
        CHECK_TOKEN( EOF,  "",    -1, -1 );
        delete lexer;
    }

    TEST(Recognize_Overridden_Punctuation_Expression_Terminator)
    {
        DLLexer* lexer = SetupLexer(")");
        lexer->terminator( ")" );
        CHECK_TOKEN( TERM, ")",    1,  1 );
        CHECK_TOKEN( EOF,  "",    -1, -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test General Lexer Corner Cases
    //-------------------------------------------------------------------------
    TEST(Handle_An_Empty_Input_Stream)
    {
        DLLexer* lexer = SetupLexer( "" );
        CHECK_TOKEN( EOF, "", -1, -1 );
        delete lexer;
    }

    TEST(Handle_Recognition_At_The_End_Of_Input)
    {
        DLLexer* lexer = SetupLexer( "a" );
        CHECK_TOKEN( ID,  "a", 1, 1 );
        CHECK_TOKEN( EOF, "", -1, -1 );
        delete lexer;
    }

    //-------------------------------------------------------------------------
    // Test Character Classifier Functions
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Whitespace_Chars)
    {
        Token tok;
        DLLexer* lexer = SetupLexer( " |\t|\r|\n" );
        do
        {
            CHECK( true == lexer->isWhiteSpace() );
            tok = lexer->next();
        }
        while( tok.type() != EOF );
        delete lexer;
    }

    TEST(Recognize_Valid_Digit_Chars)
    {
        DLLexer* lexer = SetupLexer( "0 1 2 3 4 5 6 7 8 9 " );
        while( lexer->lookahead(1) != EOF )
        {
            CHECK( true == lexer->isDigit( lexer->lookahead(1) ) );
            (void)lexer->next();
            lexer->WS();
        }
        delete lexer;
    }

    TEST(Recognize_Valid_String_Chars)
    {
        // Fill buffer with valid string characters
        std::stringstream ss;
        for(int i = 0; i < 256; i++)
        {
            if( (i != '\r') && (i != '\n') && (i != '"') )
            {
                ss << (char) i;
            }
        }

        // Test that the lexer recognizes all valid string chars
        DLLexer* lexer = SetupLexer( ss.str() );
        for(int i = 0; i < 256; i++)
        {
            CHECK( true == lexer->isStringChar() );
            lexer->consume();
        }
        delete lexer;
    }

    TEST(Recognize_Invalid_String_Chars)
    {
        DLLexer* lexer = SetupLexer( "\r\n\"" );
        CHECK( false == lexer->isDigit( lexer->lookahead(1) ) );
        lexer->consume();
        CHECK( false == lexer->isDigit( lexer->lookahead(1) ) );
        lexer->consume();
        CHECK( false == lexer->isDigit( lexer->lookahead(1) ) );
        lexer->consume();
        delete lexer;
    }

    TEST(Recognize_Valid_ID_Chars)
    {
        // Fill buffer with valid ID characters
        std::stringstream ss;
        for(int i = 0; i < 255; i++)
        {
            if( (i != '\r') && (i != '\n') && (i != ' ') && (i != '\t') &&
                (i != '(') && (i != ')') && (i != '#') && (i != EOF) )
            {
                ss << (char) i;
            }
        }

        // Test that the lexer recognizes all valid ID chars
        DLLexer* lexer = SetupLexer( ss.str() );
        for(int i = 0; i < ss.str().size(); i++)
        {
            CHECK( true == lexer->isIDChar() );
            lexer->consume();
        }
        delete lexer;
    }

    TEST(Recognize_Invalid_ID_Chars)
    {
        DLLexer* lexer = SetupLexer( "\r\n \t()#" );
        for(int i = 0; i < 8; i++)
        {
            CHECK( false == lexer->isIDChar() );
            lexer->consume();
        }
        delete lexer;
    }

}
