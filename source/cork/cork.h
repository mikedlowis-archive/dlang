#ifndef CORK_H
#define CORK_H 

#ifdef DETECT_MEM_LEAKS
    #include <string>
    typedef unsigned int size_t;

    void Cork_ReportMemoryLeaks(void);
    void * operator new (size_t size, std::string file, unsigned int line);
    #define TBL_SIZE       512
    #define REPORT_LEAKS() Cork_ReportMemoryLeaks()
    #define _new           new (__FILE__,__LINE__)
#else 
    #define REPORT_LEAKS()
    #define _new new
#endif

#endif
