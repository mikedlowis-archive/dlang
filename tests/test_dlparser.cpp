// Unit Test Framework Includes
#include "UnitTest++.h"

// Supporting Includes
#include <sstream>
#include "exception.h"
#include "ivisitor.h"

// File To Test
#include "dlparser.h"

using namespace UnitTest;

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------
class TreeTester : public IVisitor {
    public:
        eTokenTypes* expected_types;
        unsigned int cur_index;
        TreeTester(eTokenTypes expected[]) : IVisitor(), expected_types(expected), cur_index(0) {}
    private:
        void afterChildren(AST* cur, int depth) {
            CHECK_EQUAL( expected_types[ cur_index ], cur->type() );
            cur_index = (expected_types[cur_index] == PROGRAM) ? cur_index : (cur_index + 1);
        }

        // Not Used Here
        void beforeVisit(AST* cur, int depth) {}
        void afterVisit(AST* cur, int depth) {}
        void beforeChildren(AST* cur, int depth) {}
        void beforeChild(AST* cur, int depth) {}
        void afterChild(AST* cur, int depth) {}
};



//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------
void TestParserWithInput(std::string& input, eTokenTypes expected_types[])
{
    // Setup
    std::istringstream input_stream(input);
    DLParser parser;
    TreeTester tester(expected_types);

    // Parse the test input
    parser.input( new DLLexer( input_stream ) );
    parser.parse();

    // Test the tree against expected format
    parser.process( tester );
}

//void TestParserThrowsException(std::string& input)
//{
//    // Setup
//    //std::istringstream input_stream(input);
//    //DLLexer* lexer = new DLLexer(input_stream);
//
//    //CHECK_THROW( lexer->next(), Exception );
//
//    // Cleanup
//    //delete lexer;
//}

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
namespace {

    //-------------------------------------------------------------------------
    // Test Parsing of Data Type Literals
    //-------------------------------------------------------------------------

