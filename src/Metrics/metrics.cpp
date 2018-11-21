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
{

}

void Metrics::updateMetrics(int codeResponse, double time)
{
    mData.pingCount++;
    mData.sumTime+= time;
    if (mData.maxTime < time)
        mData.maxTime = time;
    if (mData.minTime > time)
        mData.minTime = time;
}

void Metrics::updateMetrics(int codeResponse)
{
    mData.pingCount++;
    if(codeResponse != 0)
        mData.hostUnreachableCount++;
}


Data Metrics::getMetrics()
{
    return mData;
}

void Metrics::deletePing(Ping ping)
{
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
    if(minToUpdate)
        updateMin(pingList);
    if(maxToUpdate)
        updateMax(pingList);
    if(mData.pingCount != 0)
        mData.avgTime = mData.sumTime/(double)mData.pingCount;
}

void Metrics::updateMin(const std::list<Ping>& pingList)
{
    minToUpdate = false;
    mData.minTime = pingList.front().timeDelay;
    for(auto ite = pingList.begin(); ite != pingList.end(); ite++)
    {
        if(ite->timeDelay < mData.minTime)
            mData.minTime = ite->timeDelay;
    }
}

void Metrics::updateMax(const std::list<Ping>& pingList)
{
    maxToUpdate = false;
    mData.maxTime = pingList.front().timeDelay;
    for(auto ite = pingList.begin(); ite != pingList.end(); ite++)
    {
        if(ite->timeDelay < mData.maxTime)
            mData.maxTime = ite->timeDelay;
    }
}


