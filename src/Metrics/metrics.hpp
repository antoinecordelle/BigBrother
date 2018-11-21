#ifndef METRICS_HPP
#define METRICS_HPP

#include <ctime>

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
    Metrics(std::time_t time);

    void updateMetrics(int codeResponse, double time);
    void updateMetrics(int codeResponse);
    Data getMetrics();

private:
    Data mData;
    time_t timeWindow;
};

#endif // METRICS_HPP