    // Vector Literals
    //----------------
    TEST(Parse_An_Empty_Vector)
    {
        std::string input("[]");
        eTokenTypes expected[] = {
            VECTOR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_An_Vector_With_One_Item)
    {
        std::string input("[1]");
        eTokenTypes expected[] = {
            NUM, VECTOR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Vector_Of_Two_Items)
    {
        std::string input("[1,2]");
        eTokenTypes expected[] = {
            NUM, NUM, VECTOR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Vector_Of_Three_Items)
    {
        std::string input("[1,2,3]");
        eTokenTypes expected[] = {
            NUM, NUM, NUM, VECTOR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Vector_With_Trailing_Commas)
    {
        std::string input("[1,]");
        eTokenTypes expected[] = {
            NUM, VECTOR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // List Literals
    //--------------
    TEST(Parse_A_List)
    {
        std::string input("()");
        eTokenTypes expected[] = {
            LIST,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_An_List_With_One_Item)
    {
        std::string input("(1,)"); // Comma is required here
        eTokenTypes expected[] = {
            NUM, LIST,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_List_Of_Two_Items)
    {
        std::string input("(1,2)");
        eTokenTypes expected[] = {
            NUM, NUM, LIST,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_List_Of_Three_Items)
    {
        std::string input("(1,2,3)");
        eTokenTypes expected[] = {
            NUM, NUM, NUM, LIST,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_List_With_Trailing_Commas)
    {
        std::string input("(1,)");
        eTokenTypes expected[] = {
            NUM, LIST,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_List_With_Heterogeneous_Elements)
    {
        std::string input("(1,$foo,bar)");
        eTokenTypes expected[] = {
            NUM, SYMBOL, ID, LIST,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Block Literals
    //---------------
    TEST(Parse_A_Block_With_No_Parameters)
    {
        std::string input("{}");
        eTokenTypes expected[] = {
            PARAMS, BLOCK, FUNC,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Block_With_One_Param)
    {
        std::string input("{|a|}");
        eTokenTypes expected[] = {
            ID, PARAMS, BLOCK, FUNC,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Block_With_Two_Params)
    {
        std::string input("{|a,b|}");
        eTokenTypes expected[] = {
            ID, ID, PARAMS, BLOCK, FUNC,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Block_With_Two_Params_That_Performs_An_Addition_Operation)
    {
        std::string input("{|a,b| a + b}");
        eTokenTypes expected[] = {
            ID, ID, PARAMS, ID, ID, ADD, BLOCK, FUNC,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Map Literals
    //-------------
    TEST(Parse_An_Empty_Map)
    {
        std::string input("@{}");
        eTokenTypes expected[] = {
            MAP,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Map_With_One_Entry)
    {
        std::string input("@{ $foo : 42 }");
        eTokenTypes expected[] = {
            SYMBOL, NUM, SEP, MAP,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Map_With_Two_Entries)
    {
        std::string input("@{ $foo : 42, $bar : 42 }");
        eTokenTypes expected[] = {
            SYMBOL, NUM, SEP, SYMBOL, NUM, SEP, MAP,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Maps_Should_Allow_A_Trailing_Comma)
    {
        std::string input("@{ $foo : 42, $bar : 42, }");
        eTokenTypes expected[] = {
            SYMBOL, NUM, SEP, SYMBOL, NUM, SEP, MAP,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Id Literals
    //------------
    TEST(Parse_An_Id)
    {
        std::string input("foo");
        eTokenTypes expected[] = {
            ID,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Number Literals
    //----------------
    TEST(Parse_A_Num)
    {
        std::string input("42");
        eTokenTypes expected[] = {
            NUM,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Character Literals
    //-------------------
    TEST(Parse_A_Char)
    {
        std::string input("'a'");
        eTokenTypes expected[] = {
            CHAR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // String Literals
    //----------------
    TEST(Parse_A_String)
    {
        std::string input("\"\"");
        eTokenTypes expected[] = {
            STRING,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Symbol Literals
    //----------------
    TEST(Parse_A_Symbol)
    {
        std::string input("$foo");
        eTokenTypes expected[] = {
            SYMBOL,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test Operators And Operator Precedence
    //-------------------------------------------------------------------------

    // Member Accessor
    //----------------
    TEST(Parse_A_Member_Acces_One_Level_Deep)
    {
        std::string input("foo.bar");
        eTokenTypes expected[] = {
            ID, ID, MEMB,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Member_Acces_Two_Levels_Deep)
    {
        std::string input("foo.bar.somethin");
        eTokenTypes expected[] = {
            ID, ID, ID, MEMB, MEMB,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Grouping Expression
    //--------------------
    //TEST(Parse_A_Grouping_Expression_With_A_Lower_Priority_Statement)
    //{
    //    std::string input("(foo.bar).somethin");
    //    eTokenTypes expected[] = {
    //        ID, ID, MEMB, ID, MEMB,
    //        PROGRAM
    //    };
    //    TestParserWithInput( input, expected );
    //}

    //TEST(Parse_A_Grouping_Expression_With_A_Higher_Priority_Statement)
    //{
    //    std::string input("(1+1).somethin");
    //    eTokenTypes expected[] = {
    //        NUM, NUM, ADD, ID, MEMB,
    //        PROGRAM
    //    };
    //    TestParserWithInput( input, expected );
    //}

    // Function Application
    //---------------------
    TEST(Parse_A_Simple_Function_Application)
    {
        std::string input("foo()");
        eTokenTypes expected[] = {
            ID, LIST, FN_CALL,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Collection Access
    //------------------
    TEST(Parse_An_Array_Indexing_Expression)
    {
        std::string input("foo[1]");
        eTokenTypes expected[] = {
            ID, NUM, ARRY_IDX,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Logical Operators
    //------------------
    TEST(Parse_A_Logical_And_Operation)
    {
        std::string input("foo && bar");
        eTokenTypes expected[] = {
            ID, ID, AND,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Logical_Or_Operation)
    {
        std::string input("foo || bar");
        eTokenTypes expected[] = {
            ID, ID, OR,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Logical_Not_Operation)
    {
        std::string input("! foo");
        eTokenTypes expected[] = {
            ID, NOT,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Arithmetic Operators
    //---------------------
    TEST(Parse_An_Addition_Operation)
    {
        std::string input("foo + bar");
        eTokenTypes expected[] = {
            ID, ID, ADD,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Subtraction_Operation)
    {
        std::string input("foo - bar");
        eTokenTypes expected[] = {
            ID, ID, SUB,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Multiplication_Operation)
    {
        std::string input("foo * bar");
        eTokenTypes expected[] = {
            ID, ID, MUL,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Division_Operation)
    {
        std::string input("foo / bar");
        eTokenTypes expected[] = {
            ID, ID, DIV,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Comparison Operators
    //---------------------
    TEST(Parse_An_Equal_Operation)
    {
        std::string input("foo == bar");
        eTokenTypes expected[] = {
            ID, ID, EQ,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Not_Equal_Operation)
    {
        std::string input("foo != bar");
        eTokenTypes expected[] = {
            ID, ID, NE,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Less_Than_Operation)
    {
        std::string input("foo < bar");
        eTokenTypes expected[] = {
            ID, ID, LT,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Greater_Than_Operation)
    {
        std::string input("foo > bar");
        eTokenTypes expected[] = {
            ID, ID, GT,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Less_Than_Or_Equal_Operation)
    {
        std::string input("foo <= bar");
        eTokenTypes expected[] = {
            ID, ID, LTE,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Greater_Than_Or_Equal_Operation)
    {
        std::string input("foo >= bar");
        eTokenTypes expected[] = {
            ID, ID, GTE,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    // Definition and Assignment
    //--------------------------
    TEST(Parse_A_Definition_Operation)
    {
        std::string input("foo := 1");
        eTokenTypes expected[] = {
            ID, NUM, DEFN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_An_Assignment_Operation)
    {
        std::string input("foo = bar");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test Macro Definition and Expansion
    //-------------------------------------------------------------------------
    TEST(Parse_A_Macro_Taking_One_Map)
    {
        std::string input("\% foo [ (M) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_Vector)
    {
        std::string input("\% foo [ (V) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_List)
    {
        std::string input("\% foo [ (L) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_Block)
    {
        std::string input("\% foo [ (B) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_taking_One_Id)
    {
        std::string input("\% foo [ (I) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_Number)
    {
        std::string input("\% foo [ (N) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_Character)
    {
        std::string input("\% foo [ (C) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_String)
    {
        std::string input("\% foo [ (St) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_Symbol)
    {
        std::string input("\% foo [ (Sy) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    TEST(Parse_A_Macro_Taking_One_Expression)
    {
        std::string input("\% foo [ (E) : $1 ]");
        eTokenTypes expected[] = {
            ID, ID, ASSIGN,
            PROGRAM
        };
        TestParserWithInput( input, expected );
    }

    //-------------------------------------------------------------------------
    // Test General Corner Cases
    //-------------------------------------------------------------------------
    TEST(Parse_An_Empty_Program)
    {
        std::string input("");
        eTokenTypes expected[] = { PROGRAM };
        TestParserWithInput( input, expected );
    }
}
