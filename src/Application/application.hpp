#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Website/website.hpp"
#include "../Dashboard/dashboard.hpp"
#include "../Alert/alertHandler.hpp"
#include <vector>
#include <thread>


class Application
{
public:
    using WebsitePtr = std::unique_ptr<Website>;

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
    void checkWebsites();
    void checkAlerts(Data data);
    void getMetrics(int websitePos, time_t timeWindow);

private:
    std::vector<WebsitePtr> mWebsites;
    std::vector<std::thread> mWebsiteThreads;

    AlertHandler mAlertHandler;
    std::vector<Alert> mAlerts;
    time_t mAlertWindow;

    std::vector<std::map<time_t, Data>> mData;
    std::vector<time_t> mTimeWindows;
    std::vector<int> mCheckIntervals;

    std::thread mDashboardThread;
    Dashboard mDashboard;

    int mCycleCounter;
};

#endif // APPLICATION_HPP
