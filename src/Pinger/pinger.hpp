#ifndef PINGER_H
#define PINGER_H

#include <string>
#include <memory>

class Pinger
{
public:
    Pinger(std::string name);

    virtual std::string ping();


private:
    std::string mCommand;
};

#endif // PINGER_H
