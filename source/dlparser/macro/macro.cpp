#include "macro.h"

Macro::Macro()
{
}

Macro::Macro(const std::list<Pattern>& patts) : patterns(patts)
{
}

Macro::~Macro()
{
}

std::list<Pattern>::iterator Macro::begin()
{
    return patterns.begin();
}

std::list<Pattern>::iterator Macro::end()
{
    return patterns.end();
}

