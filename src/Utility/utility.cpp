#include "utility.hpp"
#include <iostream>

using namespace std;

Utility::Utility()
{

}

int Utility::getCursesInt(WINDOW* win, int line, int col)
{
    try
    {
        char number[64];
        mvwgetnstr(win, line, col, number, 64);
        return atoi(number);
    } catch (const invalid_argument& error)
    {
        cerr << "Invalid argument: " << error.what() << '\n';
    }
    return 0;
}


string Utility::getCursesStr(WINDOW* win, int line, int col)
{
    char str[128];
    mvwgetnstr(win, line, col, str, 128);
    return string(str);
}
