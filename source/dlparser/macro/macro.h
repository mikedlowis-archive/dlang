#ifndef MACRO_H
#define MACRO_H

#include <string>
#include <list>
#include "param.h"
#include "ast.h"

class Macro
{
	private:
		std::string macro_name;
		std::list<Param*> macro_params;
		AST* macro_body;

		void setUpParamList( AST* param_tree );
	public:
		Macro(AST* macro_def);
		~Macro();

		const std::string& name(void);
		const std::list<Param*>& params(void);
		AST* apply(void);
};

#endif
