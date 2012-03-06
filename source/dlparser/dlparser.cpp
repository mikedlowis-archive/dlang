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

bool DLParser::speculate_GroupExpr(void)
{
    bool success = true;

    mark();
    try
    {
        match(LPAR);
        delete LogicalExpr();
        match(RPAR);
    }
    catch (Exception e)
    {
        success = false;
    }
    release();

    return success;
}

bool DLParser::speculate_MapLiteral(void)
{
    bool success = true;

    mark();
    try
    {
        delete MapLiteral();
    }
    catch (Exception e)
    {
        success = false;
    }
    release();

    return success;
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

    if( (lookaheadType(1) == MACRO) && (lookaheadType(2) == ID))
    {
        ret = MacroDefinition();
    }
    //else if( isMacro( lookaheadToken(1) ) )
    //{
    //    ret = MacroExpansion();
    //}
    else
    {
        ret = AssignExpr();
    }
    return ret;
}

AST* DLParser::AssignExpr(void)
{
    AST* ret = NULL;
    ret = LogicalExpr();
    if(lookaheadType(1) == ASSIGN)
    {
        match(ASSIGN);
        ret = new AST(ASSIGN, 2, ret, LogicalExpr());
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

    if(speculate_GroupExpr())
    {
        match(LPAR);
        ret = LogicalExpr();
        match(RPAR);
    }
    else
    {
        ret = MemberExpr();
    }

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

    return ret;
}

AST* DLParser::MemberExpr(void)
{
    AST* ret = NULL;
    ret = Literal();
    if ( lookaheadType(1) == MEMB )
    {
        match(MEMB);
        ret = new AST(MEMB, 2, ret, LogicalExpr());
    }
    return ret;
}

AST* DLParser::Literal(void)
{
    AST* node = NULL;
    if(speculate_MapLiteral())
    {
        node = MapLiteral();
    }
    else
    {
        switch(lookaheadType(1))
        {
            // Literal = VectorLiteral
            case LBRACK:
                node = VectorLiteral();
                break;

            // Literal = ListLiteral
            case LPAR:
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
    }
    return node;
}

// MapLiteral = '{' (Literal ':' LogicalExpr)* '}'
AST* DLParser::MapLiteral(void)
{
    AST* ret = NULL;

    //throw Exception(lookaheadToken(1).line(),lookaheadToken(1).column());
    match(LBRACE);
    do
    {
        if( lookaheadType(1) == COMMA ) consume();

        AST* child = Literal();
        match(SEP);
        child = _new AST(SEP, 2, child, LogicalExpr());

        ret = ((ret == NULL) ? _new AST(MAP) : ret);
        ret->addChild(child);
    }
    while( lookaheadType(1) == COMMA );
    match(RBRACE);

    return ret;
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

// MacroDefinition = '%' ID '[' MacroPatternList ']'
AST* DLParser::MacroDefinition(void)
{
    AST* macro = NULL;
    AST* id = NULL;
    AST* patt_list = NULL;
    match(MACRO);

    // Get the macro's name
    id = _new AST( ID, lookaheadToken(1).text() );
    consume();

    // Parse the macro param list
    match(LBRACK);
    patt_list = MacroPatternList();
    match(RBRACK);

    // Build and register the macro
    macro = _new AST(MACRO,2,id,patt_list);
    //macros.insert( std::pair<std::string,Macro*>(id->text(), macro) );

    delete macro;

    return _new AST(MACRO);
}

// MacroPatternList = MacroPattern (',' MacroPattern)*
AST* DLParser::MacroPatternList(void)
{
    AST* ret = _new AST( LIST );
    ret->addChild( MacroPattern() );
    while(lookaheadType(1) == COMMA)
    {
        match(COMMA);
        ret->addChild( MacroPattern() );
    }
    return ret;
}

// MacroPattern = '(' ID+ ')' ':' LogicalExpr
AST* DLParser::MacroPattern(void)
{
    AST* ret = _new AST(LIST);

    match(LPAR);
    do
    {
        ret->addChild( _new AST( ID, lookaheadToken(1).text() ) );
        consume();
    }
    while( lookaheadType(1) == ID );
    match(RPAR);
    match(SEP);
    ret = _new AST(PATT, 2, ret, LogicalExpr());

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

