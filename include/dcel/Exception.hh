///////////////////////////////////////////////////////////////////////////////
//
//  Copyright Leonardo Fischer 2011 - http://coderender.blogspot.com
//
//  Distributed under the licence available in the accompanying file 
//  LICENCE.txt. Please read it before use this code.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __DCEL_EXCEPTION_H__
#define __DCEL_EXCEPTION_H__

#include <exception>
#include <string>

namespace dcel {

class Exception: public std::exception {
public:
	Exception(const char* m) :
			msg(m)
	{
	}

	Exception(const std::string& msg) :
			msg(msg)
	{
	}

	Exception(const std::exception& e) :
			msg(e.what())
	{
	}

	~Exception() throw ()
	{
	}

	Exception& operator=(const std::exception& e) throw ()
	{
		this->msg = e.what();
		return *this;
	}

	virtual const char* what() const throw ()
	{
		return msg.c_str();
	}

	virtual const std::string& message() const
	{
		return this->msg;
	}

private:
	std::string msg;
};

}

#endif /* __DCEL_EXCEPTION_H__ */
