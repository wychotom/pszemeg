#ifndef ENB
#define ENB

#include "Channel.h"
#include <vector>

class eNodeB
{
    public:
        eNodeB();
        ~eNodeB();
        void start();

    private:
        std::vector<Channel*> channels;
};

#endif