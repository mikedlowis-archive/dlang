#include "dlparser.h"
#include "exception.h"
#include "common.h"

using namespace std;

DLParser::DLParser() : BTParser()
{
    pattern_types.insert( pair<string,PatternType_T>( "M", MAP_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "V", VECT_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "L", LIST_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "B", BLK_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "I", ID_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "N", NUM_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "C", CHAR_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "St", STR_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "Sy", SYM_TYP ));
    pattern_types.insert( pair<string,PatternType_T>( "E", EXPR_TYP ));
}

DLParser::~DLParser()
{
}

void DLParser::parse(void)
{
    result = Program();
}

AST* DLParser::Program(void)
{
    AST* node = _new AST( PROGRAM );
    while( lookaheadType(1) != EOF )
    {
        node->addChild( Expression() );
    }
    return node;
}

AST* DLParser::Expression(void)
{
    AST* ret = NULL;

    // Expression := Application
    //             | Literal
    //
    // Application := define '(' ID Expression ')'
    //              | set '(' ID Expression ')'
    //              | lambda '(' IdList ExpList? ')'
    //              | begin '(' ExpList* ')'
    //              | if '(' Expression Expression Expression? ')'
    //              | ID '(' ExpList ')'
    //
    // ExpList := Expression+
    //
    // IdList := '(' ID* ')'
    //
    // Literal := ID
    //          | CHAR
    //          | SYMBOL
    //          | STRING
    //          | NUMBER

    if ((ID == lookaheadType(1)) && (LPAR == lookaheadType(2)))
    {
        // Get the ID text
        Token id = lookaheadToken(1);
        std::string id_text = id.text();
        consume();

        match(LPAR);
        if (0 == id_text.compare("define"))
        {
            ret = new AST( lookaheadToken(1) );
            match(ID);
            ret = new AST(DEFINE, 2, ret, Expression());
        }
        else if (0 == id_text.compare("set"))
        {
            ret = new AST( lookaheadToken(1) );
            match(ID);
            ret = new AST(ASSIGN, 2, ret, Expression());
        }
        else if (0 == id_text.compare("lambda"))
        {
            ret = new AST(LAMBDA, 2, IdList(), ExpList());
        }
        else if (0 == id_text.compare("begin"))
        {
            ret = new AST(BEGIN, 1, ExpList());
        }
        else if (0 == id_text.compare("if"))
        {
            ret = new AST(IF, 2, Expression(), Expression());
            if(lookaheadType(1) != RPAR)
            {
                ret->addChild( Expression() );
            }
        }
        //else if (0 == id_text.compare("macro"))
        //{
        //}
        else
        {
            ret = new AST( id );
            ret = new AST(APPLY, 2, ret, ExpList());
        }
        match(RPAR);
    }
    else if( LPAR == lookaheadType(1) )
    {
        AST* op       = NULL;
        AST* operand1 = NULL;
        AST* operand2 = NULL;
        Token op_tok;

        // Left Operand
        match(LPAR);
        operand1 = Expression();

        // Operator
        op_tok = lookaheadToken(1);
        match(ID);
        op = new AST(op_tok);

        // Right Operand
        operand2 = Expression();
        match(RPAR);

        ret = new AST(APPLY, 3, op, operand1, operand2);
    }
    else
    {
        ret = new AST( lookaheadToken(1) );
        consume();
    }

    return ret;
}

AST* DLParser::ExpList(void)
{
    AST* ret = new AST(EXP_LIST);
    while(RPAR != lookaheadType(1))
    {
        ret->addChild( Expression() );
    }
    return ret;
}

AST* DLParser::IdList(void)
{
    AST* ret = new AST(ID_LIST);
    match(LPAR);
    while(ID == lookaheadType(1))
    {
        ret->addChild( new AST( lookaheadToken(1) ) );
        consume();
    }
    match(RPAR);
    return ret;
}

