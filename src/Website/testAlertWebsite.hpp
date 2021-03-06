#ifndef TESTALERTWEBSITE_HPP
#define TESTALERTWEBSITE_HPP

#include "website.hpp"

/// Class using a fakewebsite to test the Alert Logic. Uses a TestAlertPinger
class TestAlertWebsite : public Website
{
public:
    TestAlertWebsite(std::string name, int interval = 250, std::vector<time_t> mTimeWindows = std::vector<time_t>({120, 600, 3600}));
};

#endif // TESTALERTWEBSITE_HPP
