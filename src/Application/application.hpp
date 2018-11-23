#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "../Website/website.hpp"
#include <vector>
#include <thread>

class Application
{
public:
    Application();
    void run();

private:
    std::vector<Website> mWebsites;
    std::vector<std::thread> mThreads;
};

#endif // APPLICATION_HPP
