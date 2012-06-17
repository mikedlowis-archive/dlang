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

#define PROGRAM(num, ...) new AST(PROGRAM, "", num, __VA_ARGS__)
#define DEFINE(a,b) new AST(DEFINE,"", 2, a, b)
#define ASSIGN(a,b) new AST(ASSIGN,"", 2, a, b)
#define BEGIN(num, ...) new AST(BEGIN, "", num, __VA_ARGS__)
#define IF(num, ...) new AST(IF,"", num, __VA_ARGS__)
#define LAMBDA(num, ...) new AST(LAMBDA,"", num, __VA_ARGS__)
#define MACRO(num, ...) new AST(MACRO,"", num, __VA_ARGS__)
#define LIST(num, ...) new AST(LIST,"", num, __VA_ARGS__)
#define APPLY(a,b) new AST(APPLY, "", 2, a, b)
#define ID(text) new AST(ID,text)
#define NUM(text) new AST(NUM,text)
#define CHAR(text) new AST(CHAR,text)
#define STRING(text) new AST(STRING,text)
#define SYMBOL(text) new AST(SYMBOL,text)

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
        class TestDLParser : public DLParser
        {
            public:
                using DLParser::syntaxes;
        };
        std::string syn_name ("foobar");
        Syntax* syntax = new Syntax();
        TestDLParser parser;
        std::istringstream input("foobar");
        parser.input(new DLLexer(input));
        syntax->name( syn_name );
        parser.syntaxes[ syntax->name() ] = syntax;
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
        CHECK( UNKNOWN == parser.getCoreFormId() );
    }

    //-------------------------------------------------------------------------
    // Test parse
    //-------------------------------------------------------------------------
    TEST(Verify_parse_calls_Program_subrule)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = PROGRAM( 1, NUM("+5") );
        std::istringstream input("5");
        parser.input(new DLLexer(input));
        parser.parse();
        result = parser.ast()->clone();
        CHECK_AST( result, expected );
    }

    //-------------------------------------------------------------------------
    // Test Program
    //-------------------------------------------------------------------------
    TEST(Verify_Program_can_parse_a_group_of_expressions)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = PROGRAM( 2, NUM("+5"), NUM("+5") );
        std::istringstream input("5 5");
        parser.input(new DLLexer(input));
        parser.parse();
        result = parser.ast()->clone();
        CHECK_AST( result, expected );
    }

    //-------------------------------------------------------------------------
    // Test Expression
    //-------------------------------------------------------------------------
    TEST(Verify_Expression_parses_a_core_form)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = DEFINE( ID("foo"), NUM("+5") );
        std::istringstream input("def foo 5");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Expression_parses_a_basic_expression)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = ID("foo");
        std::istringstream input("foo");
        parser.input(new DLLexer(input));
        result = parser.Expression();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Expression_parses_a_traditional_function_call)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = APPLY(
                          ID("foo"),
                          LIST( 2,
                            NUM("+5"),
                            NUM("+5") ));
        std::istringstream input("foo(5 5)");
        parser.input(new DLLexer(input));
        result = parser.Expression();
        CHECK_AST( result, expected );
    }

    //-------------------------------------------------------------------------
    // Test CoreForm
    //-------------------------------------------------------------------------
    TEST(Verify_CoreForm_parses_def_expression)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = DEFINE( ID("foo"), NUM("+5") );
        std::istringstream input("def foo 5");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_parses_set_expression)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = ASSIGN( ID("foo"), NUM("+5") );
        std::istringstream input("set foo 5");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_parses_begin_expression)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = BEGIN( 1,
                          LIST( 1,
                            APPLY(
                              ID("+"),
                              LIST( 2,
                                NUM("+1"),
                                NUM("+1") ))));
        std::istringstream input("begin (1 + 1) end");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_parses_if_expression_with_no_else)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = IF( 2, NUM("+1"), NUM("+2") );
        std::istringstream input("if 1 2 end");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_parses_if_expression_with_else)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = IF( 3, NUM("+1"), NUM("+2"), NUM("+3"));
        std::istringstream input("if 1 2 3 end");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_parses_function_expression)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = LAMBDA( 2,
                          LIST( 1,
                            ID("a" )),
                          LIST( 1,
                            APPLY(
                              ID( "+" ),
                                LIST( 2,
                                  ID( "a" ),
                                  NUM( "+1" ) ))));
        std::istringstream input("func(a) (a + 1) end");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_parses_macro_expression)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = MACRO( 2,
                          LIST( 1,
                            ID("a" )),
                          LIST( 1,
                            APPLY(
                              ID( "+" ),
                                LIST( 2,
                                  ID( "a" ),
                                  NUM( "+1" ) ))));
        std::istringstream input("macro(a) (a + 1) end");
        parser.input(new DLLexer(input));
        result = parser.CoreForm();
        CHECK_AST( result, expected );
    }

    TEST(Verify_CoreForm_throws_exception_for_unknown_form)
    {
        DLParser parser;
        std::istringstream input("foo");
        parser.input(new DLLexer(input));
        CHECK_THROW( parser.CoreForm(), Exception );
    }

    //-------------------------------------------------------------------------
    // Test BasicExp
    //-------------------------------------------------------------------------
    TEST(Verify_BasicExp_recognizes_an_infix_function_application)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( APPLY, "", 2,
                            new AST( ID, "+" ),
                            new AST( LIST, "", 2,
                                new AST( NUM, "+1" ),
                                new AST( NUM, "+2" ) ));
        std::istringstream input("(1 + 2)");
        parser.input(new DLLexer(input));
        result = parser.BasicExp();
        CHECK_AST( result, expected );
    }

    TEST(Verify_BasicExp_recognizes_a_literal)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( ID, "foo" );
        std::istringstream input("foo");
        parser.input(new DLLexer(input));
        result = parser.BasicExp();
        CHECK_AST( result, expected );
    }

    //-------------------------------------------------------------------------
    // Test Literal
    //-------------------------------------------------------------------------
    TEST(Verify_Literal_parses_an_id)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( ID, "foo" );
        std::istringstream input("foo");
        parser.input(new DLLexer(input));
        result = parser.Literal();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Literal_parses_a_char)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( CHAR, "a" );
        std::istringstream input("'a'");
        parser.input(new DLLexer(input));
        result = parser.Literal();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Literal_parses_a_string)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( STRING, "" );
        std::istringstream input("\"\"");
        parser.input(new DLLexer(input));
        result = parser.Literal();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Literal_parses_a_symbol)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( SYMBOL, "foo" );
        std::istringstream input("$foo");
        parser.input(new DLLexer(input));
        result = parser.Literal();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Literal_parses_a_num)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( NUM, "+5" );
        std::istringstream input("5");
        parser.input(new DLLexer(input));
        result = parser.Literal();
        CHECK_AST( result, expected );
    }

    TEST(Verify_Literal_throws_exception_if_no_literal_found)
    {
        DLParser parser;
        std::istringstream input("(");
        parser.input(new DLLexer(input));
        CHECK_THROW( parser.Literal(), Exception );
    }

    //-------------------------------------------------------------------------
    // Test ExpList
    //-------------------------------------------------------------------------
    TEST(Verify_ExpList_parses_an_empty_exp_list)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( LIST, "" );
        std::istringstream input("end");
        parser.input(new DLLexer(input));
        result = parser.ExpList(TERM);
        CHECK_AST( result, expected );
    }

    TEST(Verify_ExpList_parses_an_exp_list_with_1_item)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( LIST, "", 1,
                            new AST(NUM, "+5") );
        std::istringstream input("5 end");
        parser.input(new DLLexer(input));
        result = parser.ExpList(TERM);
        CHECK_AST( result, expected );
    }

    TEST(Verify_ExpList_parses_an_exp_list_with_2_items)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( LIST, "", 2,
                            new AST(NUM, "+5"),
                            new AST(NUM, "+5") );
        std::istringstream input("5 5 end");
        parser.input(new DLLexer(input));
        result = parser.ExpList(TERM);
        CHECK_AST( result, expected );
    }

    TEST(Verify_ExpList_throws_an_exception_when_no_terminator_found)
    {
        DLParser parser;
        std::istringstream input("a");
        parser.input(new DLLexer(input));
        CHECK_THROW( parser.ExpList(TERM), Exception );
    }

    //-------------------------------------------------------------------------
    // Test IdList
    //-------------------------------------------------------------------------
    TEST(Verify_IdList_parses_an_empty_list)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( LIST, "" );
        std::istringstream input("()");
        parser.input(new DLLexer(input));
        result = parser.IdList();
        CHECK_AST( result, expected );
    }

    TEST(Verify_IdList_parses_a_list_with_one_id)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( LIST, "", 1,
                            new AST(ID,"a") );
        std::istringstream input("(a)");
        parser.input(new DLLexer(input));
        result = parser.IdList();
        CHECK_AST( result, expected );
    }

    TEST(Verify_IdList_parses_a_list_with_two_ids)
    {
        DLParser parser;
        AST* result = NULL;
        AST* expected = new AST( LIST, "", 2,
                            new AST(ID,"a"),
                            new AST(ID,"b") );
        std::istringstream input("(a b)");
        parser.input(new DLLexer(input));
        result = parser.IdList();
        CHECK_AST( result, expected );
    }

    TEST(Verify_IdList_throws_exception_on_missing_left_paren)
    {
        DLParser parser;
        std::istringstream input("a b)");
        parser.input(new DLLexer(input));
        CHECK_THROW( parser.IdList(), Exception );
    }

    TEST(Verify_IdList_throws_exception_on_missing_right_paren)
    {
        DLParser parser;
        std::istringstream input("(a b");
        parser.input(new DLLexer(input));
        CHECK_THROW( parser.IdList(), Exception );
    }

    TEST(Verify_IdList_throws_exception_when_list_contains_non_id)
    {
        DLParser parser;
        std::istringstream input("(a 5)");
        parser.input(new DLLexer(input));
        CHECK_THROW( parser.IdList(), Exception );
    }
}
