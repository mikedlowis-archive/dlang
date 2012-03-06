#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "dlparser.h"
#include "astprinter.h"
#include "scheme.h"
#include "cork.h"
#include "macro.h"

using namespace std;

static bool fileExists(char* fname);
static string createTempFileName(string fname);

int main(int argc, char** argv)
{
    int ret = 0;

    if( (argc == 2) && fileExists( argv[1] ) )
    {
        string input_fname(argv[1]);
        string temp_fname = createTempFileName( input_fname );
        DLParser parser;
        ASTPrinter* visitor = NULL;

        // Open the input and output files
        ifstream input(input_fname.c_str());

        // Parse the file
        parser.setInput(&input);

        // Translate the AST
        visitor = _new ASTPrinter( parser.parse() );
        visitor->visit();

        // Write output to screen
        cout << visitor->str();

        delete visitor;
    }
    else
    {
        ret = 1;
    }

    if(ret != 0)
    {
        // Print error code specific error
        cout << "Usage: " << argv[0] << " <filename>\n" << endl;
        cerr << "Error: No input files." << endl;
    }

    Cork_ReportMemoryLeaks();

    return ret;
}

bool fileExists(char* fname)
{
    string in_fname(fname);
    ifstream file(fname,ifstream::in);
    return (file != NULL);
}

string createTempFileName(string fname)
{
    string ret_str;
    size_t ext_index = fname.find_last_of('.');
    // If we did NOT find a match, assume no extension given
    if (ext_index == string::npos)
    {
        ret_str = fname + ".scm";
    }
    // else replace the existing extension
    else
    {
        ret_str = fname;
        ret_str.replace(ext_index, (ret_str.size() -1), ".scm");
    }
    return ret_str;
}

