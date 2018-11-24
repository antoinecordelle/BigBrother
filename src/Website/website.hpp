#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include <string>
#include <ctime>
#include <list>
#include <map>
#include <vector>
#include <chrono>
#include <mutex>
#include <atomic>

#include "../Pinger/pinger.hpp"
#include "../Metrics/metrics.hpp"


class Website
{
public:
    Website(std::string name, int interval = 250, std::vector<time_t> mTimeWindows = std::vector<time_t>({120, 600, 3600}));

    void run();
    Data getMetrics(time_t timeWindow, bool deleteOldPings = false);
    void stopRunning();
    const std::string getName();

private:
    void processPing(std::string pingResponse);
    int getResponseCode(const std::string& pingResponse) const;
    double getResponseTime(const std::string& pingResponse) const;
    void updateMetrics(int codeResponse);
    void updateMetrics(int codeResponse, double timer);
    void deleteOldMetrics(time_t timeWindow, bool deleteOldPings = false);
    void checkOldestPing(std::list<Ping>::iterator& pingIte, time_t timeWindow, time_t currentTime);

private:
    const std::string mName;
    std::list<Ping> mPingList;
    std::map<time_t, std::unique_ptr<Metrics>> mMetricsMap;
    Pinger mPinger;
    const std::chrono::milliseconds mInterval;
    std::atomic<bool> isRunning;
    std::mutex mListLock;

};

#endif // WEBSITE_HPP
