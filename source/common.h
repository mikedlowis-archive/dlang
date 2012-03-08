#ifndef COMMON_H
#define COMMON_H

#ifdef DEBUG
    #include "cork.h"
#else
    #define _new new
#endif

#endif
