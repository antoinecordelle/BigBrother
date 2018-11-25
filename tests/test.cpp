#include "Metrics/metrics.hpp"
#include "Alert/alertHandler.hpp"
#include "Application/application.hpp"

#include <gtest/gtest.h>
#include <list>

using namespace std;

// Test Alert Logic
TEST ( TestAlertLogic, AlertLogic1)
{
    Data data;
    data.name = "test";
    data.availability = 0.1;
    AlertHandler alertHandler;
    vector<Application::WebsitePtr> websites;
    websites.push_back(Application::WebsitePtr(new Website("test")));
    alertHandler.initializeStatusMap(websites);

    EXPECT_EQ(true, alertHandler.shouldGetAlert(data));

    // Get alert : statusMap["test"] should be set to false
    alertHandler.getAlert(data);
    AlertHandler::StatusMap map = alertHandler.getStatusMap();
    EXPECT_EQ(false, map["test"]);

    data.availability = 0.9;

    EXPECT_EQ(true, alertHandler.shouldGetAlert(data));

    //Get alert : statusMap["test"] should be true : test is back up
    alertHandler.getAlert(data);
    map = alertHandler.getStatusMap();
    EXPECT_EQ(true, map["test"]);
}


// Test Updating old metrics
TEST ( TestUpdateOldMetrics, OldMetrics1)
{
    std::list<Ping> pingList;
    pingList.push_back(Ping(0, 0, 1.f));

    Metrics metrics(pingList.begin(), "test");
    metrics.updateMetrics(0, 1.f);
    metrics.updateOldMetrics(pingList);

    EXPECT_EQ(1, metrics.getMetrics().avgTime);
    EXPECT_EQ(0, metrics.getMetrics().hostUnreachableCount);
    EXPECT_EQ(1, metrics.getMetrics().pingCount);
}

int main(int argc,char *argv[]) {
 ::testing::InitGoogleTest(&argc,argv);
 return RUN_ALL_TESTS();
}
