#ifndef METRICS_HPP
#define METRICS_HPP

#include <ctime>
#include <list>

struct Ping {
    std::time_t time;
    int codeResponse;
    double timeDelay;

    Ping(std::time_t aTime, int aCode, int aTimeDelay = 0.f);
};

struct Data {
    unsigned int hostUnreachableCount = 0;
    unsigned int pingCount = 0;
    double sumTime = 0.f;
    double maxTime = 0.f;
    double minTime = 1.79769e+308;
    double avgTime = 0.f;
};

class Metrics
{
public:
    Metrics(std::list<Ping>::iterator ite);

    void updateMetrics(int codeResponse, double time);
    void updateMetrics(int codeResponse);
    Data getMetrics();
    std::list<Ping>::iterator& getOldestPing();
    void deletePing(Ping ping);
    void updateOldMetrics(const std::list<Ping>& pingList);

private:
    void updateMin(const std::list<Ping>& pingList);
    void updateMax(const std::list<Ping>& pingList);

private:
    Data mData;
    bool minToUpdate;
    bool maxToUpdate;
    std::list<Ping>::iterator mOldestPing;
};

#endif // METRICS_HPP
