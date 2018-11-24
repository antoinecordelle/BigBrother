#include "application.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Application::Application()
{
    //Websites on smart_pointers as Websites are non-copyable (Websites own a non-copyable mutex)
    mWebsites.push_back(unique_ptr<Website>(new Website("google.com", 250, {10, 60})));
}


void Application::run()
{
    launchThreads();

    monitor();

    stopThreads();
}


void Application::launchThreads()
{
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mThreads.push_back(thread(&Website::run, mWebsites[i].get()));
    }
}

void Application::stopThreads()
{
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mWebsites[i]->stopRunning();
        mThreads[i].join();
    }
}

void Application::monitor()
{
    int i(0);

    while(i++ < 1000)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
        for(int i = 0; i != mWebsites.size(); i++)
        {
            Data ok10 = mWebsites[i]->getMetrics(10, false);
            Data ok60 = mWebsites[i]->getMetrics(60, true);
            cout << "10sec : " << ok10.pingCount << " " << ok10.avgTime << " " << ok10.maxTime << " " << ok10.minTime << "  " << ok10.hostUnreachableCount << endl;
            cout << "60sec : " << ok60.pingCount << " " << ok60.avgTime << " " << ok60.maxTime << " " << ok60.minTime << "  " << ok60.hostUnreachableCount << endl << endl;
        }

    }
}
