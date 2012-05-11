#ifndef MACRO_H
#define MACRO_H

#include <string>

class Macro {
    private:
        std::string str_name;
        std::string str_terminator;
    public:
        Macro();
        ~Macro();
        const std::string& name() const;
        void name(std::string& name);
        //const std::string& keywords() const;
        //void keywords(std::string& nm);
        const std::string& terminator() const;
        void terminator(std::string& term);
        //const std::string& transforms() const;
        //void transforms(std::string& nm);
};

#endif
