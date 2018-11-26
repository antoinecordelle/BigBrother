#include "website.hpp"
#include "../Error/undocumentedErrorCodeException.hpp"

#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

Website::Website(string name, int interval, vector<time_t> mTimeWindows)
    :mName(name)
    ,mMetricsMap()
    ,mPinger(new Pinger(name))
    ,mInterval(interval)
    ,isRunning(true)
{
    // Initializing the metrics by timeWindow, initializing the rolling iterators through pingList (see Metrics/metrics.hpp)
    for(auto ite = mTimeWindows.begin(); ite != mTimeWindows.end(); ite++)
        mMetricsMap[*ite] = unique_ptr<Metrics>(new Metrics(mPingList.begin(), name));
}

const std::string Website::getName()
{
    return mName;
}


void Website::run()
{
    // Sending and processing pings every mInterval ms
    using namespace std::chrono;
    auto timer = system_clock::now();
    while(isRunning)
    {
        timer = system_clock::now();
        processPing(mPinger->ping());
        std::this_thread::sleep_for(mInterval - duration_cast<milliseconds>(system_clock::now() - timer));
    }
}

void Website::processPing(std::string pingResponse)
{
    try
    {
        if(pingResponse.size() > 0)
        {
            // Code response 0 : successful ping
            int codeResponse = getResponseCode(pingResponse);
            if(codeResponse == 0)
            {
                double time(getResponseTime(pingResponse));
                updateMetrics(codeResponse, time);
            }
            // Code response 1 or 2 : ping error
            else
                updateMetrics(codeResponse);
        }
    } catch (const invalid_argument& error)
    {
        cerr << "Dropping ping : Invalid argument: " << error.what() << '\n';
    } catch (const UndocumentedErrorCodeException& error)
    {
        // Code response different : unexpected response
        cerr << "Dropping ping : Unexpected Error Code : " << error.what() << '\n';
    }
}

int Website::getResponseCode(const string& pingResponse) const
{
    // PingResponse ends by the code response : "echo $?" in the ping command

    int codeResponse = stoi(pingResponse.substr(pingResponse.size() - 2));
    if(codeResponse != 0 && codeResponse != 1 && codeResponse != 2 && codeResponse != 68)
        throw UndocumentedErrorCodeException(codeResponse);
    return codeResponse;
}

double Website::getResponseTime(const string& pingResponse) const
{
    // Linux response
    int pos(7);
    size_t positionStats = pingResponse.find("mdev = ");
    if(positionStats == string::npos)
    {
	// MacOS response
        positionStats = pingResponse.find("stddev = ");
        pos = 9;
    }
    size_t positionEndStats = pingResponse.find('/', positionStats);
    double time = stod(pingResponse.substr(positionStats + pos, positionEndStats - positionStats - pos));
    return time;
}

void Website::updateMetrics(int codeResponse)
{
    // Sends metric update : ping with error
    lock_guard<mutex> lock(mListLock);
    mPingList.push_back(Ping(time(0), codeResponse));
    for(auto ite = mMetricsMap.begin(); ite != mMetricsMap.end(); ite++)
        (ite->second)->updateMetrics(codeResponse);
}

void Website::updateMetrics(int codeResponse, double timer)
{
    // Sends metric update : successful ping
    cout << codeResponse << "  " << timer << endl;
    lock_guard<mutex> lock(mListLock);
    mPingList.push_back(Ping(time(0), codeResponse, timer));
    for(auto ite = mMetricsMap.begin(); ite != mMetricsMap.end(); ite++)
        (ite->second)->updateMetrics(codeResponse, timer);
}

Data Website::getMetrics(time_t timeWindow, bool deleteOldPings)
{
    // Delete the old pings and returns the associated metrics. See deleteOldMetrics
    lock_guard<mutex> lock(mListLock);
    deleteOldMetrics(timeWindow, deleteOldPings);
    return mMetricsMap[timeWindow]->getMetrics();
}

void Website::deleteOldMetrics(time_t timeWindow, bool deleteOldPings)
{
    // Metrics keeps in memory the position of the last ping from the last getMetrics in pingList.
    // If this one is too old, this is the oldest to remove from the metric
    // => we only have to iterate through a small and constant number of pings instead of the all list
    time_t currentTime = time(0);
    if(!mMetricsMap[timeWindow]->shouldInitialize())
    {
        // The rolling iterator needs a safe initialization as iterator in an empty list are set to list.end()
        if(mPingList.size() != 0)
            mMetricsMap[timeWindow]->setInitialized();
        mMetricsMap[timeWindow]->setOldestPing(mPingList.begin());
    }
    auto oldestPing = mMetricsMap[timeWindow]->getOldestPing();
    checkOldestPing(oldestPing, timeWindow, currentTime);
    // Iterate from the last check oldestPing to the oldestPing that is recent enough for this time window
    for(auto ite = oldestPing; ite != mPingList.end(); ite++)
    {
        // Check if the ping is too old. If it is, remove it, else, we are up to date
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











