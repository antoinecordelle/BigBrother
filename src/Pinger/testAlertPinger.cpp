#include "testAlertPinger.hpp"


TestAlertPinger::TestAlertPinger(std::string )
    :Pinger("google.com")
    ,mCounter(0)
{
}

std::string TestAlertPinger::ping()
{
    if(mCounter < 100)
    {
        mCounter++;
        return "Ping test fails code error   2";
    }
    else
        return Pinger::ping();
}
