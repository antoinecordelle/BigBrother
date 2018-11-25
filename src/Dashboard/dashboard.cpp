#include "dashboard.hpp"
#include "../Utility/utility.hpp"


using namespace std;

Dashboard::Dashboard()
{

}


vector<pair<string, int>> Dashboard::initializeWebsites()
{
    WINDOW* header;
    WINDOW* input;
    WINDOW* footer;
    initscr();
    keypad(stdscr, TRUE);

    vector<pair<string, int>> websites;
    mFocusWebsite = mStatusMap.begin();
    string website;
    int pingInterval;
    int currentLine(1);
    refresh();
    header = initializationBaseWindow(3, COLS, 0, 0, "Initialization", true);
    footer = initializationBaseWindow(4, COLS, LINES - 4, 0, "Name of the website : \"default\" to default config, \"0\" to end");
    mvwprintw(footer, 2, 1, "Ping interval : 250ms recommended. Invalid character set to 250.");
    wrefresh(footer);
    input = initializationBaseWindow(LINES - 3 - 4, COLS, 3, 0, "");
    refresh();

    while(website != "0" && website != "default")
    {
        mvwprintw(input, currentLine, 1, "Website to monitor : ");
        website = Utility::getCursesStr(input, currentLine++, 22);
        if(website == "default")
            websites.push_back(pair<string, int>("default", 0));
        else if(website != "0")
        {
            mvwprintw(input, currentLine, 1, "Ping interval in ms : ");
            pingInterval = Utility::getCursesInt(input, currentLine++, 23);
            if(pingInterval <= 50 || pingInterval > 5000)
                pingInterval = 250;
            websites.push_back(pair<string, int>(website, pingInterval));
        }
        if(++currentLine + 1 >= LINES - 7)
        {
            wclear(input);
            box(input, 0, 0);
            currentLine = 1;
        }
    }
    delwin(footer);
    delwin(input);
    delwin(header);
    endwin();
    return websites;
}

WINDOW* Dashboard::initializationBaseWindow(int height, int width, int startY, int startX, std::string text, bool center, bool withBox, bool title)
{
    WINDOW* win;
    win = newwin(height, width, startY, startX);
    if(withBox)
    {
        box(win, 0 , 0);
        if(title)
            mvwprintw(win, 0, 1, text.c_str());
        else
            mvwprintw(win, 1, 1, text.c_str());
    }
    else if(center)
        mvwprintw(win, 1, max(COLS/2 - 6, 0), text.c_str());
    else
        mvwprintw(win, 0, 1, text.c_str());
    wrefresh(win);
    return win;
}


bool Dashboard::isRunning()
{
    return mIsRunning;
}

void Dashboard::run()
{
    WINDOW* footer;
    WINDOW* websitesMenu;
    WINDOW* websiteDetails;
    WINDOW* alertDisplay;

    mIsRunning = true;
    initscr();
    clear();
    refresh();
    noecho();
    keypad(stdscr, TRUE);
    footer = initializationBaseWindow(1, COLS, LINES - 1, 0, "Press F1 to leave. Press the up and down arrows to navigate through websites", false, false, false);
    websitesMenu = initializationBaseWindow(LINES/2, COLS/3, 0, 0, "Websites list :", false, false);
    websiteDetails = initializationBaseWindow(LINES/2, 2*COLS/3 + 1 , 0, COLS/3, "Website details :", false, true, true);
    alertDisplay = initializationBaseWindow(LINES/2 - 1, COLS, LINES/2, 0, "Alerts : ", false, true, true);
    int input;
    int counter;
    timeout(1000);
    while(isRunning())
    {
        mvwprintw(alertDisplay, 1, 0, to_string(counter++).c_str());
        mvwprintw(websiteDetails, 1, 0, to_string(mStatusMap.size()).c_str());
        wrefresh(websiteDetails);
        wrefresh(alertDisplay);
        input = getch();
        switch(input)
        {
        case KEY_F(1):
            mIsRunning = false;
            break;
        case KEY_UP:
            shouldRefresh = true;
            focusWebsite(true);
            break;
        case KEY_DOWN:
            shouldRefresh = true;
            focusWebsite(false);
            break;
        default:
            break;
        }
        if(shouldRefresh)
        {
            shouldRefresh = false;
            displayMenu(websitesMenu);
            //displayDetails(websiteDetails);
            //displayAlerts(alertDisplay);
        }
    }

    delwin(footer);
    delwin(websitesMenu);
    delwin(websiteDetails);
    delwin(alertDisplay);
    endwin();
}


void Dashboard::displayMenu(WINDOW* websitesMenu)
{
    std::lock_guard<std::mutex> lock(mDashboardLock);
    int line(1);
    wclear(websitesMenu);
    websitesMenu = initializationBaseWindow(LINES/2, COLS/3, 0, 0, "Websites list :", false, false);
    for(auto websiteIte = mStatusMap.begin(); websiteIte != mStatusMap.end(); websiteIte++)
    {
        if(websiteIte->first == mFocusWebsite->first)
            mvwprintw(websitesMenu, line, 0, "+");
        if(websiteIte->second == true)
            mvwprintw(websitesMenu, line, 2, "UP");
        else
            mvwprintw(websitesMenu, line, 2, "DO");
        mvwprintw(websitesMenu, line++, 5, (websiteIte->first).c_str());
    }

    wrefresh(websitesMenu);
}

void Dashboard::displayDetails(WINDOW* websiteDetails)
{
    std::lock_guard<std::mutex> lock(mDashboardLock);
    wclear(websiteDetails);
    websiteDetails = initializationBaseWindow(LINES/2, 2*COLS/3 + 1 , 0, COLS/3, "Website details :", false, true, true);

    wrefresh(websiteDetails);
}

void Dashboard::displayAlerts(WINDOW* alertDisplay)
{
    std::lock_guard<std::mutex> lock(mDashboardLock);
    wclear(alertDisplay);
    alertDisplay = initializationBaseWindow(LINES/2 - 1, COLS, LINES/2, 0, "Alerts : ", false, true, true);

    wrefresh(alertDisplay);
}
/*
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
}*/

void Dashboard::retrieveData(std::vector<std::map<time_t, Data>> data, std::vector<Alert> alerts, Dashboard::StatusMap statusMap)
{
    std::lock_guard<std::mutex> lock(mDashboardLock);
    shouldRefresh = true;
    mAlerts = alerts;
    mData = data;
    for(auto ite = statusMap.begin(); ite != statusMap.end(); ite++)
    {
        mStatusMap[ite->first] = ite->second;
    }
}

void Dashboard::focusWebsite(bool next)
{
    std::lock_guard<std::mutex> lock(mDashboardLock);
    if(next)
    {
        if(mFocusWebsite != mStatusMap.begin())
            mFocusWebsite--;
    }
    else
        if(++mFocusWebsite == mStatusMap.end())
            mFocusWebsite--;

}






