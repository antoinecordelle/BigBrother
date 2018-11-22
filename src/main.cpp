#include <string>
#include <iostream>

#include "Pinger/pinger.hpp"
#include "Website/website.hpp"


using namespace std;

int main()
{
    Website website("google.com", 250, {10});

    website.run();

    Data ok = website.getMetrics(10, true);

    cout << ok.avgTime << " " << ok.maxTime << " " << ok.minTime;

    return 0;
}
