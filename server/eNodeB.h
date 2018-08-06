#ifndef _ENB
#define _ENB

#include <queue>

#include "PDCCH.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "UE.h"

class eNodeB
{
    public:
        eNodeB();
        void start();

    private:
        PDCCH pdcch;
        PBCH pbch;
        PDSCH pdsch;
        std::queue<UE*> ue_queue;
};

#endif