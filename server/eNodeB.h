#ifndef _ENB
#define _ENB

#include <queue>
#include <vector>

#include "PDCCH.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PRACH.h"
#include "PUSCH.h"
#include "PUCCH.h"
#include "UE.h"

enum Ports
{
    pbch_port = 20700,
    pdsch_port = 20701,
    pusch_port = 20702,
    pdcch_port = 20703,
    pucch_port = 20704,
    prach_port = 20705
};

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
        PUCCH pucch;
        std::queue<UE*> ue_queue;
};

#endif