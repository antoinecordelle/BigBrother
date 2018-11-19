#include "website.hpp"

#include <iostream>
#include <unistd.h>
#include <chrono>
#include <thread>


using namespace std;

Website::Website(std::string name, int interval)
    :mName(name)
    ,mPinger(name)
    ,mInterval(interval)
    ,isRunning(true)
{
}

void Website::run()
{
    int i(0);
    while(isRunning)
    {
        processPing(mPinger.ping());
        this_thread::sleep_for(chrono::milliseconds(mInterval));
        i++;
        if(i == 10)
            isRunning = false;
    }
}

void Website::processPing(std::string pingResponse)
{
    cout << pingResponse << endl;
}
