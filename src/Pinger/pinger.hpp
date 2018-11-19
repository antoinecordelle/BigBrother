#ifndef PINGER_H
#define PINGER_H

#include <string>
#include <memory>

class Pinger
{
public:
    Pinger(std::string name, int interval);
    int ping();


private:
    std::string mCommand;
    int mInterval;
};

#endif // PINGER_H
