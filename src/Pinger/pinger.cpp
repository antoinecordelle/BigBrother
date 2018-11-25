#include "pinger.hpp"

#include <iostream>
#include <stdio.h>

using namespace std;

Pinger::Pinger(std::string name)
/// mCommand used to ping websites : ping -c 1 -W 1 -q name  2>&1 ; echo $?
/**
 * @brief -c 1 : one ping per command
 * -W 1 : Deadline if the host is unreachable
 * -q : quiet ping : limits the size of the string
 * 2>&1 : allows to combine the error output and the content output to detect ping errors
 * echo $? : writes the response code at the end of the response
 */
    :mCommand("ping -c 1 -W 1 -q " + name + " 2>&1 ; echo $?")
{
}

string Pinger::ping()
{
    char buffer [100];
    FILE* pingRequest;
    string response;

    // Open the pipe sending the ping (mCommand)
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
    return response;
}
