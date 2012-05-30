#include "syntax.h"

Syntax::Syntax() : str_name(""), str_terminator("")
{
}

Syntax::~Syntax()
{
}

const std::string& Syntax::name() const
{
    return str_name;
}

void Syntax::name(std::string& name)
{
    str_name = name;
}

const std::string& Syntax::terminator() const
{
    return str_terminator;
}

void Syntax::terminator(std::string& term)
{
    str_terminator = term;
}

