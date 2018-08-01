#include <iostream>
#include "eNodeB.h"
#include <string>

int main()
{
    try
    {
        eNodeB enb;
        std::cout << "\n\nJESZCZE DZIALA :D\n\n";
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
    }
}