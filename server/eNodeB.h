#ifndef _ENB
#define _ENB

#include "Channel.h"
#include "UE.h"
#include "PDCCH.h"
#include "PBCH.h"
#include "PDSCH.h"
#include <vector>
#include <queue>

class eNodeB
{
    public:
        eNodeB();
        void start();

    private:
        PDCCH pdcch;
        PBCH pbch;
        PDSCH pdsch;
        std::queue<int> ue_queue;
        std::vector<UE> clients;
};

#endif