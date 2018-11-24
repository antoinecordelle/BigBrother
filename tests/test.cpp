#include "Metrics/metrics.hpp"

#include <gtest/gtest.h>
#include <list>



TEST ( TestUpdateOldMetrics, OldMetrics1)
{
    std::list<Ping> pingList;
    pingList.push_back(Ping(0, 0, 1.f));

    Metrics metrics(pingList.begin());
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
