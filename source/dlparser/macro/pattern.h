#ifndef PATTERN_H
#define PATTERN_H

#include <list>
#include "ast.h"

typedef enum {
    MAP_TYP,
    VECT_TYP,
    LIST_TYP,
    BLK_TYP,
    ID_TYP,
    NUM_TYP,
    CHAR_TYP,
    STR_TYP,
    SYM_TYP,
    EXPR_TYP
} PatternType_T;

class Pattern {
    private:
        std::list<PatternType_T> pattern;
        const AST* expr_ast;
    public:
        Pattern(const std::list<PatternType_T>& patt, const AST* ast);
        ~Pattern();
        std::list<PatternType_T>::iterator begin();
        std::list<PatternType_T>::iterator end();
};

#endif
