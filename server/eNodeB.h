#ifndef _ENB
#define _ENB

#include <vector>

#include "PDCCH.h"
#include "PBCH.h"
#include "PDSCH.h"
#include "PRACH.h"
#include "PUSCH.h"
#include "PUCCH.h"
#include "SRB.h"
#include "UE.h"

class eNodeB
{
    public:
        eNodeB();
        void start();
        std::vector<UE*> clients;
        static bool is_running;

    private:
        PDCCH pdcch;
        PBCH pbch;
        PDSCH pdsch;
        PRACH prach;
        PUSCH pusch;
        PUCCH pucch;
        SRB srb;
        std::vector<UE*> ue_to_handle;
};

#endif