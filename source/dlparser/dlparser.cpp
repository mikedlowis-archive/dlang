#include "dlparser.h"
#include "exception.h"
#include "cork.h"

DLParser::DLParser() : BTParser(_new DLLexer())
{
}

DLParser::~DLParser()
{
    std::map<std::string,Macro*>::iterator iter;
    for (iter = macros.begin(); iter != macros.end(); ++iter) {
        delete (iter->second);
    }
}

AST* DLParser::parse(void)
{
    return Program();
}

bool DLParser::isMacro( Token& token )
{
    bool ret = false;
    if( (token.type() == ID) && (macros.find(token.text()) != macros.end()) )
    {
        ret = true;
    }
    return ret;
}

AST* DLParser::parseMacroParam(Param* param)
{
    AST* ret = NULL;
    switch( param->type() )
    {
        case ExpTyp:
            ret = LogicalExpr();
            break;

        case BlockTyp:
            ret = FuncLiteral();
            break;

        default:
            Token& tok = lookaheadToken(1);
            ostringstream oss;
            oss << "Expected macro parameter type. Expected " << param->type() << ", received " << tok.type() << ".";
            Exception ex( tok.line(), tok.column() );
            ex.setMessage(oss.str());
            break;
    }
    return ret;
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
    if((lookaheadType(1) == ID) && (lookaheadType(2) == ASSIGN))
    {
        AST* id_node = _new AST( ID,(char*)(lookaheadToken(1).text().c_str()) );
        consume();
        match(ASSIGN);
        ret = _new AST( ASSIGN, 2, id_node, Expression());
    }
    else if( (lookaheadType(1) == MACRO) && (lookaheadType(2) == ID))
    {
        ret = MacroDefinition();
    }
    else if( isMacro( lookaheadToken(1) ) )
    {
        ret = MacroExpansion();
    }
    else
    {
        ret = LogicalExpr();
    }
    return ret;
}

AST* DLParser::LogicalExpr(void)
{
    AST* ret = CompExpr();
    while((lookaheadType(1) == AND) || (lookaheadType(1) == OR))
    {
        ret = _new AST( lookaheadType(1), 1, ret);
        consume();
        ret->addChild( CompExpr() );
    }
    return ret;
}

AST* DLParser::CompExpr(void)
{
    AST* ret = AddSubExpr();
    while(   (lookaheadType(1) == EQ)  || (lookaheadType(1) == NE)
       || (lookaheadType(1) == LT)  || (lookaheadType(1) == GT)
       || (lookaheadType(1) == LTE) || (lookaheadType(1) == GTE))
    {
        ret = _new AST( lookaheadType(1), 1, ret);
        consume();
        ret->addChild( AddSubExpr() );
    }
    return ret;
}

AST* DLParser::AddSubExpr(void)
{
    AST* ret = MulDivExpr();
    while((lookaheadType(1) == ADD) || (lookaheadType(1) == SUB))
    {
        ret = _new AST( lookaheadType(1), 1, ret);
        consume();
        ret->addChild( MulDivExpr() );
    }
    return ret;
}

AST* DLParser::MulDivExpr(void)
{
    AST* ret = UnaryExpr();
    while((lookaheadType(1) == MUL) || (lookaheadType(1) == DIV))
    {
        ret = _new AST( lookaheadType(1), 1, ret);
        consume();
        ret->addChild( UnaryExpr() );
    }
    return ret;
}

AST* DLParser::UnaryExpr(void)
{
    AST* ret = NULL;
    if(lookaheadType(1) == NOT)
    {
        consume();
        ret = _new AST(NOT, 1, GroupExpr() );
    }
    else
    {
        ret = GroupExpr();
    }
    return ret;
}

AST* DLParser::GroupExpr(void)
{
    AST* ret = NULL;
    if(lookaheadType(1) == LPAR)
    {
        match(LPAR);
        ret = LogicalExpr();
        match(RPAR);
    }
    else
    {
        ret = Literal();
        if( lookaheadType(1) == LPAR )
        {
            match(LPAR);
            ret = _new AST( FN_CALL, 2, ret, ExpList( LIST, RPAR ) );
            match(RPAR);
        }
        else if( lookaheadType(1) == LBRACK )
        {
            match(LBRACK);
            ret = _new AST( ARRY_IDX, 2, ret, LogicalExpr() );
            match(RBRACK);
        }
    }
    return ret;
}

