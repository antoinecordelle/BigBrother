#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Website/website.hpp"
#include "../Dashboard/dashboard.hpp"
#include <vector>
#include <thread>


class Application
{
public:
    Application();
    void run();
    void stopApp();

private:
    void launchThreads();
    void stopThreads();
    void monitor();
    void initialize();
    void loadDefaultWebsites();
    void addWebsite(std::string url, int pingInterval);
    void getMetrics(int websitePos, time_t timeWindow);

private:
    std::vector<std::unique_ptr<Website>> mWebsites;
    std::vector<std::thread> mWebsiteThreads;

    std::vector<std::map<time_t, Data>> mData;
    std::vector<time_t> mTimeWindows;

    std::thread mDashboardThread;
    Dashboard mDashboard;
    int mCycleCounter;
};

#endif // APPLICATION_HPP
