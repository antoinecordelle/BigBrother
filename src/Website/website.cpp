#include "website.hpp"

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>


using namespace std;

Website::Website(std::string name, int interval)
    :mName(name)
    ,mPinger(name)
    ,mInterval(interval)
    ,isRunning(true)
{
}

void Website::run()
{
    int i(0);
    while(isRunning)
    {
        processPing(mPinger.ping());
        this_thread::sleep_for(chrono::milliseconds(mInterval));
        i++;
        if(i == 1)
            isRunning = false;
    }
}

void Website::processPing(std::string pingResponse)
{
    bool pingSuccess = getResponseCode(pingResponse);
    if(pingSuccess)
        double time(getResponseTime(pingResponse));
}

bool Website::getResponseCode(const std::string& pingResponse)
{
    if(pingResponse.rfind("100% packet loss") == string::npos)
        return true;
    else
        return false;
}

double Website::getResponseTime(const std::string& pingResponse)
{
    size_t positionStats = pingResponse.find("mdev = ");
    size_t positionEndStats = pingResponse.find('/', positionStats);

    double time(-1.f);
    try
    {
        time = stod(pingResponse.substr(positionStats + 7, positionEndStats - positionStats - 7));
    }
    catch (const std::invalid_argument& error) {
        std::cerr << "Invalid argument: " << error.what() << '\n';
    }
    return time;
}












