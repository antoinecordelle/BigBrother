#include "utility.hpp"
#include "ncurses.h"

#include <iostream>

using namespace std;

Utility::Utility()
{

}

int Utility::getCursesInt()
{
    try
    {
        char number[64];
        getnstr(number, 64);
        return atoi(number);
    } catch (const invalid_argument& error)
    {
        cerr << "Invalid argument: " << error.what() << '\n';
    }
    return 0;
}


string Utility::getCursesStr()
{
        char str[128];
        getnstr(str, 128);
        return string(str);
}
