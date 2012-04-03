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

    TEST(Recognize_And_Ignore_Whitespace)
    {
        std::string input("foo \t\r\n foo");
        eTokenTypes expected[] = { ID, ID, (eTokenTypes)EOF };
        TestLexerWithInput( input, expected );
    }

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

    TEST(Recognize_Valid_Numbers)
    {
        CHECK(false);
    }

    TEST(Recognize_Valid_Characters)
    {
        CHECK(false);
    }

    TEST(Recognize_Valid_Strings)
    {
        CHECK(false);
    }

    TEST(Recognize_Valid_Symbols)
    {
        CHECK(false);
    }

    TEST(Recognize_Valid_Operators)
    {
        CHECK(false);
    }

}
