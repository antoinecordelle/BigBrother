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
#include <memory>

#include "../Pinger/pinger.hpp"
#include "../Metrics/metrics.hpp"

/// Class used to manage the monitoring of a website
/**
 * @brief Used in order to request pings and update its metrics (add new and delete old pings)
 */
class Website
{
public:
    /// Website Constructor
    /**
     * @param Name : name of the website to monitor
     * @param interval : interval between 2 pings in ms
     * @param timeWindows : vector containing the different TimeWindows to compute metrics in
     */
    Website(std::string name, int interval = 250, std::vector<time_t> mTimeWindows = std::vector<time_t>({120, 600, 3600}));

    const std::string getName();

    /// Method run : loops until stopped to send the ping requests and update the new metrics
    void run();
    /// Method : requests to stop the website monitoring
    void stopRunning();

    /// Method getMetrics : returns the updated metrics, after old pings removal
    /**
     * @param timeWindow : the method will return the data from the past timeWindow minutes
     * @param deleteOldPings : if true, deletes the old pings from PingList
     * @return Returns the metrics data
     */
    Data getMetrics(time_t timeWindow, bool deleteOldPings = false);

private:
    /// Process ping after receiving the response from the pinger
    void processPing(std::string pingResponse);

    /// Returns the response code from pingResponse. Called by processPing
    int getResponseCode(const std::string& pingResponse) const;

    /// Returns the response time from the ping. Called by processPing
    double getResponseTime(const std::string& pingResponse) const;

    /// Update the metrics with the new ping (error)
    void updateMetrics(int codeResponse);

    /// Update the metrics with the new ping (success)
    void updateMetrics(int codeResponse, double timer);


    /// Delete the old pings from the timeWindow metrics
    /**
     * @brief Called by getMetrics. Removes the too old pings from the aggregated metric
     * An iterator from Metrics remembers the oldest ping position and deletes the pings from there
     * Allowing minimum computing
     * @param deleteOldPings : If true, timeWindow is the largest window, and we can safely delete pings from pingList
     */
    void deleteOldMetrics(time_t timeWindow, bool deleteOldPings = false);

    /// Check the rolling iterator's place
    void checkOldestPing(std::list<Ping>::iterator& pingIte, time_t timeWindow, time_t currentTime);

private:
    const std::string mName;
    std::list<Ping> mPingList;
    /// Map of metrics : a metric for each timeWindow
    std::map<time_t, std::unique_ptr<Metrics>> mMetricsMap;
    /// Interval between each ping
    const std::chrono::milliseconds mInterval;
    std::atomic<bool> isRunning;
    std::mutex mListLock;

protected:
    /// Pinger : used to send pings commands and return responses. Protected to build TestAlertPinger
    std::unique_ptr<Pinger> mPinger;
};

#endif // WEBSITE_HPP
