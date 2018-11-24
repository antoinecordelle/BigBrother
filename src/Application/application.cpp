#include "application.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <ctime>

using namespace std;

Application::Application()
    :mTimeWindows({10,60})
    ,mCycleCounter(0)
{
    sort(mTimeWindows.begin(), mTimeWindows.end());
}


void Application::initialize()
{
    auto websites = mDashboard.initializeWebsites();
    for (auto ite = websites.begin(); ite != websites.end(); ite++)
    {
        if(ite->first == "default")
            loadDefaultWebsites();
        else
            addWebsite(ite->first, ite->second);
    }
}

void Application::loadDefaultWebsites()
{
    addWebsite("google.com", 250);
    addWebsite("datadoghq.com", 250);
}

void Application::addWebsite(string url, int pingInterval)
{
    //Websites on smart_pointers as Websites are non-copyable (Websites own a non-copyable mutex)
    mWebsites.push_back(unique_ptr<Website>(new Website(url, pingInterval, mTimeWindows)));
    mData.push_back(map<time_t, Data>());
}


void Application::run()
{
    initialize();

    launchThreads();

    monitor();

    stopThreads();
}


void Application::launchThreads()
{
    mDashboardThread = thread(&Dashboard::run, &mDashboard);
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mWebsiteThreads.push_back(thread(&Website::run, mWebsites[i].get()));
    }
}

void Application::stopThreads()
{
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mWebsites[i]->stopRunning();
        mWebsiteThreads[i].join();
    }
    mDashboardThread.join();
}

void Application::monitor()
{
    using namespace std::chrono;
    auto timer = system_clock::now();
    if(mWebsites.size() != 0)
    {
        while(mDashboard.isRunning())
        {
            mCycleCounter++;
            timer = system_clock::now();
            for(int websitePos = 0; websitePos != mWebsites.size(); websitePos++)
            {
                for(auto windowIte = mTimeWindows.begin(); windowIte != mTimeWindows.end(); windowIte ++)
                {
                    if(mCycleCounter % 1 == 0)
                        getMetrics(websitePos, *windowIte);
                }
            }
            mDashboard.retrieveData(mData);
            std::this_thread::sleep_for(milliseconds(1000)- duration_cast<milliseconds>(system_clock::now() - timer));
        }
    }
}

void Application::getMetrics(int websitePos, time_t timeWindow)
{
    bool eraseOldPings = (timeWindow == mTimeWindows[mTimeWindows.size() - 1]);
    mData[websitePos][timeWindow] = mWebsites[websitePos]->getMetrics(timeWindow, eraseOldPings);
    if(eraseOldPings)
        mCycleCounter = 0;
}






