#include "pinger.hpp"

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdio.h>

using namespace std;

Pinger::Pinger(std::string name, int interval)
    :mCommand("ping -c 1 -W 1 " + name)
    ,mInterval(interval)
{
}

int Pinger::ping()
{
    char buffer [100];
    FILE* file;

    file = popen(mCommand.c_str(), "r");

    sleep(mInterval);
    if (file == NULL) perror ("Error opening file");
    else
        {
        while ( ! feof(file) )
        {
            if ( fgets (buffer , 100 , file) == NULL ) break;
                fputs (buffer , stdout);
        }
        fclose (file);
    }
    cout << buffer << endl << endl;
    return 0;
}