AST* DLParser::Literal(void)
{
    AST* node = NULL;
    switch(lookaheadType(1))
    {
        // Literal = VectorLiteral
        case LBRACK:
            node = VectorLiteral();
            break;

        // Literal = ListLiteral
        case LIST:
            node = ListLiteral();
            break;

        // Literal = FuncLiteral
        case LBRACE:
            node = FuncLiteral();
            break;

        // Literal = ID
        case ID:
            node = _new AST( ID, lookaheadToken(1).text() );
            consume();
            break;

        // Literal = NUM
        case NUM:
            node = _new AST( NUM, lookaheadToken(1).text() );
            consume();
            break;

        // Literal = CHAR
        case CHAR:
            node = _new AST( CHAR, lookaheadToken(1).text() );
            consume();
            break;

        // Literal = STRING
        case STRING:
            node = _new AST( STRING, lookaheadToken(1).text() );
            consume();
            break;

        // Literal = SYMBOL
        case SYMBOL:
            node = _new AST( SYMBOL, lookaheadToken(1).text() );
            consume();
            break;

        default:
            Token& tok = lookaheadToken(1);
            ostringstream oss;
            oss << "Expected literal type, recieved type " << tok.type() << ".";
            Exception ex( tok.line(), tok.column() );
            ex.setMessage(oss.str());
            throw ex;
    }
    return node;
}

AST* DLParser::VectorLiteral(void)
{
    AST* ret = NULL;
    match(LBRACK);
    ret = ExpList(VECTOR, RBRACK);
    match(RBRACK);
    return ret;
}

AST* DLParser::ListLiteral(void)
{
    AST* ret = NULL;
    match(LIST);
    match(LPAR);
    ret = ExpList(LIST, RPAR);
    match(RPAR);
    return ret;
}

AST* DLParser::FuncLiteral(void)
{
    AST* ret = NULL;

    match(LBRACE);
    if(lookaheadType(1) == PIPE)
    {
        match(PIPE);
        ret = ExpList(PARAMS, PIPE);
        match(PIPE);
        ret = _new AST(FUNC, 2, ret, ExpBlock(BLOCK,RBRACE));
    }
    else
    {
        ret = _new AST(FUNC, 2, _new AST(PARAMS), ExpBlock(BLOCK,RBRACE));
    }
    match(RBRACE);
    return ret;
}

// MacroDefinition = '%' ID '(' MacroParamList ')' '{' Expression '}'
AST* DLParser::MacroDefinition(void)
{
    AST* ret = NULL;
    AST* id = NULL;
    AST* params = NULL;
    AST* body = NULL;
    Macro* macro = NULL;

    match(MACRO);
    id = _new AST( ID, lookaheadToken(1).text() );
    consume();
    match(LPAR);
    params = MacroParamList();
    match(RPAR);
    match(LBRACE);
    body = Expression();
    match(RBRACE);
    ret = _new AST( MACRO, 3, id, params, body );

    macro = _new Macro( ret );
    macros.insert( std::pair<std::string,Macro*>(id->text(), macro) );

    return _new AST(MACRO);
}

AST* DLParser::MacroExpansion(void)
{
    AST* ret = NULL;
    Macro* cur_macro = macros[ lookaheadToken(1).text() ];
    list<Param*>::const_iterator it = cur_macro->params().begin();

    consume();
    for(; it != cur_macro->params().end(); it++)
    {
        (*it)->setValue( parseMacroParam( *it ) );
    }
    ret = cur_macro->apply();

    return ret;
}

// MacroParamList = MacroParam (',' MacroParam)*
AST* DLParser::MacroParamList(void)
{
    AST* ret = _new AST( PARAMS );
    ret->addChild( MacroParam() );
    while(lookaheadType(1) == COMMA)
    {
        match(COMMA);
        ret->addChild( MacroParam() );
    }
    return ret;
}

// MacroParam = ID (':' ID)?
AST* DLParser::MacroParam(void)
{
    AST* ret = _new AST( ID, lookaheadToken(1).text() );
    consume();
    if( lookaheadType(1) == SEP )
    {
        match(SEP);
        AST* type = _new AST( ID, lookaheadToken(1).text() );
        consume();
        ret = _new AST(SEP, 2, ret, type);
    }
    return ret;
}

// ExpList = (Expression (',' Expression)*)?
AST* DLParser::ExpList(TokenType_T node_type, TokenType_T terminator)
{
    AST* node = _new AST( node_type );
    if(lookaheadType(1) != terminator)
    {
        node->addChild( Expression() );
        while(lookaheadType(1) == COMMA)
        {
            match(COMMA);
            node->addChild( Expression() );
        }
    }
    return node;
}

// ExpBlock = Expression+
AST* DLParser::ExpBlock(TokenType_T node_type, TokenType_T terminator)
{
    AST* node = _new AST(node_type);
    while(lookaheadType(1) != RBRACE)
    {
        node->addChild( Expression() );
    }
    return node;
}

