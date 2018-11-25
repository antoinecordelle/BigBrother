#include "application.hpp"
#include "../Website/testAlertWebsite.hpp"

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
    ,mCheckIntervals({10, 10, 60})
    ,mDashboard(mAlertWindow)
    ,mCycleCounter(0)
{
}


void Application::initialize()
{
    // Initialization of the websites through the user interface
    auto websites = mDashboard.initializeWebsites();
    for (auto ite = websites.begin(); ite != websites.end(); ite++)
    {
        if(ite->first == "default")
            loadDefaultWebsites();
        else
            addWebsite(ite->first, ite->second);
    }
    // Initialization of the AlertHandler and the Dashboard main menu
    mAlertHandler.initializeStatusMap(mWebsites);
    mStatusMap = mAlertHandler.getStatusMap();
    mDashboard.retrieveData(mData, mAlerts, mStatusMap);
}

void Application::loadDefaultWebsites()
{
    addWebsite("google.com", 250);
    addWebsite("datadoghq.com", 500);
    addWebsite("fakewebsite", 250);
}

void Application::addWebsite(string url, int pingInterval)
{
    //Websites on smart_pointers as Websites are non-copyable (Websites own a non-copyable mutex)
    if(url == "fakewebsite")
        mWebsites.push_back(WebsitePtr(new TestAlertWebsite(url, pingInterval, mTimeWindows)));
    else
    {
        mWebsites.push_back(WebsitePtr(new Website(url, pingInterval, mTimeWindows)));
    }
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
    // Launch the dashboard main menu in its thread
    mDashboard.setRunning();
    mDashboardThread = thread(&Dashboard::run, &mDashboard);
    // Launch the different websites : each website will ping from its thread
    for(unsigned int i = 0; i != mWebsites.size(); i++)
    {
        mWebsiteThreads.push_back(thread(&Website::run, mWebsites[i].get()));
    }
}

void Application::stopThreads()
{
    // Shuts down the different threads (websites and dashboard)
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
            // Each loop will last 1 sec
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
    // Checks the websites, retrieving their data, and checks for alerts
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
    // If the timeWindow is the last one, we can erase the ping from the website's pingList
    bool eraseOldPings = (timeWindow == mTimeWindows[mTimeWindows.size() - 1]);
    mData[websitePos][timeWindow] = mWebsites[websitePos]->getMetrics(timeWindow, eraseOldPings);
    if(eraseOldPings)
        mCycleCounter = 0;
}






