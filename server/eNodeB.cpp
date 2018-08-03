#include "eNodeB.h"
#include "channels_struct.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() :
pdcch(PORTS::pdcch_port, 0xFFFF),
pbch(PORTS::broadcast_port),
pdsch(PORTS::dl_sch_port)
{
}

void eNodeB::start()
{
    int MIB_iterator = 0;

    while(true)
    {
        // PBCH
        MIB_iterator++;

        if(MIB_iterator%1000000)
        {
            MIB_iterator = 0;
            this->pbch.send_MIB();
        }
        // -----

        // PDCCH
        this->pdcch.handle_connections();

        if(this->pdcch.getCounter() == 0)
        {
            this->pdcch.send_dci_to_all(true);
        }
        // -----

        // PDSCH
        this->pdsch.handle_connections();
//        queue
//        {
//            this->pdsch.send_random_access_response(UE ue);
//        }
        // -----
    }
}