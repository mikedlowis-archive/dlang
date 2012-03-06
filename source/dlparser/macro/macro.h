#ifndef MACRO_H
#define MACRO_H

#include <list>
#include "pattern.h"

class Macro {
    private:
        std::list<Pattern> patterns;
    public:
        Macro(const std::list<Pattern>& patts);
        ~Macro();
};

#endif
