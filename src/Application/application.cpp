#include "application.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>

using namespace std;

Application::Application()
{
    mWebsites.push_back(Website("google.com", 250, {10}));
}

void Application::run()
{
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mThreads.push_back(std::thread(&Website::run, &mWebsites[i]));
    }

    int i(0);

    while(i++ < 20)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        Data ok = mWebsites[0].getMetrics(10, true);
        cout << ok.avgTime << " " << ok.maxTime << " " << ok.minTime << endl;
    }

    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mWebsites[i].stopRunning();
        mThreads[i].join();
    }
}

