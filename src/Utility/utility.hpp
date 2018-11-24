#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <string>
#include "ncurses.h"

class Utility
{
public:
    Utility();
    static int getCursesInt(WINDOW* win, int line, int col = 1);
    static std::string getCursesStr(WINDOW* win, int line, int col = 1);

};

#endif // UTILITY_HPP
