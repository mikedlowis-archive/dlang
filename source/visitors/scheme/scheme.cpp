#include <fstream>
#include "scheme.h"
#include "exception.h"

using namespace std;

extern char binary_res_environment_scm_start;

Scheme::Scheme(std::ostream& out) : IVisitor(), stream(out) {
    // Print scheme environment to output stream
    stream << &binary_res_environment_scm_start << endl;
}

string Scheme::typeToString(ASTNodeType type)
{
    ostringstream ret;

    switch (type)
    {
        case ID:
            ret << "ID "; break;
        case NUM:
            ret << "NUM "; break;
        case MAP:
            ret << "MAP "; break;
        case CHAR:
            ret << "CHAR "; break;
        case ADD:
            ret << "+ "; break;
        case SUB:
            ret << "- "; break;
        case MUL:
            ret << "* "; break;
        case DIV:
            ret << "/ "; break;
        case AND:
            ret << "and "; break;
        case OR:
            ret << "or "; break;
        case NOT:
            ret << "not "; break;
        case EQ:
            ret << "equal? "; break;
        case NE:
            ret << "NE "; break;
        case LT:
            ret << "< "; break;
        case GT:
            ret << "> "; break;
        case LTE:
            ret << "<= "; break;
        case GTE:
            ret << ">= "; break;
        case DEFN:
            ret << "define "; break;
        case ASSIGN:
            ret << "set! "; break;
        case MUTATE:
            ret << "obj-set! "; break;
        case PROGRAM:
            ret << "begin "; break;
        case VECTOR:
            ret << "vector "; break;
        case LIST:
            ret << "list "; break;
        case BLOCK:
            ret << "begin "; break;
        case FUNC:
            ret << "lambda "; break;
        case FN_CALL:
            ret << "apply "; break;
        case ARRY_IDX:
            ret << "ARRY_IDX "; break;
        case SEP:
            ret << "cons "; break;
        case MEMB:
            ret << "hash-table-ref "; break;

        // Print nothing for the following nodes
        case MACRO:
        case PARAMS:
            break;

        // Print out the type id (this will probably cause an error but also
        // alert us to the fact that it is not properly handled)
        default:
            ret << type; break;
    }

    return ret.str();
}

void Scheme::beforeVisit(AST* cur, int depth)
{
}

void Scheme::afterVisit(AST* cur, int depth)
{
    stream << endl;
}

void Scheme::beforeChildren(AST* cur, int depth)
{
    nodeTypeBeginAction(cur);
    if( cur->type() != MACRO )
    {
        if( isDatatype( cur->type() ) )
        {
            printDatatype( cur );
        }
        else
        {
            stream << "(" << typeToString( cur->type() ) << cur->text();
        }
    }
}

void Scheme::afterChildren(AST* cur, int depth)
{
    nodeTypeEndAction(cur);
    if( !isDatatype( cur->type() ) && (cur->type() != MACRO))
    {
        stream << ")";
    }
}

void Scheme::beforeChild(AST* cur, int depth)
{
    stream << endl;
    for(int i = 0; i< depth; i++)
    {
        stream << "  ";
    }
}

void Scheme::afterChild(AST* cur, int depth)
{
}

bool Scheme::isDatatype(ASTNodeType type)
{
    bool ret = false;
    switch(type)
    {
        case ID:
        case NUM:
        case CHAR:
        case STRING:
        case SYMBOL:
            ret = true;
            break;
        default:
            break;
    }
    return ret;
}

void Scheme::printDatatype(AST* cur)
{
    switch(cur->type())
    {
        case ID:
            stream << "dl/" << cur->text();
            break;
        case NUM:
            stream << cur->text();
            break;
        case CHAR:
            charToString( cur->text() );
            break;
        case STRING:
            stream << '"' << cur->text() << '"';
            break;
        case SYMBOL:
            stream << '\'' << cur->text();
            break;
        default:
            break;
    }
}

void Scheme::charToString(string ch)
{
    switch(ch.at(0))
    {
        case ' ':
            stream << "#\\space";
            break;

        // Convert escape sequence
        case '\\':
            switch ( ch.at(1) )
            {
                case 'a':
                    stream << "#\\alarm";
                    break;

                case 'b':
                    stream << "#\\backspace";
                    break;

                case 'n':
                    stream << "#\\newline";
                    break;

                case 'r':
                    stream << "#\\return";
                    break;

                case 't':
                    stream << "#\\tab";
                    break;

                case 'v':
                    stream << "#\\vtab";
                    break;

                default:
                    stream << "#\\" << ch;
                    break;
            };
            break;

        default:
            stream << "#\\" << ch;
    }
}

void Scheme::nodeTypeBeginAction(AST* cur)
{
    std::string text = cur->text();
    switch( cur->type() )
    {
        case MEMB:
            cur->children()->back()->type( STRING );
            break;

        case BLOCK:
            scope_stack.startScope();
            break;

        case DEFN:
            defineSymbol(cur);
            break;

        case ASSIGN:
            assignSymbol(cur);
            break;

        //TODO: Define builtin symbols and enable adding function args to scope
        //case ID:
        //    referenceSymbol(cur);
        //    break;

        default:
            break;
    }
}

void Scheme::nodeTypeEndAction(AST* cur)
{
    switch( cur->type() )
    {
        case BLOCK:
            scope_stack.stopScope();
            break;

        default:
            break;
    }
}

void Scheme::defineSymbol(AST* cur)
{
    string text = cur->children()->front()->text();
    if( scope_stack.lookup( text ) == NULL )
    {
        scope_stack.define( text );
    }
    else if ( scope_stack.isLocal(text) )
    {
        Exception ex;
        ex << "Redefining local symbol: '" << text << "'.";
        throw ex;
    }
}

void Scheme::assignSymbol(AST* cur)
{
    if( cur->type() == ID )
    {
        string text = cur->children()->front()->text();
        if( scope_stack.lookup( text ) == NULL )
        {
            Exception ex;
            ex << "Symbol '" << text << "' has not been defined in this scope.";
            throw ex;
        }
    }
}

void Scheme::referenceSymbol(AST* cur)
{
    string text = cur->text();
    if( scope_stack.lookup( text ) == NULL )
    {
        Exception ex;
        ex << "Symbol '" << text << "' has not been defined in this scope.";
        throw ex;
    }
}
