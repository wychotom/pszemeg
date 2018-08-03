#ifndef _ENB
#define _ENB

#include "Channel.h"
#include "PDCCH.h"
#include "PBCH.h"
#include "PDSCH.h"
#include <vector>

class eNodeB
{
    public:
        eNodeB();
        void start();

    private:
        PDCCH pdcch;
        PBCH pbch;
        PDSCH pdsch;
};

#endif