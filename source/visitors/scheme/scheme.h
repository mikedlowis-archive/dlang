#ifndef Scheme_H
#define Scheme_H 

#include <iostream>
#include <sstream>
#include "visitor.h"
#include "dllexer.h"

class Scheme : public Visitor {
	protected:
		ostringstream stream;
	public:
		Scheme(AST* root);
		string str();
		string typeToString(ASTNodeType type);
		bool isDatatype(ASTNodeType type);
		void printDatatype(AST* cur);
		void charToString(string ch);
	private:
		void beforeVisit(AST* cur, int depth);
		void afterVisit(AST* cur, int depth);
		void beforeChildren(AST* cur, int depth);
		void afterChildren(AST* cur, int depth);
		void beforeChild(AST* cur, int depth);
		void afterChild(AST* cur, int depth);
};

#endif
