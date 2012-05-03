// Unit Test Framework Includes
#include "UnitTest++.h"

// Supporting Includes
#include <sstream>
#include "exception.h"

// File To Test
#include "dllexer.h"

using namespace UnitTest;

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------
void TestLexerWithInput(std::string& input, eTokenTypes expected_types[])
{
    // Setup
    std::istringstream input_stream(input);
    DLLexer* lexer = new DLLexer(input_stream);
    int i = 0;
    Token tok;

    // Compare tokens
    do
    {
        tok = lexer->next();
        CHECK_EQUAL( expected_types[i], tok.type() );
        if( tok.type() != expected_types[i] )
        {
            std::cout << "Test failed at index " << i << "." << std::endl;
            break;
        }
        i++;
    }
    while(tok.type() != EOF);

    // Cleanup
    delete lexer;
}

void TestLexerThrowsException(std::string& input)
{
    // Setup
    std::istringstream input_stream(input);
    DLLexer* lexer = new DLLexer(input_stream);

    CHECK_THROW( lexer->next(), Exception );

    // Cleanup
    delete lexer;
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
        std::string input("foo \t\r\n foo");
        eTokenTypes expected[] = { ID, ID, (eTokenTypes)EOF };
        TestLexerWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Recognize and Ignore Comments
    //-------------------------------------------------------------------------
    TEST(Recognize_And_Ignore_Comments)
    {
        std::string input(
            "foo # Comment after valid token\r\n"
            "# Comment on a line by itself\r\n"
            "# Comment terminated by only a newline\n"
            "bar\n"
            "#\n" // An Empty comment
            "foo"
            "#" // A comment at the end of the file
        );
        eTokenTypes expected[] = { ID, ID, ID, (eTokenTypes)EOF };
        TestLexerWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Recognize Valid IDs
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_IDs)
    {
        std::string input(
            // Make Sure we recognize all valid characters for an ID
            "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\n"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_\n"
            "a_123\n"
            "a123\n"
            "a_\n"
            "a_a\n"
        );
        eTokenTypes expected[] = { ID, ID, ID, ID, ID, ID, (eTokenTypes)EOF };
        TestLexerWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test Number Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Numbers)
    {
        std::string input(
            // Recognize all of the digits
            "0 1 2 3 4 5 6 7 8 9\n"
            // Recognize combinations of digits
            "10 11 12 13 14 15 16 17 18 19\n"
            // Recognize floating point numbers (with and without exponents)
            "1.0 -1.0 0.1e1 10.0e-1 1e0 10e-1"
        );
        eTokenTypes expected[] = {
            NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM,
            NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM, NUM,
            NUM, NUM, NUM, NUM, NUM, NUM,
            (eTokenTypes)EOF
        };
        TestLexerWithInput( input, expected );
    }

    TEST(Recognize_Invalid_Numbers)
    {
        std::string missing_exp("1.0e-");
        TestLexerThrowsException( missing_exp );
    }

    //-------------------------------------------------------------------------
    // Test Character Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Characters)
    {
        std::string input(
            // Make Sure we recognize characters and escaped characters
            "'a' '\\a'"
        );
        eTokenTypes expected[] = {
            CHAR, CHAR, (eTokenTypes)EOF
        };
        TestLexerWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test String Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Strings)
    {
        std::string input(
            // Make Sure we recognize all valid characters for a symbol
            "\"\" \n"
            "\"a\" \n"
            "\"\\a\" \n"
        );
        eTokenTypes expected[] = {
            STRING, STRING, STRING, (eTokenTypes)EOF
        };
        TestLexerWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test Symbol Recognition
    //-------------------------------------------------------------------------
    TEST(Recognize_Valid_Symbols)
    {
        std::string input(
            // Make Sure we recognize all valid characters for a symbol
            "$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_\n"
            "$ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_\n"
            "$a_123\n"
            "$a123\n"
            "$a_\n"
            "$a_a\n"
        );
        eTokenTypes expected[] = {
            SYMBOL, SYMBOL, SYMBOL, SYMBOL, SYMBOL, SYMBOL, (eTokenTypes)EOF
        };
        TestLexerWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test Exceptional Cases
    //-------------------------------------------------------------------------
    TEST(Throw_Exceptions_For_Exceptional_Cases)
    {
        // Make sure invalid number literals throw exceptions where appropriate
        std::string num_exception1("1.0e-");
        TestLexerThrowsException( num_exception1 );

        std::string num_exception2("1.0e-a");
        TestLexerThrowsException( num_exception2 );

        std::string num_exception3("1.0e-");
        TestLexerThrowsException( num_exception3 );

        std::string num_exception4("1.a");
        TestLexerThrowsException( num_exception4 );
    }

    //-------------------------------------------------------------------------
    // Test General Corner Cases
    //-------------------------------------------------------------------------
    TEST(Handle_An_Empty_Input_Stream)
    {
        std::string input("");
        eTokenTypes expected[] = { (eTokenTypes)EOF };
        TestLexerWithInput( input, expected );
    }

    TEST(Handle_Recognition_At_The_End_Of_Input)
    {
        std::string input("a");
        eTokenTypes expected[] = { ID, (eTokenTypes)EOF };
        TestLexerWithInput( input, expected );
    }

}
