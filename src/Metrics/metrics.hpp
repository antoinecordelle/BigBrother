#ifndef METRICS_HPP
#define METRICS_HPP

#include <ctime>
#include <list>
#include <mutex>
#include <atomic>

struct Ping {
    std::time_t time;
    int codeResponse;
    double timeDelay;

    Ping(std::time_t aTime, int aCode, double aTimeDelay = 0.f);
};

struct Data {
    unsigned int hostUnreachableCount = 0;
    unsigned int pingCount = 0;
    double sumTime = 0.f;
    double maxTime = 0.f;
    double minTime = 1.79769e+308;
    double avgTime = 0.f;
    double availibility = 1.f;
    std::string name;
};

class Metrics
{
public:
    Metrics();
    Metrics(std::list<Ping>::iterator ite, std::string name);

    void updateMetrics(int codeResponse, double time);
    void updateMetrics(int codeResponse);
    Data getMetrics();
    std::list<Ping>::iterator getOldestPing();
    void setOldestPing(std::list<Ping>::iterator newOldestPing);
    void removePing(Ping ping);
    void updateOldMetrics(const std::list<Ping>& pingList);
    bool shouldInitialize();
    void setInitialized();

private:
    void updateMin(const std::list<Ping>& pingList);
    void updateMax(const std::list<Ping>& pingList);

private:
    Data mData;
    bool minToUpdate;
    bool maxToUpdate;
    bool initialized;
    std::list<Ping>::iterator mOldestPing;
    std::mutex mMetricsLock;
};

#endif // METRICS_HPP
