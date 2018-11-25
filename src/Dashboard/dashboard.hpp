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

/// Console user interface using the ncurses libray
class Dashboard
{
public:
    /// Alias defining the statusMap of websites <url, status>
    using StatusMap = std::map<std::string, bool>;

public:
    Dashboard(time_t alertWindow);

    /// Interface to initialize the websites to check
    /**
     * @brief initializeWebsites : asks the user for websites url and pingIntervals
     * @return vector of pair(url, pingInterval)
     */
    std::vector<std::pair<std::string, int>> initializeWebsites();

    /// Method run : runs to display the monitor interface, menus, details and alerts
    void run();

    bool isRunning();
    void setRunning();

    /// Updates the data displayed, sent by the application
    /**
     * @param data : for each website : map of the datas for each timeWindow
     * @param alerts : vector of all the alerts sent by the AlertHandler to application
     * @param statusMap : statusMap of the websites (url, status)
     */
    void retrieveData(std::vector<std::map<time_t, Data>> data, std::vector<Alert> alerts, StatusMap statusMap);

private:
    /// Displays the menu in the ncurses websitesMenu window
    void displayMenu(WINDOW* websitesMenu);

    /// Displays the details of the selected website in the websiteDetails window
    void displayDetails(WINDOW* websiteDetails);
    /// Displays the data of the selected website in the websiteDetails window. Called by displayDetails
    void displayData(WINDOW* websiteDetails, int position, time_t timerWindow, Data data);
    /// Selects the website focused. Called by user input keys (up and down)
    void focusWebsite(bool next);

    /// Displays the alerts triggered in the alertDisplay window
    void displayAlerts(WINDOW* alertDisplay);
    /// Displays one alert in the alert window. Called by displayAlerts
    void displayOneAlert(WINDOW* alertDisplay, const Alert& alert, int position);

    /// Tool method to initialize a window easily
    WINDOW* initializationBaseWindow(int height, int width, int startY, int startX, std::string text, bool center = false, bool withBox = true, bool title = false);

private:
    std::atomic<bool> mIsRunning;
    std::atomic<bool> shouldRefresh;

    // Represents the data displayed : for each website : map of the datas for each timeWindow
    std::vector<std::map<time_t, Data>> mData;
    std::vector<std::string> mWebsitePos;

    // StatusMap of the websites (url, status). true if website is up, false if down
    StatusMap mStatusMap;

    // Iterator to select the focused and detailed website
    StatusMap::iterator mFocusWebsite;

    // alerts : vector of all the alerts sent by the AlertHandler to application
    std::vector<Alert> mAlerts;
    time_t mAlertWindow;

    std::mutex mDashboardLock;
};

#endif // DASHBOARD_HPP
