#include "alertHandler.hpp"

using namespace std;

Alert::Alert(Data data)
    :name(data.name)
    ,availibility(data.availibility)
    ,timer(time(0))
{
}


AlertHandler::AlertHandler()
{
}


void AlertHandler::initializeStatusMap(const std::vector<WebsitePtr>& websites)
{
    for(unsigned int i = 0; i != websites.size(); i++)
    {
        mStatusMap[websites[i]->getName()] =  true;
    }
}

bool AlertHandler::shouldGetAlert(Data data)
{
    if((data.availibility < 0.8 && mStatusMap[data.name]) || (mStatusMap[data.name] == false && data.availibility >= 0.8))
        return true;
    return false;
}

Alert AlertHandler::getAlert(Data data)
{
    if(data.availibility < 0.8)
    {
        mStatusMap[data.name] = false;
        return Alert(data);
    }
    else
    {
        mStatusMap[data.name] = true;
        return Alert(data);
    }
}

