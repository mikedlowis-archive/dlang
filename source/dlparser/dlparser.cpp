#include "dlparser.h"
#include "exception.h"
#include "common.h"
#include "expprocessor.h"

using namespace std;

DLParser::DLParser() : BTParser()
{
    core_forms["def"] = DEFINE;
    core_forms["set"]   = ASSIGN;
    core_forms["if"]     = IF;
    core_forms["begin"]  = BEGIN;
    core_forms["quote"]  = QUOTE;
    core_forms["lambda"] = LAMBDA;
    core_forms["macro"]  = MACRO;
    core_forms["syntax"] = SYNTAX;
}

DLParser::~DLParser()
{
}

bool DLParser::isSyntaxName(void)
{
    return (syntaxes.count( lookaheadToken(1).text() ) > 0);
}

bool DLParser::isCoreFormName(void)
{
    return (core_forms.count( lookaheadToken(1).text() ) > 0);
}

eTokenTypes DLParser::getCoreFormId(void)
{
    return core_forms[ lookaheadToken(1).text() ];
}

void DLParser::parse(void)
{
    result = Program();
}

AST* DLParser::Program(void)
{
    AST* node = new AST( PROGRAM );
    while( lookaheadType(1) != EOF )
    {
        node->addChild( Expression() );
    }
    return node;
}

AST* DLParser::Expression(void)
{
    AST* ret = NULL;

    if( isCoreFormName() )
    {
        ret = CoreForm();
    }
    else
    {
        ret = BasicExp();

        // Traditional Function Application
        if( lookaheadType(1) == LPAR )
        {
            match(LPAR);
            ret = new AST(APPLY, 2, ret, ExpList(RPAR));
            match(RPAR);
        }
    }

    // Register any new syntaxes and expand any existing syntax uses
    ExpProcessor processor( syntaxes );
    processor.visit( ret );

    return ret;
}

AST* DLParser::CoreForm(void)
{
    AST* ret = NULL;
    eTokenTypes form_id = getCoreFormId();
    consume(); // Throw away the form name (we don't need it anymore)
    switch( form_id )
    {
        case DEFINE:
        case ASSIGN:
            ret = new AST( lookaheadToken(1) );
            match(ID);
            ret = new AST(form_id, 2, ret, Expression());
            break;

        case BEGIN:
            ret = new AST(BEGIN, 1, ExpList(TERM));
            match(TERM);
            break;

        case IF:
            ret = new AST(IF, 2, Expression(), Expression());
            if(lookaheadType(1) != TERM)
            {
                ret->addChild( Expression() );
            }
            match(TERM);
            break;

        case LAMBDA:
        case MACRO:
            ret = new AST(form_id, 2, IdList(), ExpList(TERM));
            match(TERM);
            break;

        //case QUOTE:
        //    match(LPAR);
        //    ret = new AST(QUOTE, 1, Expression());
        //    break;

        //case SYNTAX:
        //    ret = new AST(SYNTAX);

        //    // Get the macro name
        //    ret->addChild( new AST( lookaheadToken(1) ) );
        //    match(ID);

        //    // Get the macro keywords
        //    ret->addChild( IdList() );

        //    // Get the macro terminator
        //    ret->addChild( new AST( lookaheadToken(1) ) );
        //    match(ID);

        //    // Get the macro transform rules
        //    while (TERM != lookaheadType(1))
        //    {
        //        AST* transform = new AST( TRANSFORM );
        //        transform->addChild( IdList() );
        //        transform->addChild( Expression() );
        //        ret->addChild( transform );
        //    }
        //    break;

        case SYNTAX:
        case QUOTE:
        default:
            throw Exception( lookaheadToken(1) );
            break;
    }
    return ret;
}

AST* DLParser::FuncApp(void)
{
    return NULL;
}

AST* DLParser::BasicExp(void)
{
    AST* ret = NULL;

    // Macro Expression
    /*if ( isSyntaxName() )
    {
        // Consume the name
        ret = new AST( EXPAND, 1, new AST( lookaheadToken(1) ));
        consume();

        // Consume the expressions
        while( lookaheadType(1) != TERM )
        {
            ret->addChild( Expression() );
        }
        match(TERM);
    }

    // Infix Function Application
    else*/ if( lookaheadType(1) == LPAR )
    {
        AST* operation = NULL;
        AST* operand1  = NULL;
        AST* operand2  = NULL;

        match(LPAR);
        operand1 = Expression();
        operation = new AST( lookaheadToken(1) );
        match(ID);
        operand2 = Expression();
        match(RPAR);

        ret = new AST( APPLY, 2, operation, new AST(LIST, 2, operand1, operand2) );
    }

    // Literal
    else
    {
        ret = Literal();
    }

    return ret;
}

AST* DLParser::Literal(void)
{
    AST* ret = new AST( lookaheadToken(1) );

    if( (ID     == lookaheadType(1)) ||
        (CHAR   == lookaheadType(1)) ||
        (STRING == lookaheadType(1)) ||
        (SYMBOL == lookaheadType(1)) ||
        (NUM    == lookaheadType(1)) )
    {
        consume();
    }
    else
    {
        Exception ex( lookaheadToken(1) );
        throw ex;
    }

    return ret;
}

AST* DLParser::ExpList(eTokenTypes term)
{
    AST* ret = new AST(LIST);
    while(term != lookaheadType(1))
    {
        ret->addChild( Expression() );
    }
    return ret;
}

AST* DLParser::IdList(void)
{
    AST* ret = new AST(LIST);
    match(LPAR);
    while(ID == lookaheadType(1))
    {
        ret->addChild( new AST( lookaheadToken(1) ) );
        consume();
    }
    match(RPAR);
    return ret;
}

