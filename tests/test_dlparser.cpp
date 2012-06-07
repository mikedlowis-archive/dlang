// Unit Test Framework Includes
#include "UnitTest++.h"

// Supporting Includes
#include <sstream>
#include <list>
#include "exception.h"
#include "ivisitor.h"
#include "astprinter.h"

// File To Test
#include "dlparser.h"

using namespace UnitTest;

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------
#define CHECK_AST(left,right) CheckASTsForEquality( left, right, __LINE__ )
void CheckASTsForEquality(AST* left, AST* right, int line)
{
    bool result = ( (NULL != left) && (NULL != right) );
    if( true == result )
    {
        result = (bool)( (*left) == (*right) );
        if( !result )
        {
            ASTPrinter printer;
            UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), line),
                    "Syntax trees are not equal in CHECK_AST( left ,  right )");
            std::cout << "Printing left tree:" << std::endl;
            (*left).process( printer );
            std::cout << "Printing right tree:" << std::endl;
            (*right).process( printer );
        }
    }
    else
    {
        UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), line),
                "At least one of the given syntax trees is NULL");
    }
}

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
namespace {
    //-------------------------------------------------------------------------
    // Test isSyntaxName
    //-------------------------------------------------------------------------
    TEST(Verify_isSyntaxName_recognizes_a_registered_syntax)
    {
        DLParser parser;
        std::istringstream input("foobar");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isSyntaxName() );
    }

    TEST(Verify_isSyntaxName_returns_false_if_token_is_not_a_registered_syntax)
    {
        DLParser parser;
        std::istringstream input("foobar");
        parser.input(new DLLexer(input));
        CHECK( false == parser.isSyntaxName() );
    }

    //-------------------------------------------------------------------------
    // Test isCoreFormName
    //-------------------------------------------------------------------------
    TEST(Verify_isCoreFormName_Recognizes_def_form)
    {
        DLParser parser;
        std::istringstream input("def");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_set_form)
    {
        DLParser parser;
        std::istringstream input("set");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_if_form)
    {
        DLParser parser;
        std::istringstream input("if");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_begin_form)
    {
        DLParser parser;
        std::istringstream input("begin");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_quote_form)
    {
        DLParser parser;
        std::istringstream input("quote");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_lambda_form)
    {
        DLParser parser;
        std::istringstream input("func");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_macro_form)
    {
        DLParser parser;
        std::istringstream input("macro");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_syntax_form)
    {
        DLParser parser;
        std::istringstream input("syntax");
        parser.input(new DLLexer(input));
        CHECK( true == parser.isCoreFormName() );
    }

    TEST(Verify_isCoreFormName_Recognizes_a_non_core_form)
    {
        DLParser parser;
        std::istringstream input("foobar");
        parser.input(new DLLexer(input));
        CHECK( false == parser.isCoreFormName() );
    }

    //-------------------------------------------------------------------------
    // Test getCoreFormId
    //-------------------------------------------------------------------------
    TEST(Verify_getCoreFormId_Recognizes_def_form)
    {
        DLParser parser;
        std::istringstream input("def");
        parser.input(new DLLexer(input));
        CHECK( DEFINE == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_set_form)
    {
        DLParser parser;
        std::istringstream input("set");
        parser.input(new DLLexer(input));
        CHECK( ASSIGN == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_if_form)
    {
        DLParser parser;
        std::istringstream input("if");
        parser.input(new DLLexer(input));
        CHECK( IF == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_begin_form)
    {
        DLParser parser;
        std::istringstream input("begin");
        parser.input(new DLLexer(input));
        CHECK( BEGIN == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_quote_form)
    {
        DLParser parser;
        std::istringstream input("quote");
        parser.input(new DLLexer(input));
        CHECK( QUOTE == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_lambda_form)
    {
        DLParser parser;
        std::istringstream input("func");
        parser.input(new DLLexer(input));
        CHECK( LAMBDA == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_macro_form)
    {
        DLParser parser;
        std::istringstream input("macro");
        parser.input(new DLLexer(input));
        CHECK( MACRO == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_syntax_form)
    {
        DLParser parser;
        std::istringstream input("syntax");
        parser.input(new DLLexer(input));
        CHECK( SYNTAX == parser.getCoreFormId() );
    }

    TEST(Verify_getCoreFormId_Recognizes_a_non_core_form)
    {
        DLParser parser;
        std::istringstream input("foobar");
        parser.input(new DLLexer(input));
        //CHECK( false == parser.getCoreFormId() );
        CHECK( false );
    }
}
