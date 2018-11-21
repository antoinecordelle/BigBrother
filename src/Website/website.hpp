#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include <string>
#include <ctime>

#include "../Pinger/pinger.hpp"

struct Ping {
    std::time_t time;
    int codeResponse;
    int timeStamp;
};

struct Metrics {
    unsigned int hostUnreachableCount = 0;
    unsigned int pingCount = 0;
    double maxTime = 0;
    double minTime = 1.79769e+308;
    double sumTime = 0;
    double avgTime = 0;

    time_t timeWindow;
};

class Website
{
public:
    Website(std::string name, int interval = 250);

    void run();
    Metrics getMetrics();

private:
    void processPing(std::string pingResponse);
    int getResponseCode(const std::string& pingResponse) const;
    double getResponseTime(const std::string& pingResponse) const;
    void updateMetrics(int codeResponse);
    void updateMetrics(int codeResponse, double time);

private:
    const std::string mName;
    Metrics mMetrics;
    Pinger mPinger;
    const int mInterval;
    bool isRunning;

};

#endif // WEBSITE_HPP
