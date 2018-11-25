#ifndef UNDOCUMENTEDERRORCODEEXCEPTION_H
#define UNDOCUMENTEDERRORCODEEXCEPTION_H

#include <exception>
#include <string>
#include <iostream>

/// Error when decoding the response of a ping
/**
 * @brief The ping commands should output 0 if OK, 1 or 2 if error.
 * UndocumentedErrorCodeException manages the cases where the error is unexpected/different
 */
class UndocumentedErrorCodeException : public std::exception
{
public:
    UndocumentedErrorCodeException(const int codeResponse) throw()
        :mCodeResponse(codeResponse)
    {
    }

    const char* what() const throw()
    {
        return (std::to_string(mCodeResponse)).c_str();
    }

private:
    const int mCodeResponse;
};


#endif // UNDOCUMENTEDERRORCODEEXCEPTION_H
