#include "metrics.hpp"


Ping::Ping(std::time_t aTime, int aCode, int aTimeDelay)
    :time(aTime)
    ,codeResponse(aCode)
    ,timeDelay(aTimeDelay)
{

}


Metrics::Metrics(time_t time)
    :timeWindow(time)
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
    mData.avgTime = mData.sumTime/(double)mData.pingCount;
    return mData;
}




