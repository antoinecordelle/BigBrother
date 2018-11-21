#include "website.hpp"
#include "../Error/undocumentedErrorCodeException.hpp"

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
    isRunning = true;
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
    try
    {
        int codeResponse= getResponseCode(pingResponse);
        if(codeResponse == 0)
        {
            double time(getResponseTime(pingResponse));
            updateMetrics(codeResponse, time);
        }
        else
            updateMetrics(codeResponse);
    } catch (const std::invalid_argument& error)
    {
        std::cerr << "Dropping ping : Invalid argument: " << error.what() << '\n';
    } catch (const UndocumentedErrorCodeException& error)
    {
        std::cerr << "Dropping ping : " << error.what() << '\n';
    }
}

int Website::getResponseCode(const std::string& pingResponse) const
{
    int codeResponse = stoi(pingResponse.substr(pingResponse.size()-3));
    if(codeResponse != 0 && codeResponse != 1 && codeResponse != 2)
        throw UndocumentedErrorCodeException(codeResponse);
    return codeResponse;
}

double Website::getResponseTime(const std::string& pingResponse) const
{
    size_t positionStats = pingResponse.find("mdev = ");
    size_t positionEndStats = pingResponse.find('/', positionStats);

    double time = stod(pingResponse.substr(positionStats + 7, positionEndStats - positionStats - 7));
    return time;
}

void Website::updateMetrics(int codeResponse)
{
    mMetrics.pingCount++;
    mMetrics.hostUnreachableCount++;
}

void Website::updateMetrics(int codeResponse, double time)
{
    mMetrics.pingCount++;
}















