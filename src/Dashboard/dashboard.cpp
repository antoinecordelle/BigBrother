#include "dashboard.hpp"
#include "../Utility/utility.hpp"

#include <ncurses.h>

using namespace std;

Dashboard::Dashboard()
{

}


vector<pair<string, int>> Dashboard::initializeWebsites()
{
    initscr();
    keypad(stdscr, TRUE);

    vector<pair<string, int>> websites;
    string website;
    int pingInterval;

    while(website != "0" && website != "default")
    {
        printw("Name of the website to monitor (default to default config, 0 to end) : ");
        website = Utility::getCursesStr();
        if(website == "default")
            websites.push_back(pair<string, int>("default", 0));
        else if(website != "0")
        {
            printw("Check interval in ms (250 recommended) : ");
            pingInterval = Utility::getCursesInt();
            if(pingInterval <= 0 || pingInterval > 5000)
                pingInterval = 250;
            websites.push_back(pair<string, int>(website, pingInterval));
        }
    }
    endwin();
    return websites;
}


bool Dashboard::isRunning()
{
    return mIsRunning;
}

void Dashboard::run()
{
    mIsRunning = true;
    initscr();
    clear();
    refresh();
    keypad(stdscr, TRUE);
    timeout(1000);
    int input;
    while(isRunning())
    {
        input = getch();
        if(input == KEY_F(1))
            mIsRunning = false;
        if(shouldRefresh)
            displayData();
    }
    endwin();
}

void Dashboard::displayData()
{
    clear();
    Data data;
    for(auto ite = mData.begin(); ite != mData.end(); ite++)
    {
        for(auto dataIte = ite->begin(); dataIte != ite->end(); dataIte ++)
        {
            data = dataIte->second;
            printw(("Past " + to_string(dataIte->first) + "sec :  " + data.name + " : " + to_string(data.avgTime) + "\n").c_str());
        }
    }
    for(auto ite = mAlerts.begin(); ite != mAlerts.end(); ite++)
    {
        printw(("Alert : " + ite->name + " has availibility : " + to_string(ite->availibility) + " at time : \n").c_str());
    }
    refresh();
    shouldRefresh = false;
}

void Dashboard::retrieveData(std::vector<std::map<time_t, Data>> data, std::vector<Alert> alerts)
{
    shouldRefresh = true;
    mAlerts = alerts;
    mData = data;
}

