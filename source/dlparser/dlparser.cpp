#include "dlparser.h"
#include "exception.h"
#include "cork.h"

DLParser::DLParser() : BTParser(_new DLLexer())
{
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Map", MAP_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Vector", VECT_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "List", LIST_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Block", BLK_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Id", ID_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Num", NUM_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Char", CHAR_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "String", STR_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Symbol", SYM_TYP ));
    pattern_types.insert( std::pair<std::string,PatternType_T>( "Expression", EXPR_TYP ));
}

DLParser::~DLParser()
{
}

AST* DLParser::parse(void)
{
    return Program();
}

bool DLParser::isMacro( Token& token )
{
    bool ret = false;
    if( (token.type() == ID)
        && (macros.find(token.text()) != macros.end()) )
    {
        ret = true;
    }
    return ret;
}

AST* DLParser::MacroExpansion()
{
    AST* ret = NULL;
    Macro macro = macros[ lookaheadToken(1).text() ];
    std::list<Pattern>::iterator patt_it;

    for(patt_it = macro.begin(); patt_it != macro.end(); patt_it++)
    {
        if( speculate_MacroPatternMatch(*patt_it) )
        {
            ret = MacroPatternMatch(*patt_it);
            break;
        }
    }

    if (ret == NULL)
    {
        throw "Did not find a matching pattern for keyword <macro-name>";
    }

    return ret;
}

bool DLParser::speculate_MacroPatternMatch(Pattern patt)
{
    bool success = true;

    mark();
    try
    {
        delete MacroPatternMatch(patt);
    }
    catch (Exception e)
    {
        success = false;
    }
    release();

    return success;

}

AST* DLParser::MacroPatternMatch(Pattern patt)
{
    std::vector<AST*> params;
    std::list<PatternType_T>::iterator patt_it;
    for(patt_it = patt.begin(); patt_it != patt.end(); patt_it++)
    {
        AST* param = NULL;
        string text = lookaheadToken(1).text();
        switch( *patt_it )
        {
            case MAP_TYP:
                param = MapLiteral();
                break;

            case VECT_TYP:
                param = VectorLiteral();
                break;

            case LIST_TYP:
                param = ListLiteral();
                break;

            case BLK_TYP:
                param = FuncLiteral();
                break;

            case ID_TYP:
                match(ID);
                param = _new AST(ID,text);
                break;

            case NUM_TYP:
                match(NUM);
                param = _new AST(NUM,text);
                break;

            case CHAR_TYP:
                match(CHAR);
                param = _new AST(CHAR,text);
                break;

            case STR_TYP:
                match(STRING);
                param = _new AST(STRING,text);
                break;

            case SYM_TYP:
                match(SYMBOL);
                param = _new AST(SYMBOL,text);
                break;

            case EXPR_TYP:
                param = LogicalExpr();
                break;

            default:
                throw Exception(lookaheadToken(1).line(), lookaheadToken(1).column());
                break;
        }
        params.push_back(param);
    }

    return patt.accept( params );
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
    else
    {
        ret = AssignExpr();
    }
    return ret;
}

AST* DLParser::AssignExpr(void)
{
    AST* ret = NULL;
    if( isMacro( lookaheadToken(1) ) )
    {
        ret = MacroExpansion();
    }
    else
    {
        ret = LogicalExpr();
        if(lookaheadType(1) == ASSIGN)
        {
            match(ASSIGN);
            ret = new AST(ASSIGN, 2, ret, LogicalExpr());
        }
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
    std::list<Pattern> patterns;
    std::string name;

    match(MACRO);

    // Get the macro's name
    name = lookaheadToken(1).text();
    match(ID);

    // Parse the macro param list
    match(LBRACK);
    patterns = MacroPatternList();
    match(RBRACK);

    // Build and register the macro
    macros.insert( std::pair<std::string,Macro>(name, Macro(patterns)) );

    return _new AST(MACRO);
}

// MacroPatternList = MacroPattern (',' MacroPattern)*
std::list<Pattern> DLParser::MacroPatternList(void)
{
    std::list<Pattern> patterns;

    patterns.push_back( MacroPattern() );
    while(lookaheadType(1) == COMMA)
    {
        match(COMMA);
        patterns.push_back( MacroPattern() );
    }

    return patterns;
}

// MacroPattern = '(' ID+ ')' ':' LogicalExpr
Pattern DLParser::MacroPattern(void)
{
    AST* expr = NULL;
    std::list<PatternType_T> pattern;

    match(LPAR);
    do
    {
        std::string text = lookaheadToken(1).text();
        match(ID);
        //pattern.push_back( str_to_patt_type(text) );
        if( pattern_types.find(text) != pattern_types.end() )
        {
            pattern.push_back( pattern_types[ text ] );
        }
        else
        {
            throw Exception(lookaheadToken(1).line(), lookaheadToken(1).column());
        }
    }
    while( lookaheadType(1) == ID );
    match(RPAR);
    match(SEP);
    expr = LogicalExpr();

    return Pattern( pattern, expr );
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

