#ifndef METRICS_HPP
#define METRICS_HPP

#include <ctime>
#include <list>
#include <mutex>
#include <atomic>
#include <string>

/// Struct representing a ping. Used to store pings in PingList
struct Ping {
    std::time_t time;
    int codeResponse;
    double timeDelay;

    Ping(std::time_t aTime, int aCode, double aTimeDelay = 0.f);
};

/// Struct used as the data sent to the Application and the dashboard. Contains the metrics necessary
struct Data {
    unsigned int hostUnreachableCount = 0;
    unsigned int pingCount = 0;
    double sumTime = 0.f;
    double maxTime = 0.f;
    double minTime = 1.79769e+308;
    double avgTime = 0.f;
    double availability = 1.f;
    std::string name;
};

/// Class used to store and compute metrics related to a website and a timeWindow
class Metrics
{
public:
    Metrics();
    /// Constructor : intialize the metrics
    /**
     * @param pingIte : Initialize the iterator that will mark the oldestPing taken into account in the metrics.
     */
    Metrics(std::list<Ping>::iterator pingIte, std::string name);

    /// Metrics getter
    Data getMetrics();

    /// Update the metrics with the new ping (success)
    void updateMetrics(int codeResponse, double time);

    /// Update the metrics with the new ping (error)
    void updateMetrics(int codeResponse);

    /// Returns the oldestPing taken into account in the metrics
    std::list<Ping>::iterator getOldestPing();

    /// Sets a new oldestPing after old pings removal
    void setOldestPing(std::list<Ping>::iterator newOldestPing);

    /// Removes a ping from the metrics : updates sum, ping count and flags the need of min/max update
    /// Called by Website::deleteOldMetrics
    void removePing(Ping ping);

    /// Updates availability, avg and min/max if needed
    /// Called by Website::deleteOldMetrics
    void updateOldMetrics(const std::list<Ping>& pingList);

    /// Returns initialized to know if the list iterator is well initialized
    bool shouldInitialize();
    /// Initialized sets to true
    void setInitialized();



private:
    /// Updates the new metric min from pingList taking into account the oldestPing to consider
    void updateMin(const std::list<Ping>& pingList);

    /// Updates the new metric max from pingList taking into account the oldestPing to consider
    void updateMax(const std::list<Ping>& pingList);

private:
    Data mData;
    bool minToUpdate;
    bool maxToUpdate;
    std::atomic<bool> initialized;

    /// Iterator through the website's pingList
    /**
     * @brief Represents the oldest ping taken into account in this metrics. Used to only update a limited
     * number of pings when removing the old ones.
     */
    std::list<Ping>::iterator mOldestPing;
    std::mutex mMetricsLock;
};

#endif // METRICS_HPP
