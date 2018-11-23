#include "metrics.hpp"


Ping::Ping(std::time_t aTime, int aCode, int aTimeDelay)
    :time(aTime)
    ,codeResponse(aCode)
    ,timeDelay(aTimeDelay)
{

}

Metrics::Metrics()
    :minToUpdate(false)
    ,maxToUpdate(false)
    ,initialized(false)
{

}

Metrics::Metrics(std::list<Ping>::iterator ite)
    :minToUpdate(false)
    ,maxToUpdate(false)
    ,mOldestPing(ite)
    ,initialized(false)
{

}

void Metrics::updateMetrics(int codeResponse, double time)
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    mData.pingCount++;
    mData.sumTime+= time;
    if (mData.maxTime < time)
        mData.maxTime = time;
    if (mData.minTime > time)
        mData.minTime = time;
}

void Metrics::updateMetrics(int codeResponse)
{   
    std::lock_guard<std::mutex> lock(mMetricsLock);
    mData.pingCount++;
    if(codeResponse != 0)
        mData.hostUnreachableCount++;
}


Data Metrics::getMetrics()
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    return mData;
}

std::list<Ping>::iterator Metrics::getOldestPing()
{
    return mOldestPing;
}

void Metrics::setOldestPing(std::list<Ping>::iterator newOldestPing)
{
    mOldestPing = newOldestPing;
}


void Metrics::deletePing(Ping ping)
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    mData.pingCount--;
    if(ping.codeResponse != 0)
        mData.hostUnreachableCount--;
    else
    {
        mData.sumTime -= ping.timeDelay;
        if(mData.maxTime == ping.timeDelay)
            maxToUpdate = true;
        else if (mData.minTime == ping.timeDelay)
            minToUpdate = true;
    }
}

void Metrics::updateOldMetrics(const std::list<Ping>& pingList)
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    if(minToUpdate)
        updateMin(pingList);
    if(maxToUpdate)
        updateMax(pingList);
    if(mData.pingCount != 0)
        mData.avgTime = mData.sumTime/(double)mData.pingCount;
}

void Metrics::updateMin(const std::list<Ping>& pingList)
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    minToUpdate = false;
    mData.minTime = pingList.front().timeDelay;
    for(auto ite = mOldestPing; ite != pingList.end(); ite++)
    {
        if(ite->timeDelay < mData.minTime)
            mData.minTime = ite->timeDelay;
    }
}

void Metrics::updateMax(const std::list<Ping>& pingList)
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    maxToUpdate = false;
    mData.maxTime = pingList.front().timeDelay;
    for(auto ite = mOldestPing; ite != pingList.end(); ite++)
    {
        if(ite->timeDelay < mData.maxTime)
            mData.maxTime = ite->timeDelay;
    }
}

bool Metrics::shouldInitialize()
{
    return initialized;
}

void Metrics::setInitialized()
{
    std::lock_guard<std::mutex> lock(mMetricsLock);
    initialized = true;
}

