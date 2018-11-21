#ifndef UNDOCUMENTEDERRORCODEEXCEPTION_H
#define UNDOCUMENTEDERRORCODEEXCEPTION_H

#include <exception>
#include <string>
#include <iostream>

class UndocumentedErrorCodeException : public std::exception
{
public:
    UndocumentedErrorCodeException(const int codeResponse) throw()
        :mCodeResponse(codeResponse)
    {
    }

    const char* what() const throw()
    {
        // FIXME
        return "Unexpected Error Code from ping"; //+ std::to_string(mCodeResponse))).c_str();
    }

private:
    const int mCodeResponse;
};


#endif // UNDOCUMENTEDERRORCODEEXCEPTION_H
