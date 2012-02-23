#include <sstream>
#include "exception.h"

Exception::Exception(int line, int column) throw() : std::exception(), ex_line(line), ex_column(column)
{
}

const char* Exception::what() const throw()
{
    std::ostringstream oss;
    oss << "(ln " << ex_line << ", col " << ex_column << "): ";
    oss << ((Exception*)this)->message() << std::endl;
    return oss.str().c_str();
}

void Exception::setMessage(std::string msg) throw()
{
    ex_msg = msg;
}

std::string& Exception::message(void) throw()
{
    return ex_msg;
}

