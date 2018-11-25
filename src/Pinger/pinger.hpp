#ifndef PINGER_H
#define PINGER_H

#include <string>
#include <memory>


/// Class used as an engine to ping websites and return responses as strings
class Pinger
{
public:
    /// Constructor initializing the Pinger
    /**
     * @param name : string allowing to build the ping command
     */
    Pinger(std::string name);

    /// Method pinging a website and returning the response
    /**
     * @return string : returns the ping response + the response code at the end of the string
     */
    virtual std::string ping();


private:
    /// mCommand used to ping websites : ping -c 1 -W 1 -q name  2>&1 ; echo $?
    /**
     * @brief -c 1 : one ping per command
     * -W 1 : Deadline if the host is unreachable
     * -q : quiet ping : limits the size of the string
     * 2>&1 : allows to combine the error output and the content output to detect ping errors
     * echo $? : writes the response code at the end of the response
     */
    std::string mCommand;
};

#endif // PINGER_H
