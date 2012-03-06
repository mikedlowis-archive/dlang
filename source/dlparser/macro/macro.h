#ifndef MACRO_H
#define MACRO_H

#include <list>
#include "pattern.h"

class Macro {
    private:
        std::list<Pattern> patterns;
    public:
        Macro();
        Macro(const std::list<Pattern>& patts);
        ~Macro();
        std::list<Pattern>::iterator begin();
        std::list<Pattern>::iterator end();
};

#endif
