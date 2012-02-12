#include "MacroApplication.h"
#include "dllexer.h"

using namespace std;

Param* MacroApplication::getParamByName(std::string name)
{
	Param* ret = NULL;
	std::list<Param*>::iterator it = macro_params.begin();
	for(; it != macro_params.end(); it++)
	{
		if( (*it)->name().compare( name ) == 0)
		{
			ret = *it;
			break;
		}
	}
	return ret;
}

AST* MacroApplication::getAST(void)
{
	return mod_ast;
}

void MacroApplication::beforeVisit(AST* cur, int depth)
{
}

void MacroApplication::afterVisit(AST* cur, int depth)
{
}

void MacroApplication::beforeChildren(AST* cur, int depth)
{
	if(cur->type() == ID)
	{
		Param* param = getParamByName( cur->text() );
		if( param != NULL )
		{
			(*cur) = *(param->value());
		}
	}
}

void MacroApplication::afterChildren(AST* cur, int depth)
{
}

void MacroApplication::beforeChild(AST* cur, int depth)
{
}

void MacroApplication::afterChild(AST* cur, int depth)
{
}

