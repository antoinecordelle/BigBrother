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





