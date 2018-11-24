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
    string website;
    int pingInterval;
    int currentLine(1);
    refresh();
    header = initializationBaseWindow(3, COLS, 0, 0, "Initialization", true);
    footer = initializationBaseWindow(4, COLS, 0, LINES - 4, "Name of the website : default to default config, 0 to end");
    mvwprintw(footer, 2, 1, "Check interval : 250 recommended");
    wrefresh(footer);
    input  = initializationBaseWindow(LINES - 3 - 4, COLS, 0, 3, "");
    refresh();
    while(website != "0" && website != "default")
    {
        mvwprintw(input, currentLine, 1, "Name of the website to monitor : ");
        website = Utility::getCursesStr(input, currentLine++, 34);
        if(website == "default")
            websites.push_back(pair<string, int>("default", 0));
        else if(website != "0")
        {
            mvwprintw(input, currentLine, 1, "Check interval in ms : ");
            pingInterval = Utility::getCursesInt(input, currentLine++, 24);
            if(pingInterval <= 50 || pingInterval > 5000)
                pingInterval = 250;
            websites.push_back(pair<string, int>(website, pingInterval));
        }
        if(++currentLine + 1 >= LINES - 7)
        {
            wclear(input);
            box(input, 0,0);
            currentLine = 1;
        }
    }
    delwin(footer);
    delwin(input);
    delwin(header);
    endwin();
    return websites;
}

WINDOW* Dashboard::initializationBaseWindow(int height, int width, int startY, int startX, std::string text, bool center)
{
    WINDOW *win;
    win = newwin(height, width, startX, startY);
    box(win, 0 , 0);
    if(center)
        mvwprintw(win, 1, max(COLS/2 - 6, 0), text.c_str());
    else
        mvwprintw(win, 1, 1, text.c_str());
    wrefresh(win);
    return win;
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

