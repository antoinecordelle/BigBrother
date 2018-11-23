#include "website.hpp"
#include "../Error/undocumentedErrorCodeException.hpp"

#include <iostream>
#include <unistd.h>
#include <thread>


Website::Website(std::string name, int interval, std::vector<time_t> mTimeWindows)
    :mName(name)
    ,mMetricsMap()
    ,mPinger(name)
    ,mInterval(interval)
    ,isRunning(true)
{
    for(auto ite = mTimeWindows.begin(); ite != mTimeWindows.end(); ite++)
        mMetricsMap[*ite] = Metrics(mPingList.begin());
}

void Website::run()
{
    using namespace std::chrono;
    auto timer = system_clock::now();
    isRunning = true;
    while(isRunning)
    {
        timer = system_clock::now();
        processPing(mPinger.ping());
        std::this_thread::sleep_for(mInterval - duration_cast<milliseconds>(system_clock::now() - timer));
    }
}

void Website::processPing(std::string pingResponse)
{
    try
    {
        int codeResponse = getResponseCode(pingResponse);
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
    mPingList.push_back(Ping(std::time(0), codeResponse));
    for(auto ite = mMetricsMap.begin(); ite != mMetricsMap.end(); ite++)
        (ite->second).updateMetrics(codeResponse);
}

void Website::updateMetrics(int codeResponse, double time)
{
    mPingList.push_back(Ping(std::time(0), codeResponse, time));
    for(auto ite = mMetricsMap.begin(); ite != mMetricsMap.end(); ite++)
        (ite->second).updateMetrics(codeResponse, time);
}

Data Website::getMetrics(time_t timeWindow, bool deleteOldPings)
{
    deleteOldMetrics(timeWindow, deleteOldPings);
    return mMetricsMap[timeWindow].getMetrics();
}

void Website::deleteOldMetrics(time_t timeWindow, bool deleteOldPings)
{
    time_t currentTime = time(0);
    std::cout << mPingList.size() << std::endl;
    auto oldestPing = mMetricsMap[timeWindow].getOldestPing();
    if(mMetricsMap[timeWindow].shouldInitialize())
        oldestPing = mPingList.begin();
    checkOldestPing(oldestPing, timeWindow, currentTime);
    for(auto ite = oldestPing; ite != mPingList.end(); ite++)
    {
        if(currentTime - ite->time > timeWindow)
        {
            mMetricsMap[timeWindow].deletePing(*ite);
        }
        else
        {
            oldestPing = ite;
            break;
        }
    }
    if(deleteOldPings)
        mPingList.erase(mPingList.begin(), oldestPing);
    mMetricsMap[timeWindow].updateOldMetrics(mPingList);
}


void Website::checkOldestPing(std::list<Ping>::iterator& pingIte, time_t timeWindow, time_t currentTime)
{
    std::list<Ping>::iterator iter = pingIte;
    bool changed(false);
    iter--;
    while(currentTime - iter->time < timeWindow && iter != --mPingList.begin())
    {
        changed = true;
        iter--;
        std::cout << "Wrongly sorted ping" << std::endl;
    }
    if (changed)
        pingIte = iter;
}

void Website::stopRunning()
{
    isRunning = false;
}











