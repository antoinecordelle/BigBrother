#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Website/website.hpp"
#include "../Dashboard/dashboard.hpp"
#include <vector>
#include <thread>

class Application
{
public:
    Application();
    void run();

private:
    void launchThreads();
    void stopThreads();
    void monitor();

private:
    std::vector<std::unique_ptr<Website>> mWebsites;
    std::vector<std::thread> mThreads;
    Dashboard mDashboard;
};

#endif // APPLICATION_HPP
