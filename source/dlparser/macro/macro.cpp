#include "macro.h"

Macro::Macro() : str_name(""), str_terminator("")
{
}

Macro::~Macro()
{
}

const std::string& Macro::name() const
{
    return str_name;
}

void Macro::name(std::string& name)
{
    str_name = name;
}

const std::string& Macro::terminator() const
{
    return str_terminator;
}

void Macro::terminator(std::string& term)
{
    str_terminator = term;
}

