#include "testAlertWebsite.hpp"
#include "../Pinger/testAlertPinger.hpp"


TestAlertWebsite::TestAlertWebsite(std::string name, int interval, std::vector<time_t> mTimeWindows)
    :Website(name, interval, mTimeWindows)
{
    mPinger = std::unique_ptr<Pinger>(new TestAlertPinger(name));
}
