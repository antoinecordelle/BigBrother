#include "alertHandler.hpp"

using namespace std;

Alert::Alert(Data data)
    :name(data.name)
    ,availibility(data.availability)
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
        mStatusMap[websites[i]->getName()] = true;
    }
}

bool AlertHandler::shouldGetAlert(Data data)
{
    // If the website was up and is unavailable ( < 0.8) or if the website was down and now recovered
    return ((data.availability < 0.8 && mStatusMap[data.name]) || (!mStatusMap[data.name] && data.availability >= 0.8));
}

Alert AlertHandler::getAlert(Data data)
{
    if(data.availability < 0.8)
    {
        mStatusMap[data.name] = false;
        return Alert(data);
    }
    mStatusMap[data.name] = true;
    return Alert(data);
}

const AlertHandler::StatusMap& AlertHandler::getStatusMap()
{
    return mStatusMap;
}



