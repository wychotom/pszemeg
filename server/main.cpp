#include <iostream>
#include "eNodeB.h"
#include <string>
#include <ctime>

int main()
{
    srand(time(NULL));

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
}