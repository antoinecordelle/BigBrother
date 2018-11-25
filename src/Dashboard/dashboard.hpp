#ifndef DASHBOARD_HPP
#define DASHBOARD_HPP

#include <vector>
#include <string>
#include <atomic>
#include <map>
#include <mutex>
#include <ncurses.h>

#include "../Alert/alertHandler.hpp"
#include "../Metrics/metrics.hpp"

class Dashboard
{
public:
    using StatusMap = std::map<std::string, bool>;

public:
    Dashboard();

    std::vector<std::pair<std::string, int>> initializeWebsites();
    void run();
    bool isRunning();
    void retrieveData(std::vector<std::map<time_t, Data>> data, std::vector<Alert> alerts, StatusMap statusMap);

private:
    void displayMenu(WINDOW* websitesMenu);
    void displayDetails(WINDOW* websiteDetails);
    void displayAlerts(WINDOW* alertDisplay);
    WINDOW* initializationBaseWindow(int height, int width, int startY, int startX, std::string text, bool center = false, bool withBox = true, bool title = false);
    void focusWebsite(bool next);

private:
    std::atomic<bool> mIsRunning;
    std::atomic<bool> shouldRefresh;
    std::vector<std::map<time_t, Data>> mData;
    std::vector<Alert> mAlerts;
    StatusMap mStatusMap;
    StatusMap::iterator mFocusWebsite;
    std::mutex mDashboardLock;
};

#endif // DASHBOARD_HPP
