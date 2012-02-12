#ifndef DLPARSER_H
#define DLPARSER_H 

#include <map>
#include "btparser.h"
#include "dllexer.h"
#include "macro.h"

class DLParser : public BTParser
{
	private:
		AST* ast;
		std::map<std::string,Macro*> macros;
	public:
		DLParser();
		void parse(void);
		AST* getAST(void);
		bool isMacro(Token* token);
		AST* parseMacroParam(Param* param);
		
		/**********************************************************************
		 * EBNF Syntax Grammar
		 *********************************************************************/
		// Program    = Expression*
		//
		// Expression = ID '=' LogicalExpr
		//            | MacroDefinition
		//            | $MacroExpansion$
		//            | LogicalExpr
		//
		// LogicalExpr = CompExpr (('&&' | '||') CompExpr)*
		//
		// CompExpr = AddSubExpr (('==' | '!=' | '<' | '>' | '<=' | '>=') AddSubExpr)*
		//
		// AddSubExpr = MulDivExpr (('+' | '-') MulDivExpr)*
		//
		// MulDivExpr = UnaryExpr (('*' | '/') UnaryExpr)*
		//
		// UnaryExpr = '!' GroupExpr
		//           | GroupExpr
		//            
		// GroupExpr = '(' LogicalExpr ')'
		//           | Literal '(' ExpList ')'
		//           | Literal
		//
        // Literal    = VectorLiteral
		//            | ListLiteral
		//            | FuncLiteral
		//            | ID
		//            | NUM
		//            | CHAR
		//            | STRING
		//            | SYMBOL
		//
		// VectorLiteral =  '[' ExpList ']'
		//
		// ListLiteral = '`' '(' ExpList ')'
		//
		// FuncLiteral = '{' ExpBlock '}'
		//             |  '{' '|' ExpList '|' ExpBlock '}'
		//
		// MacroDefinition = '%' ID '(' MacroParamList ')' '{' Expression '}'
		//
		// MacroParamList = MacroParam (',' MacroParam)* 
		//
		// MacroParam = ID (':' ID)?
		//
		// ExpList    = (GroupExpr (',' GroupExpr)*)?
		//
		// ExpBlock   = Expression*
	private:
		AST* Program(void);
		AST* Expression(void);
		AST* LogicalExpr(void);
		AST* CompExpr(void);
		AST* AddSubExpr(void);
		AST* MulDivExpr(void);
		AST* UnaryExpr(void);
		AST* GroupExpr(void);
		AST* Literal(void);
		AST* VectorLiteral(void);
		AST* ListLiteral(void);
		AST* FuncLiteral(void);
		AST* MacroDefinition(void);
		AST* MacroExpansion(void);
		AST* MacroParamList(void);
		AST* MacroParam(void);
		AST* ExpList(TokenType_T node_type, TokenType_T terminator);
		AST* ExpBlock(TokenType_T node_type, TokenType_T terminator);
};

#endif
