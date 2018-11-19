#include <string>
#include <iostream>
#include "Pinger/pinger.hpp"

using namespace std;

int main()
{
    Pinger pinger1("8.8.8.8", 1);
    Pinger pinger2("10.8.8.8", 1);

    pinger1.ping();
    pinger2.ping();
}

