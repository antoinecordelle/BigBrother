#include "dashboard.hpp"
#include "../Utility/utility.hpp"
#include <ctime>


using namespace std;

Dashboard::Dashboard(time_t alertWindow)
    :mAlertWindow(alertWindow)
    ,mIsRunning(true)
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
    footer = initializationBaseWindow(4, COLS, LINES - 4, 0, "Url of the website : \"default\" to default config, \"0\" to end");
    mvwprintw(footer, 2, 1, "Ping interval : 250ms recommended. Invalid character set to 250.");
    wrefresh(footer);
    input = initializationBaseWindow(LINES - 3 - 4, COLS, 3, 0, "");
    refresh();

    while(website != "0" && website != "default")
    {
        mvwprintw(input, currentLine, 1, "Website to monitor : ");
        website = Utility::getCursesStr(input, currentLine++, 22);
        if(website.find(' ') != string::npos)
            mvwprintw(input, currentLine, 1, "Invalid URL");
        else if(website == "default")
            websites.push_back(pair<string, int>("default", 0));
        else if(website != "0")
        {
            mvwprintw(input, currentLine, 1, "Ping interval in ms (between 50 and 5000) : ");
            pingInterval = Utility::getCursesInt(input, currentLine++, 45);
            if(pingInterval <= 50 || pingInterval > 5000)
            {
                mvwprintw(input, currentLine++, 1, "Ping Interval set back to 250ms ");
                pingInterval = 250;
            }
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
        else if(!center)
            mvwprintw(win, 1, 1, text.c_str());
        else
            mvwprintw(win, 1, max(COLS/2 - 6, 0), text.c_str());
    }
    else
        mvwprintw(win, 0, 0, text.c_str());
    wrefresh(win);
    return win;
}


bool Dashboard::isRunning()
{
    return mIsRunning;
}

void Dashboard::setRunning()
{
    mIsRunning = true;
}


void Dashboard::run()
{
    WINDOW* footer;
    WINDOW* websitesMenu;
    WINDOW* websiteDetails;
    WINDOW* alertDisplay;

    mFocusWebsite = mStatusMap.begin();
    initscr();
    curs_set(0);
    clear();
    refresh();
    noecho();
    keypad(stdscr, TRUE);
    footer = initializationBaseWindow(1, COLS, LINES - 1, 0, "Press F1 to leave. Press the up and down arrows to navigate through websites", false, false, false);
    websitesMenu = initializationBaseWindow(LINES/2 + 1, COLS/3, 0, 0, "Websites list :", false, false);
    websiteDetails = initializationBaseWindow(LINES/2 + 1, 2*COLS/3 + 1 , 0, COLS/3, "Website details :", false, true, true);
    alertDisplay = initializationBaseWindow(LINES/2 - 2, COLS, LINES/2 +1, 0, "Alerts : ", false, true, true);
    int input;
    int counter;
    timeout(1000);
    while(isRunning())
    {
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
            displayDetails(websiteDetails);
            displayAlerts(alertDisplay);
            displayMenu(websitesMenu);
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
    websitesMenu = initializationBaseWindow(LINES/2 + 1, COLS/3, 0, 0, "Websites list :", false, false);
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
    int position(1);
    bool displayed(false);
    auto website(mData.begin());
    websiteDetails = initializationBaseWindow(LINES/2 + 1, 2*COLS/3 + 1 , 0, COLS/3, (mFocusWebsite->first + " ").c_str(), false, true, true);
    while(website != mData.end() && !displayed)
    {
        //Running through the sorted map of the website's data
        for(auto data = website->begin(); data != website->end(); data++)
        {
            if((data->second).name == mFocusWebsite->first && data->first != mAlertWindow)
            {
                displayData(websiteDetails, position, data->first, data->second);
                position += 6;
                displayed = true;
            }
        }
        website++;
    }
    wrefresh(websiteDetails);
}

void Dashboard::displayData(WINDOW* websiteDetails, int position, time_t timerWindow, Data data)
{
    mvwprintw(websiteDetails, position, 2, ("Last " + to_string(timerWindow/60) + " minutes : ").c_str());
    mvwprintw(websiteDetails, position + 1, 2, ("Availability : " + to_string(data.availability)).c_str());
    mvwprintw(websiteDetails, position + 2, 2, "Response time : (min/avg/max)");
    if(data.pingCount - data.hostUnreachableCount != 0)
        mvwprintw(websiteDetails, position + 3, 2, ("     " + to_string(data.minTime) + "ms / " + to_string(data.avgTime) + "ms / " + to_string(data.maxTime) + "ms").c_str());
    else
        mvwprintw(websiteDetails, position + 3, 2, "     NA / NA / NA");
    mvwprintw(websiteDetails, position + 4, 2,
              ("Total : " + to_string(data.pingCount) +
               " / Code 200 OK : " + to_string(data.pingCount - data.hostUnreachableCount) +
               " / Ping Error : " + to_string(data.hostUnreachableCount)).c_str());
}

void Dashboard::displayAlerts(WINDOW* alertDisplay)
{
    std::lock_guard<std::mutex> lock(mDashboardLock);
    wclear(alertDisplay);
    alertDisplay = initializationBaseWindow(LINES/2 - 2, COLS, LINES/2 +1, 0, "Alerts : ", false, true, true);
    int position(0);
    for(unsigned int i = max(0, (int)mAlerts.size() - 8); i != mAlerts.size(); i++)
    {
        displayOneAlert(alertDisplay, mAlerts[i], position++);
    }
    wrefresh(alertDisplay);
}


void Dashboard::displayOneAlert(WINDOW* alertDisplay, const Alert& alert, int position)
{
    char buffer [80];
    time_t timer(alert.timer);
    std::time(&timer);
    struct tm* timeinfo = localtime(&timer);
    strftime (buffer,80,"%D %I:%M%p  ",timeinfo);
    mvwprintw(alertDisplay, position + 1, 1, buffer);
    if(alert.availibility < 0.8)
    {
        mvwprintw(alertDisplay, position + 1, 20, (" DOWN    " + alert.name).c_str());
    }
    else
    {
        mvwprintw(alertDisplay, position + 1, 20, (" UP     " + alert.name).c_str());
    }
    mvwprintw(alertDisplay, position + 1, 50, ("Availibility : " + to_string(alert.availibility)).c_str());
}



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






