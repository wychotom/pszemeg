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
#include "DRB.h"
#include "UE.h"
#include "Antenna.h"

class eNodeB
{
    public:
        eNodeB();
        ~eNodeB();
        void start();
        static bool is_running;

    private:
        Antenna antenna;

        PBCH pbch;
        PDCCH pdcch;
        PDSCH pdsch;
        PRACH prach;
        PUSCH pusch;
        PUCCH pucch;
        SRB srb;
        DRB drb;

        std::vector<UE*> clients;
        std::vector<UE*> ue_to_handle;
};

#endif