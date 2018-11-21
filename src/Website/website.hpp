#ifndef WEBSITE_HPP
#define WEBSITE_HPP

#include <string>

#include "../Pinger/pinger.hpp"

struct Metrics {
    unsigned int hostUnreachableCount = 0;
    unsigned int maxTime = 0;
    unsigned int minTime = 0;
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
    int getResponseCode(const std::string& pingResponse) const;
    double getResponseTime(const std::string& pingResponse) const;
    void updateMetrics(int codeResponse);
    void updateMetrics(int codeResponse, double time);

private:
    const std::string mName;
    Metrics mMetrics;
    Pinger mPinger;
    const int mInterval;
    bool isRunning;

};

#endif // WEBSITE_HPP
