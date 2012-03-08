#ifndef PATTERN_H
#define PATTERN_H

#include <list>
#include <vector>
#include "ast.h"

typedef enum {
    MAP_TYP  = 0,
    VECT_TYP = 1,
    LIST_TYP = 2,
    BLK_TYP  = 3,
    ID_TYP   = 4,
    NUM_TYP  = 5,
    CHAR_TYP = 6,
    STR_TYP  = 7,
    SYM_TYP  = 8,
    EXPR_TYP = 9
} PatternType_T;

class Pattern {
    protected:
        std::list<PatternType_T> pattern;
        const AST* expr_ast;
    private:
        void apply(AST* cur,std::vector<AST*>& params);
    public:
        Pattern(const std::list<PatternType_T>& patt, const AST* ast);
        ~Pattern();
        std::list<PatternType_T>::iterator begin();
        std::list<PatternType_T>::iterator end();
        AST* accept(std::vector<AST*>& params);
};

#endif
