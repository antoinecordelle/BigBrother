#include "application.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <chrono>
#include <algorithm>
#include <ctime>

using namespace std;

Application::Application()
    :mAlertWindow(120)
    ,mTimeWindows({120, 600, 3600})
    ,mCheckIntervals({5, 5, 10})
    ,mDashboard(mAlertWindow)
    ,mCycleCounter(0)
{
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
    mAlertHandler.initializeStatusMap(mWebsites);
    mStatusMap = mAlertHandler.getStatusMap();
    mDashboard.retrieveData(mData, mAlerts, mStatusMap);
}

void Application::loadDefaultWebsites()
{
    addWebsite("google.com", 250);
    addWebsite("datadoghq.com", 250);
    addWebsite("fakewebsite", 250);
}

void Application::addWebsite(string url, int pingInterval)
{
    //Websites on smart_pointers as Websites are non-copyable (Websites own a non-copyable mutex)
    mWebsites.push_back(WebsitePtr(new Website(url, pingInterval, mTimeWindows)));
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
            checkWebsites();
            mDashboard.retrieveData(mData, mAlerts, mStatusMap);
            std::this_thread::sleep_for(milliseconds(1000)- duration_cast<milliseconds>(system_clock::now() - timer));
        }
    }
}

void Application::checkWebsites()
{
    for(int websitePos = 0; websitePos != mWebsites.size(); websitePos++)
    {
        for(int windowIte = 0; windowIte != mTimeWindows.size(); windowIte ++)
        {
            if(mCycleCounter % mCheckIntervals[windowIte] == 0)
            {
                getMetrics(websitePos, mTimeWindows[windowIte]);
                if(mTimeWindows[windowIte] == mAlertWindow)
                {
                    checkAlerts(mData[websitePos][mTimeWindows[windowIte]]);
                }
            }
        }
    }
}

void Application::checkAlerts(Data data)
{
    if(mAlertHandler.shouldGetAlert(data))
    {
        mAlerts.push_back(mAlertHandler.getAlert(data));
    }
    mStatusMap = mAlertHandler.getStatusMap();
}

void Application::getMetrics(int websitePos, time_t timeWindow)
{
    bool eraseOldPings = (timeWindow == mTimeWindows[mTimeWindows.size() - 1]);
    mData[websitePos][timeWindow] = mWebsites[websitePos]->getMetrics(timeWindow, eraseOldPings);
    if(eraseOldPings)
        mCycleCounter = 0;
}






