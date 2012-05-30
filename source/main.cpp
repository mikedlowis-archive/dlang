#include <stdlib.h>
#include <stdio.h>
#include <sstream>
#include <fstream>
#include "dlparser.h"
#include "scheme.h"
#include "common.h"
#include "options.h"
#include "astprinter.h"

using namespace std;

static bool fileExists(char* fname);
static string createTempFileName(string fname);

int main(int argc, char** argv)
{
    int ret = 0;

    //opts_parse(argc,argv);

    if( (argc == 2) && fileExists( argv[1] ) )
    {
        // Setup input and output files
        string input_fname(argv[1]);
        string temp_fname = createTempFileName( input_fname );

        // Open input and output streams
        ifstream input(input_fname.c_str());
        ofstream output(temp_fname.c_str());

        // Setup Parser and Visitors
        DLParser parser;
        //Scheme printer(output);
        //Scheme debug_printer(std::cout);
        ASTPrinter debug_printer;
        parser.input(new DLLexer(input));

        // Parse the input stream
        parser.parse();

        // Post process the AST (converts to scheme and prints to output file)
        parser.process( debug_printer );
        //parser.process( printer );

        // Close the output file
        output.close();

        // Compile the temporary file with chicken scheme
        //system( string("csc -O5 -v " + temp_fname).c_str() );
        (void)temp_fname;

        //cout << "Removing temporary files..." << endl;
        //(void)remove( temp_fname.c_str() );
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

#ifdef DEBUG
    Cork_ReportMemoryLeaks();
#endif

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

