#ifndef _ENB
#define _ENB

#include <queue>
#include <vector>

#include "PDCCH.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PRACH.h"
#include "PUSCH.h"
#include "UE.h"

class eNodeB
{
    public:
        eNodeB();
        void start();
        std::vector<UE*> clients;

    private:
        PDCCH pdcch;
        PBCH pbch;
        PDSCH pdsch;
        PRACH prach;
        PUSCH pusch;
        std::queue<UE*> ue_queue;
};

#endif