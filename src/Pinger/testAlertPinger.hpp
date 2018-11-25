#ifndef TESTALERTPINGER_HPP
#define TESTALERTPINGER_HPP

#include "pinger.hpp"

/// Pinger : Class used as an engine to test and demonstrate the alerting logic
/**
 * @brief Will send 100 failing pings to detect an anomaly and trigger an alert
 * Then it will ping google.com, returning normal pings and triggering the recover alert
 */
class TestAlertPinger : public Pinger
{
public:
    TestAlertPinger(std::string name);

    /// Method pinging a website and returning the response
    /**
     * @return string : returns the ping response + the response code at the end of the string
     */
    virtual std::string ping();

private:
    /// Counts the number of failing pings before sending real pings
    int mCounter;
};

#endif // TESTALERTPINGER_HPP
