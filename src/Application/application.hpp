#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Website/website.hpp"
#include "../Dashboard/dashboard.hpp"
#include "../Alert/alertHandler.hpp"
#include <vector>
#include <thread>

/// Class managing the websites and interface launch, and the link between the two
/**
 * @brief Runs the threads of the websites, the dashboard
 * Gets data from websites and sends them to the AlertHandler and the Dashboard
 */
class Application
{
public:
    /// Alias defining a unique_ptr of Website
    using WebsitePtr = std::unique_ptr<Website>;
    /// Alias defining the statusMap of websites <url, status>
    using StatusMap = std::map<std::string, bool>;

public:
    Application();

    /// Run : launch the threads, the monitoring and interface and finally shuts down the threads and the app
    void run();

private:
    /// Initialization of the app : website creation, dashboard and alertHandler initialization
    void initialize();
    /// Option to launch default websites : google.com, datadoghq.com and a fakewebsite to test alerts
    void loadDefaultWebsites();

    void addWebsite(std::string url, int pingInterval);

    /// Launch the threads
    /**
     * @brief launchThreads : one for each website (pings are done in a separate thread)
     * One thread for the user interface
     */
    void launchThreads();

    /// Stops all the websites and interface threads
    void stopThreads();

    /// Launch the monitoring
    /**
     * @brief monitor : gets the datas from websites, sends them to the AlertHandler and the Dashboard
     */
    void monitor();

    /// Checks and retrieves the data from the different websites
    void checkWebsites();
    /// Asks metrics to the website at position i in mWebsites vector corresponding to the timeWindow
    void getMetrics(int websitePos, time_t timeWindow);

    /// Checks if alerts need to be triggered depending on the data
    void checkAlerts(Data data);

private:
    std::vector<WebsitePtr> mWebsites;
    std::vector<std::thread> mWebsiteThreads;

    AlertHandler mAlertHandler;
    std::vector<Alert> mAlerts;
    time_t mAlertWindow;

    /// StatusMap of the websites (url, status). true if website is up, false if down
    StatusMap mStatusMap;

    /// Represents the data collected : for each website : map of the datas for each timeWindow
    std::vector<std::map<time_t, Data>> mData;
    std::vector<time_t> mTimeWindows;
    std::vector<int> mCheckIntervals;

    std::thread mDashboardThread;
    Dashboard mDashboard;

    int mCycleCounter;
};

#endif // APPLICATION_HPP
