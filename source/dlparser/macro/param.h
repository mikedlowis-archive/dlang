#ifndef PARAM
#define PARAM

#include <string>
#include "ast.h"

typedef enum {
	ExpTyp,
	BlockTyp
} ParamType_T;

class Param
{
	private:
		std::string param_name;
		ParamType_T param_type;
		AST* param_value;

		void setType( const std::string& type_string );

	public:
		Param(AST* param_def);
		~Param();

		std::string name(void);
		ParamType_T type(void);
		AST* value(void);
		void setValue(AST* val);
};

#endif
