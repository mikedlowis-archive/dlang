#include "param.h"

Param::Param(AST* param_def)
{
	int children = param_def->children()->size();
	if( children == 0 )
	{
		param_name = param_def->text();
	}
	else if( children == 2)
	{
		param_name = param_def->children()->front()->text();
		setType( param_def->children()->front()->text() );
	}
	else
	{
		// Throw
	}
}

Param::~Param()
{
	delete param_value;
}

std::string Param::name(void)
{
	return param_name;
}

ParamType_T Param::type(void)
{
	return param_type;
}

AST* Param::value(void)
{
	return param_value;
}

void Param::setValue(AST* val)
{
	param_value = val;
}

void Param::setType( const std::string& type_string )
{
	if ( type_string.compare("Block") == 0 )
	{
		param_type = BlockTyp;
	}
	else
	{
		param_type = ExpTyp;
	}
}

