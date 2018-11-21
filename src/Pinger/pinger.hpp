#ifndef PINGER_H
#define PINGER_H

#include <string>
#include <memory>

class Pinger
{
public:
    Pinger(std::string name);

    std::string ping();


private:
    const std::string mCommand;
};

#endif // PINGER_H
