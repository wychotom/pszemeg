#include <iostream>
#include "eNodeB.h"
#include <string>
#include <csignal>

void stop_handler(int sig)
{
    if (sig == SIGINT)
    {
        std::cout << "STOPPPPPP" << std::endl;
        eNodeB::is_running = false;
    }
}

int main()
{
    signal(SIGINT, stop_handler);

    try
    {
        eNodeB enb;
        std::cout << "\n\nJESZCZE DZIALA :D\n\n";
        enb.start();
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
    }

    std::cout << "MAIN STOPPPPPP" << std::endl;
}