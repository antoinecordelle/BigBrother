#include "application.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Application::Application()
{
    //Websites on smart_pointers as Websites are non-copyable (Websites own a non-copyable mutex)
    mWebsites.push_back(unique_ptr<Website>(new Website("google.com", 250, {10})));
}

void Application::run()
{
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mThreads.push_back(thread(&Website::run, mWebsites[i].get()));
    }

    int i(0);

    while(i++ < 20)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
        Data ok = mWebsites[0]->getMetrics(10, true);
        cout << ok.avgTime << " " << ok.maxTime << " " << ok.minTime << endl;
    }

    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mWebsites[i]->stopRunning();
        mThreads[i].join();
    }
}

