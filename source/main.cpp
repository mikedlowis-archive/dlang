#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "dlparser.h"
#include "sexp.h"
#include "scheme.h"
#include "cork.h"

using namespace std;

static bool fileExists(char* fname);
static string createTempFileName(string fname);

int main(int argc, char** argv)
{
    int ret = 0;

    if( (argc == 2) && fileExists( argv[1] ) )
    {
        //string input_fname(argv[1]);
        //string temp_fname = createTempFileName( input_fname );
        //ifstream input(input_fname.c_str());
        //DLLexer lexer;
        //lexer.setInput(&input);
        //lexer.next();
        //(void)temp_fname;

        string input_fname(argv[1]);
        string temp_fname = createTempFileName( input_fname );
        (void)temp_fname;
        DLParser parser;
        Scheme* visitor = NULL;
        AST* parse_tree = NULL;

        // Open the input and output files
        ifstream input(input_fname.c_str());
        //ofstream output(temp_fname.c_str());

        // Parse the file
        parser.setInput(&input);
        parser.parse();
        parse_tree = parser.getAST();

        // Translate the AST
        visitor = _new Scheme( parse_tree );
        //visitor->visit();

        // Write to a temp file
        //output << visitor->str();
        //cout << visitor->str();
        //output.close();

        // Compile temp file
        //system(("csc " + temp_fname).c_str());

        //// delete temp file
        //remove(temp_fname.c_str());
        //delete visitor;
        delete parse_tree;
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

    REPORT_LEAKS();

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

