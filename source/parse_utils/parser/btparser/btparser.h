#ifndef BT_PARSER_H
#define BT_PARSER_H

#include <exception>
#include <vector>
#include "parser.h"
#include "lexer.h"
#include "ast.h"

class BTParser : public Parser
{
	private:
		Lexer* lexer;
		unsigned int current;
		std::vector<unsigned int> markers;
		std::vector<Token*> lookahead;
	public:
		BTParser(Lexer* lxer);
		~BTParser();

		void setInput(char* in);
		void setInput(string& in);
		void setInput(istream* in);

		void consume(void);
		void sync(unsigned int i);
		void fill(unsigned int n);
		void match(TokenType_T type);
		Token* lookaheadToken(unsigned int i);
		TokenType_T lookaheadType(unsigned int i);
		unsigned int mark(void);
		void release(void);
		void seek(unsigned int index);
		bool isSpeculating(void);

		virtual void parse(void) = 0;
};

#endif
