#include "eNodeB.h"
#include "channels_struct.h"
#include "Channel.h"
#include <vector>
#include <iostream>

eNodeB::eNodeB() :
pdcch(PORTS::pdcch_port, 0xFFFF),
pbch(PORTS::broadcast_port, 0xFFFFFF),
pdsch(PORTS::dl_sch_port)
{
    Channel::clients = this->clients;
}

eNodeB::~eNodeB()
{
//    for(auto channel : this->channels)
//    {
//        delete channel;
//    }
}

void eNodeB::start()
{
    while(true)
    {
        // PBCH
        if(this->pbch.getCounter() == 0)
        {
            this->pbch.send_mib();
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