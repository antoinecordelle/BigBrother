#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include <string>
#include <ctime>
#include <list>

#include "../Pinger/pinger.hpp"
#include "../Metrics/metrics.hpp"


class Website
{
public:
    Website(std::string name, int interval = 250);

    void run();
    Data getMetrics(time_t timeWindow, bool deleteOldPings = false);

private:
    void processPing(std::string pingResponse);
    int getResponseCode(const std::string& pingResponse) const;
    double getResponseTime(const std::string& pingResponse) const;
    void updateMetrics(int codeResponse);
    void updateMetrics(int codeResponse, double time);
    void deleteOldMetrics(time_t timeWindow, bool deleteOldPings = false);
    void checkOldestPing(std::list<Ping>::iterator& pingIte, time_t timeWindow, time_t currentTime);

private:
    const std::string mName;
    std::list<Ping> mPingList;
    Metrics mMetrics;
    Pinger mPinger;
    const int mInterval;
    bool isRunning;

};

#endif // WEBSITE_HPP
