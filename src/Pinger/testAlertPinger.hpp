#ifndef TESTALERTPINGER_HPP
#define TESTALERTPINGER_HPP

#include "pinger.hpp"

class TestAlertPinger : public Pinger
{
public:
    TestAlertPinger(std::string name);

    virtual std::string ping();

private:
    int mCounter;
};

#endif // TESTALERTPINGER_HPP
