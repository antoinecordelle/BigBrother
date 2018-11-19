#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include <string>

#include "../Pinger/pinger.hpp"

struct Metrics {
    unsigned int availibility = 0;
    unsigned int maxTime = 0;
    unsigned int avgTime = 0;
    unsigned int pingCount = 0;
};


class Website
{
public:
    Website(std::string name, int interval = 250);

    void run();

private:
    void processPing(std::string pingResponse);

private:
    std::string mName;
    Metrics mMetrics;
    Pinger mPinger;
    int mInterval;
    bool isRunning;

};

#endif // WEBSITE_HPP
