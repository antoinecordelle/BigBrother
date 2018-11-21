#include <string>
#include <iostream>

#include "Pinger/pinger.hpp"
#include "Website/website.hpp"

using namespace std;

int main()
{
    Website website("google.com", 500);

    website.run();

    Data ok = website.getMetrics();

    cout << ok.avgTime << " " << ok.maxTime << " " << ok.minTime;

    return 0;
}

