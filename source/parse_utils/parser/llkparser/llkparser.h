#ifndef LLK_PARSER_H
#define LLK_PARSER_H

#include <exception>
#include "parser.h"
#include "lexer.h"
#include "ast.h"

class LLKParser : public Parser
{
	private:
		int k;
		int next;
		Lexer*  lexer;
		Token** lookahead;
	public:
		LLKParser(int k_val, Lexer* lxer);
		~LLKParser();

		void setInput(char* in);
		void setInput(string& in);
		void setInput(istream* in);

		void         consume(void);
		void         match(TokenType_T type);
		Token*       lookaheadToken(int i);
		TokenType_T  lookaheadType(int i);
		virtual void parse(void) = 0;
};

#endif
