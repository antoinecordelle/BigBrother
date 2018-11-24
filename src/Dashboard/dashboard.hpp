#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <vector>
#include <string>
#include <atomic>
#include <map>

#include "../Metrics/metrics.hpp"

class Dashboard
{
public:
    Dashboard();

    std::vector<std::pair<std::string, int>> initializeWebsites();
    void run();
    bool isRunning();
    void retrieveData(std::vector<std::map<time_t, Data>> data);

private:
    void displayData();

private:
    std::atomic<bool> mIsRunning;
    std::atomic<bool> shouldRefresh;
    std::vector<std::map<time_t, Data>> mData;
};

#endif // DASHBOARD_HPP
