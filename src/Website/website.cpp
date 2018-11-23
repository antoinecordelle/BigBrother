#include "website.hpp"
#include "../Error/undocumentedErrorCodeException.hpp"

#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

Website::Website(string name, int interval, vector<time_t> mTimeWindows)
    :mName(name)
    ,mMetricsMap()
    ,mPinger(name)
    ,mInterval(interval)
    ,isRunning(true)
{
    for(auto ite = mTimeWindows.begin(); ite != mTimeWindows.end(); ite++)
        mMetricsMap[*ite] = unique_ptr<Metrics>(new Metrics(mPingList.begin()));
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
    } catch (const invalid_argument& error)
    {
        cerr << "Dropping ping : Invalid argument: " << error.what() << '\n';
    } catch (const UndocumentedErrorCodeException& error)
    {
        cerr << "Dropping ping : " << error.what() << '\n';
    }
}

int Website::getResponseCode(const string& pingResponse) const
{
    int codeResponse = stoi(pingResponse.substr(pingResponse.size()-3));
    if(codeResponse != 0 && codeResponse != 1 && codeResponse != 2)
        throw UndocumentedErrorCodeException(codeResponse);
    return codeResponse;
}

double Website::getResponseTime(const string& pingResponse) const
{
    size_t positionStats = pingResponse.find("mdev = ");
    size_t positionEndStats = pingResponse.find('/', positionStats);
    double time = stod(pingResponse.substr(positionStats + 7, positionEndStats - positionStats - 7));
    return time;
}

void Website::updateMetrics(int codeResponse)
{
    lock_guard<mutex> lock(mListLock);
    mPingList.push_back(Ping(time(0), codeResponse));
    for(auto ite = mMetricsMap.begin(); ite != mMetricsMap.end(); ite++)
        (ite->second)->updateMetrics(codeResponse);
}

void Website::updateMetrics(int codeResponse, double timer)
{
    lock_guard<mutex> lock(mListLock);
    mPingList.push_back(Ping(time(0), codeResponse, timer));
    for(auto ite = mMetricsMap.begin(); ite != mMetricsMap.end(); ite++)
        (ite->second)->updateMetrics(codeResponse, timer);
}

Data Website::getMetrics(time_t timeWindow, bool deleteOldPings)
{
    deleteOldMetrics(timeWindow, deleteOldPings);
    return mMetricsMap[timeWindow]->getMetrics();
}

void Website::deleteOldMetrics(time_t timeWindow, bool deleteOldPings)
{
    time_t currentTime = time(0);
    lock_guard<mutex> lock(mListLock);
    if(!mMetricsMap[timeWindow]->shouldInitialize())
    {
        mMetricsMap[timeWindow]->setInitialized();
        mMetricsMap[timeWindow]->setOldestPing(mPingList.begin());
    }
    auto oldestPing = mMetricsMap[timeWindow]->getOldestPing();
    checkOldestPing(oldestPing, timeWindow, currentTime);
    for(auto ite = oldestPing; ite != mPingList.end(); ite++)
    {
        if(currentTime - ite->time > timeWindow)
        {
            mMetricsMap[timeWindow]->removePing(*ite);
        }
        else
        {
            oldestPing = ite;
            mMetricsMap[timeWindow]->setOldestPing(ite);
            break;
        }
    }
    if(deleteOldPings)
    {
        mPingList.erase(mPingList.begin(), oldestPing);
    }
    mMetricsMap[timeWindow]->updateOldMetrics(mPingList);
}


void Website::checkOldestPing(list<Ping>::iterator& pingIte, time_t timeWindow, time_t currentTime)
{
    list<Ping>::iterator iter = pingIte;
    bool changed(false);
    iter--;
    while(currentTime - iter->time < timeWindow && iter != --mPingList.begin())
    {
        changed = true;
        iter--;
    }
    if (changed)
        pingIte = iter;
}

void Website::stopRunning()
{
    isRunning = false;
}











