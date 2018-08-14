#include <iostream>
#include "eNodeB.h"
#include "Log.h"
#include <string>
#include <csignal>

void stop_handler(int sig)
{
    if (sig == SIGINT)
    {
        eNodeB::is_running = false;
    }
}

int main()
{
    signal(SIGINT, stop_handler);

    try
    {
        eNodeB enb;
        Log::general("\n\nPROGRAMMET FUNGERAR\n\n");
        enb.start();
    }
    catch(std::string &e)
    {
        Log::error(e);
    }

    Log::general("\n\nMAIN HALT\n\n");
}