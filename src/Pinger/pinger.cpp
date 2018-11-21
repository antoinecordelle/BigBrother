#include "pinger.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;

Pinger::Pinger(std::string name)
    :mCommand("ping -c 1 -W 1 -q " + name + " 2>&1 ; echo $?")
{
}

string Pinger::ping()
{
    char buffer [100];
    FILE* pingRequest;
    string response;

    pingRequest = popen(mCommand.c_str(), "r");

    if (pingRequest == NULL) perror ("Error sending request");
    else
        {
        while ( ! feof(pingRequest) )
        {
            if ( fgets (buffer , 100 , pingRequest) == NULL ) break;
            response += buffer;
        }
        fclose (pingRequest);
    }
    cout << response << endl;
    return response;
}
