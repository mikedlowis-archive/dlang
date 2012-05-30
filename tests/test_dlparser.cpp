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

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
namespace {
}
