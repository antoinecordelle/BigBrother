#ifndef ALERTHANDLER_HPP
#define ALERTHANDLER_HPP

#include <string>
#include <ctime>
#include <vector>

#include "../Metrics/metrics.hpp"
#include "../Website/website.hpp"

struct Alert {
    std::string name;
    double availibility;
    time_t timer;

    Alert(Data data);
};

class AlertHandler
{
public:
    using StatusMap = std::map<std::string, bool>;
    using WebsitePtr = std::unique_ptr<Website>;

public:
    AlertHandler();

    void initializeStatusMap(const std::vector<WebsitePtr>& websites);
    bool shouldGetAlert(Data data);
    Alert getAlert(Data data);

private:
    StatusMap mStatusMap;

};

#endif // ALERTHANDLER_HPP
