#ifndef EXCEPTION_H
#define EXCEPTION_H 

#include <exception>
#include <string>

class Exception : public std::exception
{
	protected:
		int ex_line;
		int ex_column;
		std::string ex_msg;
	public:
		Exception(int line, int column) throw();
		virtual ~Exception() throw() {};
		virtual const char* what() const throw();
		void setMessage(std::string msg) throw();
		std::string& message(void) throw();
};

#endif
