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
// Option Parsing Implementation
//-----------------------------------------------------------------------------
void parse_options(int argc, char* argv[])
{
    //int i;
    //for(i = 0; i < argc; i++)
    //{
    //    int len = strlen( argv[i] );
    //    if( (len >= 2) && (argv[0] == '-') && (argv[1] == '-') )
    //    {
    //        // Parse name
    //        // if expecting param
    //        //      consume the =
    //        //      parse param
    //        //      call callback
    //        //  endif
    //    }
    //    else if( (len >= 1) && (argv[0] == '-') )
    //    {
    //        // Parse name
    //        // if expecting param
    //        //      parse param
    //    }
    //    else
    //    {
    //    }
    //}
}

//-----------------------------------------------------------------------------
// Helper Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Begin Unit Tests
//-----------------------------------------------------------------------------
namespace {
    //-------------------------------------------------------------------------
    // Test Parsing of Data Type Literals
    //-------------------------------------------------------------------------
    TEST(Parse_An_Empty_Vector)
    {
        //char* options[] = {
        //    (char*) "asd",
        //    (char*) "-A",
        //    (char*) "--foo"
        //};
        //parse_options(3,options);
    }
}
