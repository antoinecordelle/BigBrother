#ifndef UNDOCUMENTEDERRORCODEEXCEPTION_H
#define UNDOCUMENTEDERRORCODEEXCEPTION_H

#include <exception>
#include <string>

class UndocumentedErrorCodeException : public std::exception
{
public:
    UndocumentedErrorCodeException(int codeResponse) throw()
        :mCodeResponse(codeResponse)
    {

    }

    virtual const char* what() const throw()
    {
        return ("Unexpected Error Code from ping : " + std::to_string(mCodeResponse)).c_str();
    }

private:
    int mCodeResponse;
};


#endif // UNDOCUMENTEDERRORCODEEXCEPTION_H
