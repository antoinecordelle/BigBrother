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

    while(i++ < 20)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
        Data ok10 = mWebsites[0]->getMetrics(10, false);
        Data ok60 = mWebsites[0]->getMetrics(60, true);
        cout << "10sec : " << ok10.pingCount << " " << ok10.avgTime << " " << ok10.maxTime << " " << ok10.minTime << endl;
        cout << "60sec : " << ok60.pingCount << " " << ok60.avgTime << " " << ok60.maxTime << " " << ok60.minTime << endl << endl;
    }
}
