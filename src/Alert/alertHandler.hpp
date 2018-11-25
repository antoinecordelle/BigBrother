#ifndef ALERTHANDLER_HPP
#define ALERTHANDLER_HPP

#include <string>
#include <ctime>
#include <vector>

#include "../Metrics/metrics.hpp"
#include "../Website/website.hpp"


/// Struct representing the alerts thrown if a website becomes unavailable or recovers
struct Alert {
    std::string name;
    double availibility;
    time_t timer;

    Alert(Data data);
};

/// Class managing the Alert throws and the status of each website
class AlertHandler
{
public:
    /// Alias defining the statusMap of websites <url, status>
    using StatusMap = std::map<std::string, bool>;
    /// Alias defining a unique_ptr of Website
    using WebsitePtr = std::unique_ptr<Website>;

public:
    AlertHandler();

    /// Initializes the statusMap with the differents websites to monitor
    void initializeStatusMap(const std::vector<WebsitePtr>& websites);

    /// Defines if an alert should be thrown
    /**
     * @param data sent by the application, with name and availability of a website
     * @return true if an alert should be thrown (recovery or down), else false
     */
    bool shouldGetAlert(Data data);

    /// Returns the alert that should be thrown based on data
    /**
     * @param data sent by the application, with name and availability of a website
     * @return an alert (time of the alert, name of the website and availability)
     */
    Alert getAlert(Data data);
    const StatusMap& getStatusMap();

private:
    StatusMap mStatusMap;

};

#endif // ALERTHANDLER_HPP
